#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <gdk/gdkkeysyms.h>
#include <gio/gio.h>
#include <webkit2/webkit2.h>
#include <JavaScriptCore/JavaScript.h>


void client_destroy(GtkWidget *, gpointer);
WebKitWebView *client_new(const gchar *, WebKitWebView *, gboolean,
                          gboolean);
WebKitWebView *client_new_request(WebKitWebView *, WebKitNavigationAction *,
                                  gpointer);
void cooperation_setup(void);
void changed_download_progress(GObject *, GParamSpec *, gpointer);
void changed_load_progress(GObject *, GParamSpec *, gpointer);
void changed_favicon(GObject *, GParamSpec *, gpointer);
void changed_title(GObject *, GParamSpec *, gpointer);
void changed_uri(GObject *, GParamSpec *, gpointer);
gboolean crashed_web_view(WebKitWebView *, gpointer);
gboolean decide_policy(WebKitWebView *, WebKitPolicyDecision *,
                       WebKitPolicyDecisionType, gpointer);
gboolean download_handle(WebKitDownload *, gchar *, gpointer);
void download_handle_start(WebKitWebView *, WebKitDownload *, gpointer);
void downloadmanager_cancel(GtkToolButton *, gpointer);
gboolean downloadmanager_delete(GtkWidget *, gpointer);
void downloadmanager_setup(void);
gchar *ensure_uri_scheme(const gchar *);
void grab_environment_configuration(void);
void grab_feeds_finished(GObject *, GAsyncResult *, gpointer);
void hover_web_view(WebKitWebView *, WebKitHitTestResult *, guint, gpointer);
void icon_location(GtkEntry *, GtkEntryIconPosition, GdkEvent *, gpointer);
void init_default_web_context(void);
gboolean key_common(GtkWidget *, GdkEvent *, gpointer);
gboolean key_downloadmanager(GtkWidget *, GdkEvent *, gpointer);
gboolean key_location(GtkWidget *, GdkEvent *, gpointer);
gboolean key_tablabel(GtkWidget *, GdkEvent *, gpointer);
gboolean key_web_view(GtkWidget *, GdkEvent *, gpointer);
void mainwindow_setup(void);
void mainwindow_title(gint);
void notebook_switch_page(GtkNotebook *, GtkWidget *, guint, gpointer);
gboolean quit_if_nothing_active(void);
gboolean remote_msg(GIOChannel *, GIOCondition, gpointer);
void run_user_scripts(WebKitWebView *);
void search(gpointer, gint);
void show_web_view(WebKitWebView *, gpointer);
void trust_user_certs(WebKitWebContext *);


struct Client
{
    gchar *external_handler_uri;
    gchar *hover_uri;
    gchar *feed_html;
    GtkWidget *location;
    GtkWidget *tabicon;
    GtkWidget *tablabel;
    GtkWidget *vbox;
    GtkWidget *web_view;
    gboolean focus_new_tab;
};

struct MainWindow
{
    GtkWidget *win;
    GtkWidget *notebook;
} mw;

struct DownloadManager
{
    GtkWidget *scroll;
    GtkWidget *toolbar;
    GtkWidget *win;
} dm;


const gchar *accepted_language[2] = { NULL, NULL };
gint clients = 0, downloads = 0;
gboolean cooperative_alone = TRUE;
gboolean cooperative_instances = TRUE;
int cooperative_pipe_fp = 0;
gchar *download_dir = "/var/tmp";
gboolean enable_console_to_stdout = FALSE;
gchar *fifo_suffix = "main";
gdouble global_zoom = 1.0;
gchar *history_file = NULL;
gchar *home_uri = "about:blank";
gchar *search_text = NULL;
GtkPositionType tab_pos = GTK_POS_TOP;
gint tab_width_chars = 20;
gchar *user_agent = NULL;


void
client_destroy(GtkWidget *widget, gpointer data)
{
    struct Client *c = (struct Client *)data;
    gint idx;

    g_signal_handlers_disconnect_by_func(G_OBJECT(c->web_view),
                                         changed_load_progress, c);

    idx = gtk_notebook_page_num(GTK_NOTEBOOK(mw.notebook), c->vbox);
    if (idx == -1)
        fprintf(stderr, __NAME__": Tab index was -1, bamboozled\n");
    else
        gtk_notebook_remove_page(GTK_NOTEBOOK(mw.notebook), idx);

    free(c);
    clients--;

    quit_if_nothing_active();
}

