include ../../config.mk

DEPS_CFLAGS = `pkg-config --cflags Qt6Core Qt6Gui Qt6Widgets gobject-2.0 gio-2.0`
DEPS_LIBS   = `pkg-config --libs   Qt6Core Qt6Gui Qt6Widgets gobject-2.0 gio-2.0`

# Lower versions of qt6 do not have .pc files.
# Uncomment if you have a lower version of Qt.
#DEPS_CFLAGS = $(QT6_CFLAGS) $(QT6_CORE_CFLAGS) $(QT6_GUI_CFLAGS) `pkg-config --cflags gobject-2.0 gio-2.0`
#DEPS_LIBS   = $(QT6_GUI_LIBS) $(QT6_CORE_LIBS) $(QT6_WIDGETS_LIBS) `pkg-config --libs gobject-2.0 gio-2.0`

TARGET = libqt6im-cim.so

BUILT_SOURCES = im-cim-qt.moc
SOURCES       = im-cim-qt.cpp $(BUILT_SOURCES)

CXXFLAGS = \
	$(EXTRA_CFLAGS) \
	-I$(top_srcdir)/libcim \
	-I$(QT6_CORE_PRIVATE_INCLUDE_PATH) \
	-I$(QT6_GUI_PRIVATE_INCLUDE_PATH) \
	$(DEPS_CFLAGS) \
	-DQT_NO_KEYWORDS \
	-std=c++17

LIBADD  = $(top_srcdir)/libcim/libcim.a
LDFLAGS = $(DEPS_LIBS) $(LIBADD)

all: $(TARGET) $(BUILT_SOURCES)

$(TARGET): im-cim-qt6.o
	$(CXX) -shared -fPIC $(EXTRA_LDFLAGS) im-cim-qt6.o \
	  -Wl,--as-needed $(LDFLAGS) \
	  -Wl,-soname -Wl,$(TARGET) \
	  -o $(TARGET)

im-cim-qt6.o: $(SOURCES) Makefile
	$(CXX) -fPIC $(CXXFLAGS) -c im-cim-qt.cpp -o $@

im-cim-qt.moc: im-cim-qt.cpp
	$(MOC_QT6) -I $(QT6_CORE_PRIVATE_INCLUDE_PATH) \
	           -I $(QT6_GUI_PRIVATE_INCLUDE_PATH) im-cim-qt.cpp \
	           -o im-cim-qt.moc

install:
	mkdir -p $(DESTDIR)$(QT6_IM_MODULE_DIR)
	install -m 755 $(TARGET) $(DESTDIR)$(QT6_IM_MODULE_DIR)

uninstall:
	 rm    -f $(DESTDIR)$(QT6_IM_MODULE_DIR)/$(TARGET)
	-rmdir -p $(DESTDIR)$(QT6_IM_MODULE_DIR)

clean:
	rm -f $(TARGET) im-cim-qt6.o im-cim-qt.moc
