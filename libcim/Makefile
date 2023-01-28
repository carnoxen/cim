include ../config.mk

all: libcim.a libcim.so

LIBCIM_MAJOR = 0
LIBCIM_MINOR = 0
LIBCIM_MICRO = 0
LIBCIM_VERSION = $(LIBCIM_MAJOR).$(LIBCIM_MINOR).$(LIBCIM_MICRO)

C_SOURCES = cim.c \
	c-mem.c \
	c-str.c \
	c-utils.c

H_SOURCES = cim.h \
	c-array.h \
	c-log.h \
	c-macros.h \
	c-mem.h \
	c-str.h \
	c-types.h \
	c-utils.h

SOURCES   = $(H_SOURCES) $(C_SOURCES)

CFLAGS    = -I. $(EXTRA_CFLAGS)
DEPS_LIBS = $(EXTRA_LDFLAGS) -I.
LDFLAGS   = -Wl,-z,relro -Wl,--as-needed $(DEPS_LIBS)

libcim.a: Makefile $(SOURCES) $(C_SOURCES:.c=.o)
	$(AR) rcs libcim.a $(C_SOURCES:.c=.o)

libcim.so: Makefile $(SOURCES) $(C_SOURCES:.c=.o)
	$(CC) $(CFLAGS) -shared -fPIC $(C_SOURCES:.c=.o) $(LDFLAGS) \
		-Wl,-soname -Wl,libcim.so.$(LIBCIM_MAJOR) \
		-o libcim.so.$(LIBCIM_VERSION)
	ln -sf libcim.so.$(LIBCIM_VERSION) libcim.so
	ln -sf libcim.so.$(LIBCIM_VERSION) libcim.so.$(LIBCIM_MAJOR)
	ln -sf libcim.so.$(LIBCIM_VERSION) libcim.so.$(LIBCIM_MAJOR).$(LIBCIM_MINOR)

.SUFFIXES: .c .o
.c.o:
	$(CC) -fPIC $(CFLAGS) -c $< -o $@

install:

uninstall:

clean:
	rm -f $(C_SOURCES:.c=.o) libcim.a *.core vgcore.*
	rm -f libcim.so \
				libcim.so.$(LIBCIM_VERSION) \
				libcim.so.$(LIBCIM_MAJOR).$(LIBCIM_MINOR) \
				libcim.so.$(LIBCIM_MAJOR)
