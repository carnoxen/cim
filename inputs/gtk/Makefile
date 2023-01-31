include ../../config.mk

GTK3_BINARY_VERSION=`pkg-config --variable=gtk_binary_version gtk+-3.0`
GTK2_BINARY_VERSION=`pkg-config --variable=gtk_binary_version gtk+-2.0`

IM_CIM_GTK3_DEPS_CFLAGS = `pkg-config --cflags gtk+-3.0`
IM_CIM_GTK3_DEPS_LIBS   = `pkg-config --libs   gtk+-3.0` \
	$(top_srcdir)/libcim/libcim.a

IM_CIM_GTK2_DEPS_CFLAGS = `pkg-config --cflags gtk+-2.0`
IM_CIM_GTK2_DEPS_LIBS   = `pkg-config --libs   gtk+-2.0` \
	$(top_srcdir)/libcim/libcim.a

CFLAGS = \
	$(EXTRA_CFLAGS) \
	-I$(top_srcdir)/libcim \
	-DGETTEXT_PACKAGE=\"$(GETTEXT_PACKAGE)\" \
	-DCIM_LOCALE_DIR=\"$(CIM_LOCALE_DIR)\"

SOURCES = im-cim-gtk.c

# for gtk3
gtk3_im_moduledir = $(GTK3_LIBDIR)/gtk-3.0/$(GTK3_BINARY_VERSION)/immodules
IM_CIM_GTK3 = im-cim-gtk3.so

im_cim_gtk3_CFLAGS  = $(IM_CIM_GTK3_DEPS_CFLAGS) $(CFLAGS) \
                      -Wno-error=deprecated-declarations
im_cim_gtk3_LDFLAGS = -Wl,--as-needed $(IM_CIM_GTK3_DEPS_LIBS)

# for gtk2
gtk2_im_moduledir = $(GTK2_LIBDIR)/gtk-2.0/$(GTK2_BINARY_VERSION)/immodules
IM_CIM_GTK2 = im-cim-gtk2.so

im_cim_gtk2_CFLAGS  = $(IM_CIM_GTK2_DEPS_CFLAGS) $(CFLAGS) \
                      -Wno-error=deprecated-declarations
im_cim_gtk2_LDFLAGS = -Wl,--as-needed $(IM_CIM_GTK2_DEPS_LIBS)

all: $(IM_CIM_GTK3) $(IM_CIM_GTK2)

$(IM_CIM_GTK2): im-cim-gtk2.o Makefile
	$(CC) -shared -fPIC $(EXTRA_LDFLAGS) im-cim-gtk2.o $(im_cim_gtk2_LDFLAGS) \
	  -Wl,-soname -Wl,$(IM_CIM_GTK2) \
	  -o $(IM_CIM_GTK2)

$(IM_CIM_GTK3): im-cim-gtk3.o Makefile
	$(CC) -shared -fPIC $(EXTRA_LDFLAGS) im-cim-gtk3.o $(im_cim_gtk3_LDFLAGS) \
	  -Wl,-soname -Wl,$(IM_CIM_GTK3) \
	  -o $(IM_CIM_GTK3)

im-cim-gtk2.o: $(SOURCES) Makefile
	$(CC) -fPIC $(im_cim_gtk2_CFLAGS) -c $(SOURCES) -o im-cim-gtk2.o

im-cim-gtk3.o: $(SOURCES) Makefile
	$(CC) -fPIC $(im_cim_gtk3_CFLAGS) -c $(SOURCES) -o im-cim-gtk3.o

install:
	mkdir -p $(DESTDIR)$(gtk3_im_moduledir)
	install -m 755 $(IM_CIM_GTK3) $(DESTDIR)$(gtk3_im_moduledir)
	mkdir -p $(DESTDIR)$(gtk2_im_moduledir)
	install -m 755 $(IM_CIM_GTK2) $(DESTDIR)$(gtk2_im_moduledir)

uninstall:
	rm -f $(DESTDIR)$(gtk3_im_moduledir)/$(IM_CIM_GTK3)
	rm -f $(DESTDIR)$(gtk2_im_moduledir)/$(IM_CIM_GTK2)
	-rmdir -p $(DESTDIR)$(gtk3_im_moduledir)
	-rmdir -p $(DESTDIR)$(gtk2_im_moduledir)

clean:
	rm -f $(IM_CIM_GTK3) $(IM_CIM_GTK2) im-cim-gtk3.o  im-cim-gtk2.o
