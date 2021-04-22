CFLAGS += -Wall -Wextra -Wno-unused-parameter -O3
__NAME__ = lariza
__NAME_UPPERCASE__ = LARIZA

INSTALL = install
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA = $(INSTALL) -m 644

prefix = /usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin
libdir = $(exec_prefix)/lib
datarootdir = $(prefix)/share
mandir = $(datarootdir)/man
man1dir = $(mandir)/man1
datadir = $(prefix)/share


.PHONY: all clean install installdirs

all: $(__NAME__) we_adblock.so

$(__NAME__): browser.c
	$(CC) $(CFLAGS) $(LDFLAGS) \
		-D__NAME__=\"$(__NAME__)\" \
		-D__NAME_UPPERCASE__=\"$(__NAME_UPPERCASE__)\" \
		-o $@ $< \
		`pkg-config --cflags --libs gtk+-3.0 glib-2.0 webkit2gtk-4.0`

we_adblock.so: we_adblock.c
	$(CC) $(CFLAGS) $(LDFLAGS) \
		-D__NAME__=\"$(__NAME__)\" \
		-D__NAME_UPPERCASE__=\"$(__NAME_UPPERCASE__)\" \
		-shared -o $@ -fPIC $< \
		`pkg-config --cflags --libs glib-2.0 webkit2gtk-4.0`

install: all installdirs
	$(INSTALL_PROGRAM) $(__NAME__) $(DESTDIR)$(bindir)/$(__NAME__)
	$(INSTALL_DATA) man1/$(__NAME__).1 $(DESTDIR)$(man1dir)/$(__NAME__).1
	$(INSTALL_DATA) man1/$(__NAME__).usage.1 \
		$(DESTDIR)$(man1dir)/$(__NAME__).usage.1
	$(INSTALL_PROGRAM) we_adblock.so \
		$(DESTDIR)$(libdir)/$(__NAME__)/web_extensions/we_adblock.so
	$(INSTALL_DATA) user-scripts/hints.js \
		$(DESTDIR)$(datadir)/$(__NAME__)/user-scripts/hints.js

installdirs:
	mkdir -p $(DESTDIR)$(bindir) $(DESTDIR)$(man1dir) \
		$(DESTDIR)$(libdir)/$(__NAME__)/web_extensions \
		$(DESTDIR)$(datadir)/$(__NAME__)/user-scripts

clean:
	rm -f $(__NAME__) we_adblock.so
