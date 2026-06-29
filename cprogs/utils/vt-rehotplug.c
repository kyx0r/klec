/*
 * vt-rehotplug — work around libudev-zero + Xorg live input hotplug.
 *
 * Problem: Xorg's config/udev *monitor* never delivers live add events under
 * libudev-zero, so a replugged mouse/keyboard is not re-added. Xorg's *enumerate*
 * path DOES work, and it re-runs on every VT enter (EnterVT). So: watch
 * /dev/input for new device nodes and bounce the VT, forcing Xorg to re-scan.
 *
 * Build:  cc -O2 -o vt-rehotplug vt-rehotplug.c
 * Run as root (needs /dev/tty0). Foreground; let your init supervise it.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <dirent.h>
#include <sys/inotify.h>
#include <sys/ioctl.h>
#include <linux/vt.h>

#define INPUT_DIR   "/dev/input"
#define CONSOLE     "/dev/tty0"
#define X11_SOCKDIR "/tmp/.X11-unix"
#define DEBOUNCE_MS 400

static volatile sig_atomic_t running = 1;
static void on_signal(int s) { (void)s; running = 0; }

static void logmsg(const char *m)
{
    char ts[32];
    time_t t = time(NULL);
    struct tm tm;
    localtime_r(&t, &tm);
    strftime(ts, sizeof ts, "%H:%M:%S", &tm);
    fprintf(stderr, "[%s] vt-rehotplug: %s\n", ts, m);
    fflush(stderr);
}

/* Only bother bouncing if an X server is actually up. */
static int x_is_running(void)
{
    DIR *d = opendir(X11_SOCKDIR);
    if (!d) return 0;
    struct dirent *e;
    int found = 0;
    while ((e = readdir(d))) {
        if (e->d_name[0] == 'X') { found = 1; break; }   /* X0, X1, ... */
    }
    closedir(d);
    return found;
}

/* Switch to another VT and back -> triggers Xorg EnterVT -> re-enumerate. */
static void vt_bounce(void)
{
    int fd = open(CONSOLE, O_RDWR | O_NOCTTY);
    if (fd < 0) { logmsg("open /dev/tty0 failed"); return; }

    struct vt_stat st;
    if (ioctl(fd, VT_GETSTATE, &st) < 0) { logmsg("VT_GETSTATE failed"); close(fd); return; }

    int cur = st.v_active;
    int target = (cur == 1) ? 2 : 1;   /* any VT that isn't the current one */

    if (ioctl(fd, VT_ACTIVATE, target) == 0) {
        ioctl(fd, VT_WAITACTIVE, target);
        ioctl(fd, VT_ACTIVATE, cur);
        ioctl(fd, VT_WAITACTIVE, cur);
        logmsg("bounced VT, Xorg re-enumerated input");
    } else {
        logmsg("VT_ACTIVATE failed");
    }
    close(fd);
}

/* Drain all pending inotify events; return 1 if any new event* node appeared. */
static int drain(int ifd, char *buf, size_t bufsz)
{
    int hit = 0;
    for (;;) {
        ssize_t n = read(ifd, buf, bufsz);
        if (n <= 0) break;
        for (char *p = buf; p < buf + n; ) {
            struct inotify_event *ev = (struct inotify_event *)p;
            if (ev->len && strncmp(ev->name, "event", 5) == 0)
                hit = 1;
            p += sizeof(struct inotify_event) + ev->len;
        }
        if ((size_t)n < bufsz) break;   /* probably nothing more queued */
    }
    return hit;
}

int main(void)
{
    signal(SIGTERM, on_signal);
    signal(SIGINT,  on_signal);

    int ifd = inotify_init1(IN_CLOEXEC | IN_NONBLOCK);
    if (ifd < 0) { perror("inotify_init1"); return 1; }
    if (inotify_add_watch(ifd, INPUT_DIR, IN_CREATE) < 0) {
        perror("inotify_add_watch " INPUT_DIR);
        return 1;
    }

    char buf[8192] __attribute__((aligned(__alignof__(struct inotify_event))));
    struct pollfd pfd = { .fd = ifd, .events = POLLIN };

    logmsg("watching " INPUT_DIR);

    while (running) {
        int n = poll(&pfd, 1, -1);
        if (n < 0) { if (errno == EINTR) continue; break; }
        if (!(pfd.revents & POLLIN)) continue;

        if (!drain(ifd, buf, sizeof buf)) continue;   /* not an event* node */

        /* Debounce: a single replug spawns several nodes (eventN, mouseN, ...).
           Wait until the burst goes quiet, then bounce exactly once. */
        for (;;) {
            int m = poll(&pfd, 1, DEBOUNCE_MS);
            if (m <= 0) break;
            drain(ifd, buf, sizeof buf);
        }

        if (x_is_running())
            vt_bounce();
        else
            logmsg("input added but no X server; skipping");
    }

    logmsg("exiting");
    close(ifd);
    return 0;
}