WebKitWebView *
client_new(const gchar *uri, WebKitWebView *related_wv, gboolean show,
           gboolean focus_tab)
{
    struct Client *c;
    gchar *f;
    GtkWidget *evbox, *tabbox;

    if (uri != NULL && cooperative_instances && !cooperative_alone)
    {
        f = ensure_uri_scheme(uri);
        write(cooperative_pipe_fp, f, strlen(f));
        write(cooperative_pipe_fp, "\n", 1);
        g_free(f);
        return NULL;
    }

    c = calloc(1, sizeof(struct Client));
    if (!c)
    {
        fprintf(stderr, __NAME__": fatal: calloc failed\n");
        exit(EXIT_FAILURE);
    }

    c->focus_new_tab = focus_tab;

    if (related_wv == NULL)
        c->web_view = webkit_web_view_new();
    else
        c->web_view = webkit_web_view_new_with_related_view(related_wv);

    webkit_web_view_set_zoom_level(WEBKIT_WEB_VIEW(c->web_view), global_zoom);
    g_signal_connect(G_OBJECT(c->web_view), "notify::favicon",
                     G_CALLBACK(changed_favicon), c);
    g_signal_connect(G_OBJECT(c->web_view), "notify::title",
                     G_CALLBACK(changed_title), c);
    g_signal_connect(G_OBJECT(c->web_view), "notify::uri",
                     G_CALLBACK(changed_uri), c);
    g_signal_connect(G_OBJECT(c->web_view), "notify::estimated-load-progress",
                     G_CALLBACK(changed_load_progress), c);
    g_signal_connect(G_OBJECT(c->web_view), "create",
                     G_CALLBACK(client_new_request), NULL);
    g_signal_connect(G_OBJECT(c->web_view), "close",
                     G_CALLBACK(client_destroy), c);
    g_signal_connect(G_OBJECT(c->web_view), "decide-policy",
                     G_CALLBACK(decide_policy), NULL);
    g_signal_connect(G_OBJECT(c->web_view), "key-press-event",
                     G_CALLBACK(key_web_view), c);
    g_signal_connect(G_OBJECT(c->web_view), "button-release-event",
                     G_CALLBACK(key_web_view), c);
    g_signal_connect(G_OBJECT(c->web_view), "scroll-event",
                     G_CALLBACK(key_web_view), c);
    g_signal_connect(G_OBJECT(c->web_view), "mouse-target-changed",
                     G_CALLBACK(hover_web_view), c);
    g_signal_connect(G_OBJECT(c->web_view), "web-process-crashed",
                     G_CALLBACK(crashed_web_view), c);

    if (user_agent != NULL)
        g_object_set(G_OBJECT(webkit_web_view_get_settings(WEBKIT_WEB_VIEW(c->web_view))),
                     "user-agent", user_agent, NULL);

    if (enable_console_to_stdout)
        webkit_settings_set_enable_write_console_messages_to_stdout(webkit_web_view_get_settings(WEBKIT_WEB_VIEW(c->web_view)), TRUE);

    webkit_settings_set_enable_developer_extras(webkit_web_view_get_settings(WEBKIT_WEB_VIEW(c->web_view)), TRUE);

    c->location = gtk_entry_new();
    g_signal_connect(G_OBJECT(c->location), "key-press-event",
                     G_CALLBACK(key_location), c);
    g_signal_connect(G_OBJECT(c->location), "icon-release",
                     G_CALLBACK(icon_location), c);
    /* XXX This is a workaround. Setting this to NULL (which is done in
     * grab_feeds_finished() if no feed has been detected) adds a little
     * padding left of the text. Not sure why. The point of this call
     * right here is to have that padding right from the start. This
     * avoids a graphical artifact. */
    gtk_entry_set_icon_from_icon_name(GTK_ENTRY(c->location),
                                      GTK_ENTRY_ICON_PRIMARY,
                                      NULL);

    c->vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(c->vbox), c->location, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(c->vbox), c->web_view, TRUE, TRUE, 0);
    gtk_container_set_focus_child(GTK_CONTAINER(c->vbox), c->web_view);

    c->tabicon = gtk_image_new_from_icon_name("text-html", GTK_ICON_SIZE_SMALL_TOOLBAR);

    c->tablabel = gtk_label_new(__NAME__);
    gtk_label_set_ellipsize(GTK_LABEL(c->tablabel), PANGO_ELLIPSIZE_END);
    gtk_label_set_width_chars(GTK_LABEL(c->tablabel), tab_width_chars);
    gtk_widget_set_has_tooltip(c->tablabel, TRUE);

    /* XXX I don't own a HiDPI screen, so I don't know if scale_factor
     * does the right thing. */
    tabbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,
                         5 * gtk_widget_get_scale_factor(mw.win));
    gtk_box_pack_start(GTK_BOX(tabbox), c->tabicon, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(tabbox), c->tablabel, TRUE, TRUE, 0);

    evbox = gtk_event_box_new();
    gtk_container_add(GTK_CONTAINER(evbox), tabbox);
    g_signal_connect(G_OBJECT(evbox), "button-release-event",
                     G_CALLBACK(key_tablabel), c);

    gtk_widget_add_events(evbox, GDK_SCROLL_MASK);
    g_signal_connect(G_OBJECT(evbox), "scroll-event",
                     G_CALLBACK(key_tablabel), c);

    /* For easy access, store a reference to our label. */
    g_object_set_data(G_OBJECT(evbox), "lariza-tab-label", c->tablabel);

    /* This only shows the event box and the label inside, nothing else.
     * Needed because the evbox/label is "internal" to the notebook and
     * not part of the normal "widget tree" (IIUC). */
    gtk_widget_show_all(evbox);

    gtk_notebook_insert_page(GTK_NOTEBOOK(mw.notebook), c->vbox, evbox,
                             gtk_notebook_get_current_page(GTK_NOTEBOOK(mw.notebook)) + 1);
    gtk_notebook_set_tab_reorderable(GTK_NOTEBOOK(mw.notebook), c->vbox, TRUE);

    if (show)
        show_web_view(NULL, c);
    else
        g_signal_connect(G_OBJECT(c->web_view), "ready-to-show",
                         G_CALLBACK(show_web_view), c);

    if (uri != NULL)
    {
        f = ensure_uri_scheme(uri);
        webkit_web_view_load_uri(WEBKIT_WEB_VIEW(c->web_view), f);
        g_free(f);
    }

    clients++;

    return WEBKIT_WEB_VIEW(c->web_view);
}

WebKitWebView *
client_new_request(WebKitWebView *web_view,
                   WebKitNavigationAction *navigation_action, gpointer data)
{
    return client_new(NULL, web_view, FALSE, FALSE);
}

