include ../config.mk

SUBDIRS = gtk qt5 $(ENABLE_GTK4) $(ENABLE_QT6)

all:
	for subdir in $(SUBDIRS); do \
	  $(MAKE) -C $$subdir || exit 1; \
	done

install:
	for subdir in $(SUBDIRS); do \
	  $(MAKE) DESTDIR=$(DESTDIR) install -C $$subdir || exit 1; \
	done

uninstall:
	for subdir in $(SUBDIRS); do \
	  $(MAKE) DESTDIR=$(DESTDIR) uninstall -C $$subdir || exit 1; \
	done

clean:
	for subdir in $(SUBDIRS); do \
	  $(MAKE) clean -C $$subdir || exit 1; \
	done
