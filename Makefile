MAJOR=1
MINOR=1
RELEASE=1
BUILD=1
VERSION=$(MAJOR).$(MINOR).$(RELEASE).$(BUILD)
CC=$(CROSS_COMPILE)gcc
CCLD=$(CC)
STRIP=$(CROSS_COMPILE)strip
WINDRES=$(CROSS_COMPILE)windres
RANLIB=$(CROSS_COMPILE)ranlib
CFLAGS=-Wall -Wextra -Werror
CCFLAGS=-pipe -fPIC -Wno-unused-command-line-argument -Wno-unused-function
LDFLAGS=-L.
LIBS=
ifndef NDEBUG
ifndef NODEBUG
CFLAGS+=-g
LDFLAGS+=-g
endif
endif
ifdef INSTALL_PATH
DESTDIR=$(INSTALL_PATH)
endif
ifndef INSTALL_ROOT
DESTDIR=$(INSTALL_ROOT)
endif
PREFIX=/usr
BINDIR=$(PREFIX)/bin
LIBDIR=$(PREFIX)/lib
INCDIR=$(PREFIX)/include
PKGCONF_DIR=$(LIBDIR)/pkgconfig
ifdef NOSONAME
VER=
VERFULL=
else
ifdef WIN32
VER=-$(MAJOR)
VERFULL=-$(MAJOR)-$(MINOR)-$(RELEASE)
else
VER=.$(MAJOR)
VERFULL=.$(MAJOR).$(MINOR).$(RELEASE)
endif
endif
ifdef WIN32
SOEXT=$(VER).dll
SOFULLEXT=$(VERFULL).dll
SOLESSEXT=.dll
OBJEXT=.obj
LIBEXT=.lib
EXECEXT=.exe
else
SOEXT=.so$(VER)
SOFULLEXT=.so$(VERFULL)
SOLESSEXT=.so
OBJEXT=.o
LIBEXT=.a
EXECEXT=
endif
ifndef NO_RPATH
LDFLAGS+=-Wl,-rpath -Wl,"$(shell pwd)"
endif
LIB_IPCALC=\
	libipcalculator/ipcalc_internal$(OBJEXT) \
	libipcalculator/ipcalc_public$(OBJEXT) \
	libipcalculator/ipv4_check$(OBJEXT) \
	libipcalculator/ipv4_convert$(OBJEXT) \
	libipcalculator/ipv4_get$(OBJEXT) \
	libipcalculator/ipv4_misc$(OBJEXT) \
	libipcalculator/ipv4_object$(OBJEXT) \
	libipcalculator/ipv4_parse$(OBJEXT) \
	libipcalculator/ipv4_type$(OBJEXT) \
	libipcalculator/ipv4_print$(OBJEXT)
EXE_IPCALC=\
	tools/ipcalc$(OBJEXT) \
	tools/ipcalc_usage$(OBJEXT) \
	tools/ipcalc_result$(OBJEXT)
ifdef WIN32
LIBS+=-liphlpapi -lws2_32
EXE_IPCALC+=tools/interface_windows$(OBJEXT)
else
EXE_IPCALC+=tools/interface_netlink$(OBJEXT) tools/interface_getifaddrs$(OBJEXT)
endif
all: \
	ipcalc$(EXECEXT) \
	libipcalculator$(LIBEXT) \
	libipcalculator$(SOFULLEXT) \
	libipcalculator$(SOLESSEXT) \
	libipcalculator$(SOEXT) \
	libipcalculator.pc
ifdef WIN32
data/ipcalc.res: data/ipcalc.rc data/ipcalc.ico
data/libipcalculator.res: data/libipcalculator.rc
ipcalc$(EXECEXT): ipcalc$(LIBEXT) libipcalculator$(SOLESSEXT) data/ipcalc.res
	@echo "  CCLD    $@"
	@$(CCLD) $(CCFLAGS) $(LDFLAGS) ipcalc$(LIBEXT) -lipcalculator data/ipcalc.res $(LIBS) -o $@
ipcalc-static$(EXECEXT): data/ipcalc.res ipcalc$(LIBEXT) libipcalculator$(LIBEXT)
	@echo "  CCLD    $@"
	@$(CCLD) $(CCFLAGS) $(LDFLAGS) -static ipcalc$(LIBEXT) libipcalculator$(LIBEXT) data/ipcalc.res $(LIBS) -o $@
libipcalculator$(SOFULLEXT): data/libipcalculator.res $(LIB_IPCALC)
	@echo "  CCLD    $@"
	@$(CCLD) $(CCFLAGS) $(LDFLAGS) -shared -Wl,-soname -Wl,libipcalculator$(SOEXT) $^ $(LIBS) -o $@
else
ipcalc$(EXECEXT): ipcalc$(LIBEXT) libipcalculator$(SOLESSEXT)
	@echo "  CCLD    $@"
	@$(CCLD) $(CCFLAGS) $(LDFLAGS) ipcalc$(LIBEXT) -lipcalculator $(LIBS) -o $@