void
cooperation_setup(void)
{
    GIOChannel *towatch;
    gchar *fifofilename, *fifopath;

    fifofilename = g_strdup_printf("%s-%s", __NAME__".fifo", fifo_suffix);
    fifopath = g_build_filename(g_get_user_runtime_dir(), fifofilename, NULL);
    g_free(fifofilename);

    if (!g_file_test(fifopath, G_FILE_TEST_EXISTS))
        mkfifo(fifopath, 0600);

    cooperative_pipe_fp = open(fifopath, O_WRONLY | O_NONBLOCK);
    if (!cooperative_pipe_fp)
    {
        fprintf(stderr, __NAME__": Can't open FIFO at all.\n");
    }
    else
    {
        if (write(cooperative_pipe_fp, "", 0) == -1)
        {
            /* Could not do an empty write to the FIFO which means there's
             * no one listening. */
            close(cooperative_pipe_fp);
            towatch = g_io_channel_new_file(fifopath, "r+", NULL);
            g_io_add_watch(towatch, G_IO_IN, (GIOFunc)remote_msg, NULL);
        }
        else
            cooperative_alone = FALSE;
    }

    g_free(fifopath);
}

void
changed_download_progress(GObject *obj, GParamSpec *pspec, gpointer data)
{
    WebKitDownload *download = WEBKIT_DOWNLOAD(obj);
    WebKitURIResponse *resp;
    GtkToolItem *tb = GTK_TOOL_ITEM(data);
    gdouble p, size_mb;
    const gchar *uri;
    gchar *t, *filename, *base;

    p = webkit_download_get_estimated_progress(download);
    p = p > 1 ? 1 : p;
    p = p < 0 ? 0 : p;
    p *= 100;
    resp = webkit_download_get_response(download);
    size_mb = webkit_uri_response_get_content_length(resp) / 1e6;

    uri = webkit_download_get_destination(download);
    filename = g_filename_from_uri(uri, NULL, NULL);
    if (filename == NULL)
    {
        /* This really should not happen because WebKit uses that URI to
         * write to a file... */
        fprintf(stderr, __NAME__": Could not construct file name from URI!\n");
        t = g_strdup_printf("%s (%.0f%% of %.1f MB)",
                            webkit_uri_response_get_uri(resp), p, size_mb);
    }
    else
    {
        base = g_path_get_basename(filename);
        t = g_strdup_printf("%s (%.0f%% of %.1f MB)", base, p, size_mb);
        g_free(filename);
        g_free(base);
    }
    gtk_tool_button_set_label(GTK_TOOL_BUTTON(tb), t);
    g_free(t);
}

void
changed_load_progress(GObject *obj, GParamSpec *pspec, gpointer data)
{
    struct Client *c = (struct Client *)data;
    gdouble p;
    gchar *grab_feeds =
        "a = document.querySelectorAll('"
        "    html > head > link[rel=\"alternate\"][href][type=\"application/atom+xml\"],"
        "    html > head > link[rel=\"alternate\"][href][type=\"application/rss+xml\"]"
        "');"
        "if (a.length == 0)"
        "    null;"
        "else"
        "{"
        "    out = '';"
        "    for (i = 0; i < a.length; i++)"
        "    {"
        "        url = encodeURIComponent(a[i].href);"
        "        if ('title' in a[i] && a[i].title != '')"
        "            title = encodeURIComponent(a[i].title);"
        "        else"
        "            title = url;"
        "        out += '<li><a href=\"' + url + '\">' + title + '</a></li>';"
        "    }"
        "    out;"
        "}";

    p = webkit_web_view_get_estimated_load_progress(WEBKIT_WEB_VIEW(c->web_view));
    if (p == 1)
    {
        p = 0;

        /* The page has loaded fully. We now run the short JavaScript
         * snippet above that operates on the DOM. It tries to grab all
         * occurences of <link rel="alternate" ...>, i.e. RSS/Atom feed
         * references. */
        webkit_web_view_run_javascript(WEBKIT_WEB_VIEW(c->web_view),
                                       grab_feeds, NULL,
                                       grab_feeds_finished, c);

        run_user_scripts(WEBKIT_WEB_VIEW(c->web_view));
    }
    gtk_entry_set_progress_fraction(GTK_ENTRY(c->location), p);
}

void
changed_favicon(GObject *obj, GParamSpec *pspec, gpointer data)
{
    struct Client *c = (struct Client *)data;
    cairo_surface_t *f;
    int w, h, w_should, h_should;
    GdkPixbuf *pb, *pb_scaled;

    f = webkit_web_view_get_favicon(WEBKIT_WEB_VIEW(c->web_view));
    if (f == NULL)
    {
        gtk_image_set_from_icon_name(GTK_IMAGE(c->tabicon), "text-html",
                                     GTK_ICON_SIZE_SMALL_TOOLBAR);
    }
    else
    {
        w = cairo_image_surface_get_width(f);
        h = cairo_image_surface_get_height(f);
        pb = gdk_pixbuf_get_from_surface(f, 0, 0, w, h);
        if (pb != NULL)
        {
            w_should = 16 * gtk_widget_get_scale_factor(c->tabicon);
            h_should = 16 * gtk_widget_get_scale_factor(c->tabicon);
            pb_scaled = gdk_pixbuf_scale_simple(pb, w_should, h_should,
                                                GDK_INTERP_BILINEAR);
            gtk_image_set_from_pixbuf(GTK_IMAGE(c->tabicon), pb_scaled);

            g_object_unref(pb_scaled);
            g_object_unref(pb);
        }
    }
}

