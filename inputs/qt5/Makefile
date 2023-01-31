include ../../config.mk

DEPS_CFLAGS = `pkg-config --cflags Qt5Gui  Qt5Widgets`
DEPS_LIBS   = `pkg-config --libs   Qt5Core Qt5Gui Qt5Widgets` \
	$(top_srcdir)/libcim/libcim.a

TARGET = libqt5im-cim.so

BUILT_SOURCES = im-cim-qt.moc
SOURCES       = im-cim-qt.cpp $(BUILT_SOURCES)

CXXFLAGS = \
	$(EXTRA_CFLAGS) \
	-I$(top_srcdir)/libcim \
	-I$(QT5_CORE_PRIVATE_INCLUDE_PATH) \
	-I$(QT5_GUI_PRIVATE_INCLUDE_PATH) \
	$(DEPS_CFLAGS) \
	-DQT_NO_KEYWORDS \
	-std=c++11

LDFLAGS = $(EXTRA_LDFLAGS) -Wl,--as-needed $(DEPS_LIBS)

all: $(TARGET) $(BUILT_SOURCES)

$(TARGET): im-cim-qt5.o
	$(CXX) -shared -fPIC im-cim-qt5.o $(LDFLAGS) \
	  -Wl,-soname -Wl,$(TARGET) \
	  -o $(TARGET)

im-cim-qt5.o: $(SOURCES) Makefile
	$(CXX) -fPIC $(CXXFLAGS) -c im-cim-qt.cpp -o $@

im-cim-qt.moc: im-cim-qt.cpp
	$(MOC_QT5) -I $(QT5_CORE_PRIVATE_INCLUDE_PATH) \
	           -I $(QT5_GUI_PRIVATE_INCLUDE_PATH) im-cim-qt.cpp \
	           -o im-cim-qt.moc

install:
	mkdir -p $(DESTDIR)$(QT5_IM_MODULE_DIR)
	install -m 755 $(TARGET) $(DESTDIR)$(QT5_IM_MODULE_DIR)

uninstall:
	 rm    -f $(DESTDIR)$(QT5_IM_MODULE_DIR)/$(TARGET)
	-rmdir -p $(DESTDIR)$(QT5_IM_MODULE_DIR)

clean:
	rm -f $(TARGET) im-cim-qt5.o im-cim-qt.moc