ipcalc-static$(EXECEXT): ipcalc$(LIBEXT) libipcalculator$(LIBEXT)
	@echo "  CCLD    $@"
	@$(CCLD) $(CCFLAGS) $(LDFLAGS) -static ipcalc$(LIBEXT) libipcalculator$(LIBEXT) $(LIBS) -o $@
libipcalculator$(SOFULLEXT): $(LIB_IPCALC)
	@echo "  CCLD    $@"
	@$(CCLD) $(CCFLAGS) $(LDFLAGS) -shared -Wl,-soname -Wl,libipcalculator$(SOEXT) $^ $(LIBS) -o $@
endif
ipcalc$(LIBEXT): $(EXE_IPCALC)
ifneq ($(SOEXT),$(SOFULLEXT))
libipcalculator$(SOEXT): libipcalculator$(SOFULLEXT)
	@echo "  SYMLINK $@"
	@ln -sf libipcalculator$(SOFULLEXT) libipcalculator$(SOEXT)
libipcalculator$(SOLESSEXT): libipcalculator$(SOFULLEXT)
	@echo "  SYMLINK $@"
	@ln -sf libipcalculator$(SOFULLEXT) libipcalculator$(SOLESSEXT)
endif
libipcalculator$(LIBEXT): $(LIB_IPCALC)
%.res:
	@echo "  WINDRES $@"
	@$(WINDRES) $< -O coff $@
%$(OBJEXT): %.c Makefile
	@echo "  CC      $@"
	@$(CC) $(CCFLAGS) $(CFLAGS) -DIPCALC_VERSION=\"$(VERSION)\" -c $< -o $@
%$(LIBEXT):
	@echo "  AR      $@"
	@$(AR) cr $@ $^
	@echo "  RANLIB  $@"
	@$(RANLIB) $@
libipcalculator.pc: libipcalculator/libipcalculator.pc.in
	@echo "  GEN     $@"
	@sed \
		-e "s,@LIBS@,$(LIBS),g" \
		-e "s,@LIBDIR@,$(LIBDIR),g" \
		-e "s,@BINDIR@,$(BINDIR),g" \
		-e "s,@INCDIR@,$(INCDIR),g" \
		-e "s,@VERSION@,$(VERSION),g" \
	$< > $@
install-bin: ipcalc$(EXECEXT)
	@echo "  MKD     $(DESTDIR)$(BINDIR)"
	@mkdir -p $(DESTDIR)$(BINDIR)
	@echo "  CPY     ipcalc$(EXECEXT)"
	@cp ipcalc$(EXECEXT) $(DESTDIR)$(BINDIR)/
	@echo "  CHMOD   $(BINDIR)/ipcalc$(EXECEXT)"
	@chmod 755 $(DESTDIR)$(BINDIR)/ipcalc$(EXECEXT)
	@echo "  PATCH   $(DESTDIR)$(BINDIR)/ipcalc$(EXECEXT)"
	@-patchelf --remove-rpath $(DESTDIR)$(BINDIR)/ipcalc$(EXECEXT)
	@echo "  STRIP   $(DESTDIR)$(BINDIR)/ipcalc$(EXECEXT)"
	@$(STRIP) $(DESTDIR)$(BINDIR)/ipcalc$(EXECEXT)
install-lib: libipcalculator$(SOEXT) libipcalculator$(SOFULLEXT) libipcalculator$(SOLESSEXT)
	@echo "  MKD     $(DESTDIR)$(LIBDIR)"
	@mkdir -p $(DESTDIR)$(LIBDIR)
	@echo "  CPY     libipcalculator$(SOEXT) libipcalculator$(SOFULLEXT) libipcalculator$(SOLESSEXT)"
	@cp -P \
		libipcalculator$(LIBEXT) \
		libipcalculator$(SOEXT) \
		libipcalculator$(SOLESSEXT) \
		libipcalculator$(SOFULLEXT) \
		$(DESTDIR)$(LIBDIR)/
	@echo "  CHMOD   $(DESTDIR)$(LIBDIR)/libipcalculator$(LIBEXT)"
	@echo "  CHMOD   $(DESTDIR)$(LIBDIR)/libipcalculator$(SOEXT)"
	@echo "  CHMOD   $(DESTDIR)$(LIBDIR)/libipcalculator$(SOFULLEXT)"
	@echo "  CHMOD   $(DESTDIR)$(LIBDIR)/libipcalculator$(SOLESSEXT)"
	@chmod 755 \
		$(DESTDIR)$(LIBDIR)/libipcalculator$(LIBEXT) \
		$(DESTDIR)$(LIBDIR)/libipcalculator$(SOEXT) \
		$(DESTDIR)$(LIBDIR)/libipcalculator$(SOFULLEXT) \
		$(DESTDIR)$(LIBDIR)/libipcalculator$(SOLESSEXT)
	@echo "  STRIP   $(DESTDIR)$(LIBDIR)/libipcalculator$(SOEXT)"
	@echo "  STRIP   $(DESTDIR)$(LIBDIR)/libipcalculator$(SOFULLEXT)"
	@echo "  STRIP   $(DESTDIR)$(LIBDIR)/libipcalculator$(SOLESSEXT)"
	@$(STRIP) \
		$(DESTDIR)$(LIBDIR)/libipcalculator$(SOEXT) \
		$(DESTDIR)$(LIBDIR)/libipcalculator$(SOFULLEXT) \
		$(DESTDIR)$(LIBDIR)/libipcalculator$(SOLESSEXT)