void
changed_title(GObject *obj, GParamSpec *pspec, gpointer data)
{
    const gchar *t, *u;
    struct Client *c = (struct Client *)data;

    u = webkit_web_view_get_uri(WEBKIT_WEB_VIEW(c->web_view));
    t = webkit_web_view_get_title(WEBKIT_WEB_VIEW(c->web_view));

    u = u == NULL ? __NAME__ : u;
    u = u[0] == 0 ? __NAME__ : u;

    t = t == NULL ? u : t;
    t = t[0] == 0 ? u : t;

    gtk_label_set_text(GTK_LABEL(c->tablabel), t);
    gtk_widget_set_tooltip_text(c->tablabel, t);
    mainwindow_title(gtk_notebook_get_current_page(GTK_NOTEBOOK(mw.notebook)));
}

void
changed_uri(GObject *obj, GParamSpec *pspec, gpointer data)
{
    const gchar *t;
    struct Client *c = (struct Client *)data;
    FILE *fp;

    t = webkit_web_view_get_uri(WEBKIT_WEB_VIEW(c->web_view));

    /* When a web process crashes, we get a "notify::uri" signal, but we
     * can no longer read a meaningful URI. It's just an empty string
     * now. Not updating the location bar in this scenario is important,
     * because we would override the "WEB PROCESS CRASHED" message. */
    if (t != NULL && strlen(t) > 0)
    {
        gtk_entry_set_text(GTK_ENTRY(c->location), t);

        if (history_file != NULL)
        {
            fp = fopen(history_file, "a");
            if (fp != NULL)
            {
                fprintf(fp, "%s\n", t);
                fclose(fp);
            }
            else
                perror(__NAME__": Error opening history file");
        }
    }
}

gboolean
crashed_web_view(WebKitWebView *web_view, gpointer data)
{
    gchar *t;
    struct Client *c = (struct Client *)data;

    t = g_strdup_printf("WEB PROCESS CRASHED: %s",
                        webkit_web_view_get_uri(WEBKIT_WEB_VIEW(web_view)));
    gtk_entry_set_text(GTK_ENTRY(c->location), t);
    g_free(t);

    return TRUE;
}

gboolean
decide_policy(WebKitWebView *web_view, WebKitPolicyDecision *decision,
              WebKitPolicyDecisionType type, gpointer data)
{
    WebKitResponsePolicyDecision *r;

    switch (type)
    {
        case WEBKIT_POLICY_DECISION_TYPE_RESPONSE:
            r = WEBKIT_RESPONSE_POLICY_DECISION(decision);
            if (!webkit_response_policy_decision_is_mime_type_supported(r))
                webkit_policy_decision_download(decision);
            else
                webkit_policy_decision_use(decision);
            break;
        default:
            /* Use whatever default there is. */
            return FALSE;
    }
    return TRUE;
}

void
download_handle_finished(WebKitDownload *download, gpointer data)
{
    downloads--;
}

void
download_handle_start(WebKitWebView *web_view, WebKitDownload *download,
                      gpointer data)
{
    g_signal_connect(G_OBJECT(download), "decide-destination",
                     G_CALLBACK(download_handle), data);
}

gboolean
download_handle(WebKitDownload *download, gchar *suggested_filename, gpointer data)
{
    gchar *sug_clean, *path, *path2 = NULL, *uri;
    GtkToolItem *tb;
    int suffix = 1;
    size_t i;

    sug_clean = g_strdup(suggested_filename);
    for (i = 0; i < strlen(sug_clean); i++)
        if (sug_clean[i] == G_DIR_SEPARATOR)
            sug_clean[i] = '_';

    path = g_build_filename(download_dir, sug_clean, NULL);
    path2 = g_strdup(path);
    while (g_file_test(path2, G_FILE_TEST_EXISTS) && suffix < 1000)
    {
        g_free(path2);

        path2 = g_strdup_printf("%s.%d", path, suffix);
        suffix++;
    }

    if (suffix == 1000)
    {
        fprintf(stderr, __NAME__": Suffix reached limit for download.\n");
        webkit_download_cancel(download);
    }
    else
    {
        uri = g_filename_to_uri(path2, NULL, NULL);
        webkit_download_set_destination(download, uri);
        g_free(uri);

        tb = gtk_tool_button_new(NULL, NULL);
        gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(tb), "gtk-delete");
        gtk_tool_button_set_label(GTK_TOOL_BUTTON(tb), sug_clean);
        gtk_toolbar_insert(GTK_TOOLBAR(dm.toolbar), tb, 0);
        gtk_widget_show_all(dm.win);

        g_signal_connect(G_OBJECT(download), "notify::estimated-progress",
                         G_CALLBACK(changed_download_progress), tb);

        downloads++;
        g_signal_connect(G_OBJECT(download), "finished",
                         G_CALLBACK(download_handle_finished), NULL);

        g_object_ref(download);
        g_signal_connect(G_OBJECT(tb), "clicked",
                         G_CALLBACK(downloadmanager_cancel), download);
    }

    g_free(sug_clean);
    g_free(path);
    g_free(path2);

    /* Propagate -- to whom it may concern. */
    return FALSE;
}

void
downloadmanager_cancel(GtkToolButton *tb, gpointer data)
{
    WebKitDownload *download = WEBKIT_DOWNLOAD(data);

    webkit_download_cancel(download);
    g_object_unref(download);

    gtk_widget_destroy(GTK_WIDGET(tb));
}

gboolean
downloadmanager_delete(GtkWidget *obj, gpointer data)
{
    if (!quit_if_nothing_active())
        gtk_widget_hide(dm.win);

    return TRUE;
}

