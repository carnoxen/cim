include ../../config.mk

GTK4_BINARY_VERSION=`pkg-config --variable=gtk_binary_version gtk4`

IM_CIM_GTK4_DEPS_CFLAGS = `pkg-config --cflags gtk4 x11`
IM_CIM_GTK4_DEPS_LIBS   = `pkg-config --libs   gtk4 x11` \
	$(top_srcdir)/libcim/libcim.a

CFLAGS = $(EXTRA_CFLAGS) -I$(top_srcdir)/libcim

SOURCES = libim-cim-gtk4.c

gtk4_im_moduledir = $(GTK4_LIBDIR)/gtk-4.0/$(GTK4_BINARY_VERSION)/immodules
IM_CIM_GTK4 = libim-cim-gtk4.so

im_cim_gtk4_CFLAGS  = $(IM_CIM_GTK4_DEPS_CFLAGS) $(CFLAGS)
im_cim_gtk4_LDFLAGS = $(EXTRA_LDFLAGS) -Wl,--as-needed $(IM_CIM_GTK4_DEPS_LIBS)

all: $(IM_CIM_GTK4)

$(IM_CIM_GTK4): libim-cim-gtk4.o Makefile
	$(CC) -shared -fPIC libim-cim-gtk4.o $(im_cim_gtk4_LDFLAGS) \
	  -Wl,-soname -Wl,$(IM_CIM_GTK4) \
	  -o $(IM_CIM_GTK4)

libim-cim-gtk4.o: $(SOURCES) Makefile
	$(CC) -fPIC $(im_cim_gtk4_CFLAGS) -c $(SOURCES) -o libim-cim-gtk4.o

install:
	mkdir -p $(DESTDIR)$(gtk4_im_moduledir)
	install -m 755 $(IM_CIM_GTK4) $(DESTDIR)$(gtk4_im_moduledir)

uninstall:
	rm -f $(DESTDIR)$(gtk4_im_moduledir)/$(IM_CIM_GTK4)
	-rmdir -p $(DESTDIR)$(gtk4_im_moduledir)

clean:
	rm -f $(IM_CIM_GTK4) libim-cim-gtk4.o