install-headers: libipcalculator/ipv4.h libipcalculator.pc
	@echo "  MKD     $(DESTDIR)$(INCDIR)"
	@echo "  MKD     $(DESTDIR)$(PKGCONF_DIR)"
	@mkdir -p \
		$(DESTDIR)$(INCDIR) \
		$(DESTDIR)$(PKGCONF_DIR)
	@echo "  CPY     ipv4.h"
	@cp ipv4.h $(DESTDIR)$(INCDIR)/
	@echo "  CPY     libipcalculator.pc"
	@cp libipcalculator.pc $(DESTDIR)$(PKGCONF_DIR)/
	@echo "  CHMOD   $(DESTDIR)$(INCDIR)/ipv4.h"
	@echo "  CHMOD   $(DESTDIR)$(PKGCONF_DIR)/libipcalculator.pc"
	@chmod 644 \
		$(DESTDIR)$(INCDIR)/ipv4.h \
		$(DESTDIR)$(PKGCONF_DIR)/libipcalculator.pc
install: install-bin install-lib install-headers
clean-lib:
	@echo "  CLEAN   *.a"
	@find ! -type d -name '*.a' -exec rm -f {} \;
	@echo "  CLEAN   *.lib"
	@find ! -type d -name '*.lib' -exec rm -f {} \;
	@echo "  CLEAN   lib*.so*"
	@find ! -type d -name 'lib*.so*' -exec rm -f {} \;
	@echo "  CLEAN   lib*.dll"
	@find ! -type d -name 'lib*.dll' -exec rm -f {} \;

clean-bin:
	@echo "  CLEAN   ipcalc"
	@find ! -type d -name 'ipcalc' -type f -exec rm -f {} \;
	@echo "  CLEAN   ipcalc.exe"
	@find ! -type d -name 'ipcalc.exe' -type f -exec rm -f {} \;
	@echo "  CLEAN   ipcalc-static"
	@find ! -type d -name 'ipcalc-static' -type f -exec rm -f {} \;
	@echo "  CLEAN   ipcalc-static.exe"
	@find ! -type d -name 'ipcalc-static.exe' -type f -exec rm -f {} \;
clean-obj:
	@echo "  CLEAN   *.o"
	@find ! -type d -name '*.o' -exec rm -f {} \;
	@echo "  CLEAN   *.obj"
	@find ! -type d -name '*.obj' -exec rm -f {} \;
	@echo "  CLEAN   *.h.gch"
	@find ! -type d -name '*.h.gch' -exec rm -f {} \;
	@echo "  CLEAN   *.plist"
	@find ! -type d -name '*.plist' -exec rm -f {} \;
	@echo "  CLEAN   *.res"
	@find ! -type d -name '*.res' -exec rm -f {} \;
clean-gen:
	@echo "  CLEAN   libipcalculator.pc"
	@rm -f libipcalculator.pc
clean-makepkg:
	@echo "  CLEAN   makepkg"
	@rm -rf makepkg
	@echo "  CLEAN   *.pkg.tar*"
	@find ! -type d -name '*.pkg.tar*' -exec rm -f {} \;
clean-build: clean-bin clean-lib clean-obj
	@echo "  CLEAN   build"
	@rm -rf build
	@echo "  CLEAN   cmake-build-debug"
	@rm -rf cmake-build-debug
clean: clean-build clean-gen clean-makepkg
makepkg: data/PKGBUILD
	@rm -rf makepkg
	@mkdir makepkg makepkg/src
	@tail -n +3 data/PKGBUILD | sed s/@VERSION@/$(VERSION)/g > makepkg/PKGBUILD
	@ln -s ../../tools makepkg/src/tools
	@ln -s ../../libipcalculator makepkg/src/libipcalculator
	@for i in $$(grep 'source=' data/PKGBUILD|cut -d'(' -f2|cut -d')' -f1);do \
		ln -s ../$$(find -name "$$i" -type f) makepkg/$$i;\
		ln -s ../../$$(find -name "$$i" -type f) makepkg/src/$$i;\
	done
	@cd makepkg&&makepkg -e
	@mv makepkg/ipcalc-*.pkg.tar.* .
ifdef WIN32
meson: meson.build data/meson-mingw64.ini
	@meson --cross-file data/meson-mingw64.ini build .
	@ninja -C build
cmake: CMakeLists.txt data/cmake-mingw64.cmake
	@cmake -DCMAKE_TOOLCHAIN_FILE=data/cmake-mingw64.cmake -B build -S .
	@$(MAKE) -C build
else
meson: meson.build
	@meson build .
	@ninja -C build
cmake: CMakeLists.txt
	@cmake -B build -S .
	@$(MAKE) -C build
endif
.PHONY: all install clean-lib clean-bin clean-obj clean-build clean force makepkg meson cmake