void
downloadmanager_setup(void)
{
    dm.win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_type_hint(GTK_WINDOW(dm.win), GDK_WINDOW_TYPE_HINT_DIALOG);
    gtk_window_set_default_size(GTK_WINDOW(dm.win), 500, 250);
    gtk_window_set_title(GTK_WINDOW(dm.win), __NAME__" - Download Manager");
    g_signal_connect(G_OBJECT(dm.win), "delete-event",
                     G_CALLBACK(downloadmanager_delete), NULL);
    g_signal_connect(G_OBJECT(dm.win), "key-press-event",
                     G_CALLBACK(key_downloadmanager), NULL);

    dm.toolbar = gtk_toolbar_new();
    gtk_orientable_set_orientation(GTK_ORIENTABLE(dm.toolbar),
                                   GTK_ORIENTATION_VERTICAL);
    gtk_toolbar_set_style(GTK_TOOLBAR(dm.toolbar), GTK_TOOLBAR_BOTH_HORIZ);
    gtk_toolbar_set_show_arrow(GTK_TOOLBAR(dm.toolbar), FALSE);

    dm.scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(dm.scroll),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(dm.scroll), dm.toolbar);

    gtk_container_add(GTK_CONTAINER(dm.win), dm.scroll);
}

gchar *
ensure_uri_scheme(const gchar *t)
{
    gchar *f, *fabs;

    f = g_ascii_strdown(t, -1);
    if (!g_str_has_prefix(f, "http:") &&
        !g_str_has_prefix(f, "https:") &&
        !g_str_has_prefix(f, "file:") &&
        !g_str_has_prefix(f, "about:") &&
        !g_str_has_prefix(f, "data:") &&
        !g_str_has_prefix(f, "webkit:"))
    {
        g_free(f);
        fabs = realpath(t, NULL);
        if (fabs != NULL)
        {
            f = g_strdup_printf("file://%s", fabs);
            free(fabs);
        }
        else
            f = g_strdup_printf("http://%s", t);
        return f;
    }
    else
        return g_strdup(t);
}

void
grab_environment_configuration(void)
{
    const gchar *e;

    e = g_getenv(__NAME_UPPERCASE__"_ACCEPTED_LANGUAGE");
    if (e != NULL)
        accepted_language[0] = g_strdup(e);

    e = g_getenv(__NAME_UPPERCASE__"_DOWNLOAD_DIR");
    if (e != NULL)
        download_dir = g_strdup(e);

    e = g_getenv(__NAME_UPPERCASE__"_ENABLE_CONSOLE_TO_STDOUT");
    if (e != NULL)
        enable_console_to_stdout = TRUE;

    e = g_getenv(__NAME_UPPERCASE__"_FIFO_SUFFIX");
    if (e != NULL)
        fifo_suffix = g_strdup(e);

    e = g_getenv(__NAME_UPPERCASE__"_HISTORY_FILE");
    if (e != NULL)
        history_file = g_strdup(e);

    e = g_getenv(__NAME_UPPERCASE__"_HOME_URI");
    if (e != NULL)
        home_uri = g_strdup(e);

    e = g_getenv(__NAME_UPPERCASE__"_TAB_POS");
    if (e != NULL)
    {
        if (strcmp(e, "top") == 0)
            tab_pos = GTK_POS_TOP;
        if (strcmp(e, "right") == 0)
            tab_pos = GTK_POS_RIGHT;
        if (strcmp(e, "bottom") == 0)
            tab_pos = GTK_POS_BOTTOM;
        if (strcmp(e, "left") == 0)
            tab_pos = GTK_POS_LEFT;
    }

    e = g_getenv(__NAME_UPPERCASE__"_TAB_WIDTH_CHARS");
    if (e != NULL)
        tab_width_chars = atoi(e);

    e = g_getenv(__NAME_UPPERCASE__"_USER_AGENT");
    if (e != NULL)
        user_agent = g_strdup(e);

    e = g_getenv(__NAME_UPPERCASE__"_ZOOM");
    if (e != NULL)
        global_zoom = atof(e);
}

void
grab_feeds_finished(GObject *object, GAsyncResult *result, gpointer data)
{
    struct Client *c = (struct Client *)data;
    WebKitJavascriptResult *js_result;
    JSCValue *value;
    JSCException *exception;
    GError *err = NULL;
    gchar *str_value;

    g_free(c->feed_html);
    c->feed_html = NULL;

    /* This was taken almost verbatim from the example in WebKit's
     * documentation:
     *
     * https://webkitgtk.org/reference/webkit2gtk/stable/WebKitWebView.html#webkit-web-view-run-javascript-finish */

    js_result = webkit_web_view_run_javascript_finish(WEBKIT_WEB_VIEW(object),
                                                      result, &err);
    if (!js_result)
    {
        fprintf(stderr, __NAME__": Error running javascript: %s\n", err->message);
        g_error_free(err);
        return;
    }

    value = webkit_javascript_result_get_js_value(js_result);
    if (jsc_value_is_string(value))
    {
        str_value = jsc_value_to_string(value);
        exception = jsc_context_get_exception(jsc_value_get_context(value));
        if (exception != NULL)
        {
            fprintf(stderr, __NAME__": Error running javascript: %s\n",
                    jsc_exception_get_message(exception));
        }
        else
            c->feed_html = str_value;

        gtk_entry_set_icon_from_icon_name(GTK_ENTRY(c->location),
                                          GTK_ENTRY_ICON_PRIMARY,
                                          "application-rss+xml-symbolic");
        gtk_entry_set_icon_activatable(GTK_ENTRY(c->location),
                                       GTK_ENTRY_ICON_PRIMARY,
                                       TRUE);
    }
    else
    {
        gtk_entry_set_icon_from_icon_name(GTK_ENTRY(c->location),
                                          GTK_ENTRY_ICON_PRIMARY,
                                          NULL);
    }

    webkit_javascript_result_unref(js_result);
}

void
hover_web_view(WebKitWebView *web_view, WebKitHitTestResult *ht, guint modifiers,
               gpointer data)
{
    struct Client *c = (struct Client *)data;
    const char *to_show;

    g_free(c->hover_uri);

    if (webkit_hit_test_result_context_is_link(ht))
    {
        to_show = webkit_hit_test_result_get_link_uri(ht);
        c->hover_uri = g_strdup(to_show);
    }
    else
    {
        to_show = webkit_web_view_get_uri(WEBKIT_WEB_VIEW(c->web_view));
        c->hover_uri = NULL;
    }

    if (!gtk_widget_is_focus(c->location))
        gtk_entry_set_text(GTK_ENTRY(c->location), to_show);
}

void
icon_location(GtkEntry *entry, GtkEntryIconPosition icon_pos, GdkEvent *event,
              gpointer data)
{
    struct Client *c = (struct Client *)data;
    gchar *d;
    gchar *data_template =
        "data:text/html,"
        "<!DOCTYPE html>"
        "<html>"
        "    <head>"
        "        <meta charset=\"UTF-8\">"
        "        <title>Feeds</title>"
        "    </head>"
        "    <body>"
        "        <p>Feeds found on this page:</p>"
        "        <ul>"
        "        %s"
        "        </ul>"
        "    </body>"
        "</html>";

    if (c->feed_html != NULL)
    {
        /* What we're actually trying to do is show a simple HTML page
         * that lists all the feeds on the current page. The function
         * webkit_web_view_load_html() looks like the proper way to do
         * that. Sad thing is, it doesn't create a history entry, but
         * instead simply replaces the content of the current page. This
         * is not what we want.
         *
         * RFC 2397 [0] defines the data URI scheme [1]. We abuse this
         * mechanism to show my custom HTML snippet *and* create a
         * history entry.
         *
         * [0]: https://tools.ietf.org/html/rfc2397
         * [1]: https://en.wikipedia.org/wiki/Data_URI_scheme */
        d = g_strdup_printf(data_template, c->feed_html);
        webkit_web_view_load_uri(WEBKIT_WEB_VIEW(c->web_view), d);
        g_free(d);
    }
}

void
init_default_web_context(void)
{
    gchar *p;
    WebKitWebContext *wc;

    wc = webkit_web_context_get_default();

    p = g_build_filename(g_get_user_config_dir(), __NAME__, "adblock", NULL);
    webkit_web_context_set_sandbox_enabled(wc, TRUE);
    webkit_web_context_add_path_to_sandbox(wc, p, TRUE);
    g_free(p);

    webkit_web_context_set_process_model(wc,
        WEBKIT_PROCESS_MODEL_MULTIPLE_SECONDARY_PROCESSES);

    p = g_build_filename(g_get_user_config_dir(), __NAME__, "web_extensions",
                         NULL);
    webkit_web_context_set_web_extensions_directory(wc, p);
    g_free(p);

    if (accepted_language[0] != NULL)
        webkit_web_context_set_preferred_languages(wc, accepted_language);

    g_signal_connect(G_OBJECT(wc), "download-started",
                     G_CALLBACK(download_handle_start), NULL);

    trust_user_certs(wc);

    webkit_web_context_set_favicon_database_directory(wc, NULL);
}

gboolean
key_common(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    struct Client *c = (struct Client *)data;
    WebKitWebContext *wc = webkit_web_view_get_context(WEBKIT_WEB_VIEW(c->web_view));
    gchar *f;

    if (event->type == GDK_KEY_PRESS)
    {
        if (((GdkEventKey *)event)->state & GDK_CONTROL_MASK)
        {
            switch (((GdkEventKey *)event)->keyval)
            {
                case GDK_KEY_q:  /* close window (left hand) */
                    client_destroy(NULL, c);
                    return TRUE;
                case GDK_KEY_w:  /* home (left hand) */
                    f = ensure_uri_scheme(home_uri);
                    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(c->web_view), f);
                    g_free(f);
                    return TRUE;
                case GDK_KEY_e:  /* new tab (left hand) */
                    f = ensure_uri_scheme(home_uri);
                    client_new(f, NULL, TRUE, TRUE);
                    g_free(f);
                    return TRUE;
                case GDK_KEY_r:  /* reload (left hand) */
                    webkit_web_view_reload_bypass_cache(WEBKIT_WEB_VIEW(
                                                        c->web_view));
                    return TRUE;
                case GDK_KEY_d:  /* download manager (left hand) */
                    gtk_widget_show_all(dm.win);
                    return TRUE;
                case GDK_KEY_2:  /* search forward (left hand) */
                case GDK_KEY_n:  /* search forward (maybe both hands) */
                    search(c, 1);
                    return TRUE;
                case GDK_KEY_3:  /* search backward (left hand) */
                    search(c, -1);
                    return TRUE;
                case GDK_KEY_l:  /* location (BOTH hands) */
                    gtk_widget_grab_focus(c->location);
                    return TRUE;
                case GDK_KEY_k:  /* initiate search (BOTH hands) */
                    gtk_widget_grab_focus(c->location);
                    gtk_entry_set_text(GTK_ENTRY(c->location), ":/");
                    gtk_editable_set_position(GTK_EDITABLE(c->location), -1);
                    return TRUE;
                case GDK_KEY_c:  /* reload trusted certs (left hand) */
                    trust_user_certs(wc);
                    return TRUE;
                case GDK_KEY_a:  /* go one tab to the left (left hand) */
                    gtk_notebook_prev_page(GTK_NOTEBOOK(mw.notebook));
                    return TRUE;
                case GDK_KEY_s:  /* go one tab to the right (left hand) */
                    gtk_notebook_next_page(GTK_NOTEBOOK(mw.notebook));
                    return TRUE;
            }
        }
        /* navigate backward (left hand) */
        else if (((GdkEventKey *)event)->keyval == GDK_KEY_F2)
        {
            webkit_web_view_go_back(WEBKIT_WEB_VIEW(c->web_view));
            return TRUE;
        }
        /* navigate forward (left hand) */
        else if (((GdkEventKey *)event)->keyval == GDK_KEY_F3)
        {
            webkit_web_view_go_forward(WEBKIT_WEB_VIEW(c->web_view));
            return TRUE;
        }
    }

    return FALSE;
}

gboolean
key_downloadmanager(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    if (event->type == GDK_KEY_PRESS)
    {
        if (((GdkEventKey *)event)->state & GDK_MOD1_MASK)
        {
            switch (((GdkEventKey *)event)->keyval)
            {
                case GDK_KEY_d:  /* close window (left hand) */
                case GDK_KEY_q:
                    downloadmanager_delete(dm.win, NULL);
                    return TRUE;
            }
        }
    }

    return FALSE;
}

gboolean
key_location(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    struct Client *c = (struct Client *)data;
    const gchar *t;
    gchar *f;

    if (key_common(widget, event, data))
        return TRUE;

    if (event->type == GDK_KEY_PRESS)
    {
        switch (((GdkEventKey *)event)->keyval)
        {
            case GDK_KEY_KP_Enter:
            case GDK_KEY_Return:
                gtk_widget_grab_focus(c->web_view);
                t = gtk_entry_get_text(GTK_ENTRY(c->location));
                if (t != NULL && t[0] == ':' && t[1] == '/')
                {
                    if (search_text != NULL)
                        g_free(search_text);
                    search_text = g_strdup(t + 2);
                    search(c, 0);
                }
                else
                {
                    f = ensure_uri_scheme(t);
                    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(c->web_view), f);
                    g_free(f);
                }
                return TRUE;
            case GDK_KEY_Escape:
                t = webkit_web_view_get_uri(WEBKIT_WEB_VIEW(c->web_view));
                gtk_entry_set_text(GTK_ENTRY(c->location),
                                   (t == NULL ? __NAME__ : t));
                return TRUE;
        }
    }

    return FALSE;
}

gboolean
key_tablabel(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    GdkScrollDirection direction;

    if (event->type == GDK_BUTTON_RELEASE)
    {
        switch (((GdkEventButton *)event)->button)
        {
            case 2:
                client_destroy(NULL, data);
                return TRUE;
        }
    }
    else if (event->type == GDK_SCROLL)
    {
        gdk_event_get_scroll_direction(event, &direction);
        switch (direction)
        {
            case GDK_SCROLL_UP:
                gtk_notebook_prev_page(GTK_NOTEBOOK(mw.notebook));
                break;
            case GDK_SCROLL_DOWN:
                gtk_notebook_next_page(GTK_NOTEBOOK(mw.notebook));
                break;
            default:
                break;
        }
        return TRUE;
    }
    return FALSE;
}

gboolean
key_web_view(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    struct Client *c = (struct Client *)data;
    gdouble dx, dy;
    gfloat z;

    if (key_common(widget, event, data))
        return TRUE;

    if (event->type == GDK_KEY_PRESS)
    {
        if (((GdkEventKey *)event)->keyval == GDK_KEY_Escape)
        {
            webkit_web_view_stop_loading(WEBKIT_WEB_VIEW(c->web_view));
            gtk_entry_set_progress_fraction(GTK_ENTRY(c->location), 0);
        }
    }
    else if (event->type == GDK_BUTTON_RELEASE)
    {
        switch (((GdkEventButton *)event)->button)
        {
            case 2:
                if (c->hover_uri != NULL)
                {
                    client_new(c->hover_uri, NULL, TRUE, FALSE);
                    return TRUE;
                }
                break;
            case 8:
                webkit_web_view_go_back(WEBKIT_WEB_VIEW(c->web_view));
                return TRUE;
            case 9:
                webkit_web_view_go_forward(WEBKIT_WEB_VIEW(c->web_view));
                return TRUE;
        }
    }
    else if (event->type == GDK_SCROLL)
    {
        if (((GdkEventScroll *)event)->state & GDK_MOD1_MASK ||
            ((GdkEventScroll *)event)->state & GDK_CONTROL_MASK)
        {
            gdk_event_get_scroll_deltas(event, &dx, &dy);
            z = webkit_web_view_get_zoom_level(WEBKIT_WEB_VIEW(c->web_view));
            z += -dy * 0.1;
            z = dx != 0 ? global_zoom : z;
            webkit_web_view_set_zoom_level(WEBKIT_WEB_VIEW(c->web_view), z);
            return TRUE;
        }
    }

    return FALSE;
}

void
mainwindow_setup(void)
{
    mw.win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(mw.win), 800, 600);
    g_signal_connect(G_OBJECT(mw.win), "destroy", gtk_main_quit, NULL);
    gtk_window_set_title(GTK_WINDOW(mw.win), __NAME__);

    mw.notebook = gtk_notebook_new();
    gtk_notebook_set_scrollable(GTK_NOTEBOOK(mw.notebook), TRUE);
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(mw.notebook), tab_pos);
    gtk_container_add(GTK_CONTAINER(mw.win), mw.notebook);
    g_signal_connect(G_OBJECT(mw.notebook), "switch-page",
                     G_CALLBACK(notebook_switch_page), NULL);
}

void
mainwindow_title(gint idx)
{
    GtkWidget *child, *widg, *tablabel;
    const gchar *text;

    child = gtk_notebook_get_nth_page(GTK_NOTEBOOK(mw.notebook), idx);
    if (child == NULL)
        return;

    widg = gtk_notebook_get_tab_label(GTK_NOTEBOOK(mw.notebook), child);
    tablabel = (GtkWidget *)g_object_get_data(G_OBJECT(widg), "lariza-tab-label");
    text = gtk_label_get_text(GTK_LABEL(tablabel));
    gtk_window_set_title(GTK_WINDOW(mw.win), text);
}

void
notebook_switch_page(GtkNotebook *nb, GtkWidget *p, guint idx, gpointer data)
{
    mainwindow_title(idx);
}

gboolean
quit_if_nothing_active(void)
{
    if (clients == 0)
    {
        if (downloads == 0)
        {
            gtk_main_quit();
            return TRUE;
        }
        else
            gtk_widget_show_all(dm.win);
    }

    return FALSE;
}

gboolean
remote_msg(GIOChannel *channel, GIOCondition condition, gpointer data)
{
    gchar *uri = NULL;

    g_io_channel_read_line(channel, &uri, NULL, NULL, NULL);
    if (uri)
    {
        g_strstrip(uri);
        client_new(uri, NULL, TRUE, TRUE);
        g_free(uri);
    }
    return TRUE;
}

void
run_user_scripts(WebKitWebView *web_view)
{
    gchar *base = NULL, *path = NULL, *contents = NULL;
    const gchar *entry = NULL;
    GDir *scriptdir = NULL;

    base = g_build_filename(g_get_user_config_dir(), __NAME__, "user-scripts", NULL);
    scriptdir = g_dir_open(base, 0, NULL);
    if (scriptdir != NULL)
    {
        while ((entry = g_dir_read_name(scriptdir)) != NULL)
        {
            path = g_build_filename(base, entry, NULL);
            if (g_str_has_suffix(path, ".js"))
            {
                if (g_file_get_contents(path, &contents, NULL, NULL))
                {
                    webkit_web_view_run_javascript(web_view, contents, NULL, NULL, NULL);
                    g_free(contents);
                }
            }
            g_free(path);
        }
        g_dir_close(scriptdir);
    }

    g_free(base);
}

void
search(gpointer data, gint direction)
{
    struct Client *c = (struct Client *)data;
    WebKitWebView *web_view = WEBKIT_WEB_VIEW(c->web_view);
    WebKitFindController *fc = webkit_web_view_get_find_controller(web_view);

    if (search_text == NULL)
        return;

    switch (direction)
    {
        case 0:
            webkit_find_controller_search(fc, search_text,
                                          WEBKIT_FIND_OPTIONS_CASE_INSENSITIVE |
                                          WEBKIT_FIND_OPTIONS_WRAP_AROUND,
                                          G_MAXUINT);
            break;
        case 1:
            webkit_find_controller_search_next(fc);
            break;
        case -1:
            webkit_find_controller_search_previous(fc);
            break;
    }
}

void
show_web_view(WebKitWebView *web_view, gpointer data)
{
    struct Client *c = (struct Client *)data;
    gint idx;

    (void)web_view;

    gtk_widget_show_all(mw.win);

    if (c->focus_new_tab)
    {
        idx = gtk_notebook_page_num(GTK_NOTEBOOK(mw.notebook), c->vbox);
        if (idx != -1)
            gtk_notebook_set_current_page(GTK_NOTEBOOK(mw.notebook), idx);

        gtk_widget_grab_focus(c->web_view);
    }
}

void
trust_user_certs(WebKitWebContext *wc)
{
    GTlsCertificate *cert;
    const gchar *basedir, *file, *absfile;
    GDir *dir;

    basedir = g_build_filename(g_get_user_config_dir(), __NAME__, "certs", NULL);
    dir = g_dir_open(basedir, 0, NULL);
    if (dir != NULL)
    {
        file = g_dir_read_name(dir);
        while (file != NULL)
        {
            absfile = g_build_filename(g_get_user_config_dir(), __NAME__, "certs",
                                       file, NULL);
            cert = g_tls_certificate_new_from_file(absfile, NULL);
            if (cert == NULL)
                fprintf(stderr, __NAME__": Could not load trusted cert '%s'\n", file);
            else
                webkit_web_context_allow_tls_certificate_for_host(wc, cert, file);
            file = g_dir_read_name(dir);
        }
        g_dir_close(dir);
    }
}


int
main(int argc, char **argv)
{
    int opt, i;

    gtk_init(&argc, &argv);
    grab_environment_configuration();

    while ((opt = getopt(argc, argv, "C")) != -1)
    {
        switch (opt)
        {
            case 'C':
                cooperative_instances = FALSE;
                break;
            default:
                fprintf(stderr, "Usage: "__NAME__" [OPTION]... [URI]...\n");
                exit(EXIT_FAILURE);
        }
    }

    if (cooperative_instances)
        cooperation_setup();

    if (!cooperative_instances || cooperative_alone)
        init_default_web_context();

    downloadmanager_setup();
    mainwindow_setup();

    if (optind >= argc)
        client_new(home_uri, NULL, TRUE, TRUE);
    else
    {
        for (i = optind; i < argc; i++)
            client_new(argv[i], NULL, TRUE, TRUE);
    }

    if (!cooperative_instances || cooperative_alone)
        gtk_main();

    exit(EXIT_SUCCESS);
}
