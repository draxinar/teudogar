# watcom.mk builds with Watcom C/C++ under DOSBox (bin/dos): the
# programs of 2008, byte for byte, and the game with FEATURES when CC is
# watcom.

# ZLIB_MODULES lists the modules of zlib that the game links, in their
# link order.
ZLIB_MODULES = adler32 compress crc32 deflate infblock infcodes inffast inflate inftrees infutil trees uncompr zutil

# The options that compile each family of code into the original's
# bytes: wcc386 for zlib, and wpp386 for the author's library and the
# game, and for the uninstaller.
ZLIB_CFLAGS = -bt=nt -od -s -zp1 -ei
BASE_CFLAGS = -bt=nt -s -oneatx -zp1
UNINSTAL_CFLAGS = -bt=nt -os -zp1

# UNINSTAL_TIME is the time of the uninstaller's link, in UTC.
UNINSTAL_TIME = 2005-09-13T17:32:29Z

# ORIGINAL holds the programs of 2008 and their objects, and WORK the
# directories of drive C: where the programs link.
ORIGINAL = build/original
WORK = build/dos/work
ZLIB_OBJS = $(ZLIB_MODULES:%=$(ORIGINAL)/zlib/%.obj)
ORIGINAL_OBJS = $(patsubst %,$(ORIGINAL)/teudogar/%.obj,$(BASE_MODULES) $(GAME_MODULES))

# build/dos is drive C: of bin/dos: in watcom, the DOS programs, the
# headers, and the 32-bit libraries of Watcom C/C++, hard links to its
# CD.
build/dos/.ok: build/watcom/.ok
	rm -rf build/dos && mkdir -p build/dos/watcom
	for d in binw h lib386; do cp -al build/watcom/$(WATCOM_VERSION)/$$d build/dos/watcom/$$d || exit 1; done
	touch $@

# WCC compiles $< with the Watcom compiler $(1) and the options $(2) in
# the directory $(3) of drive C:, made anew with the headers $(4), under
# the settings $(5) of bin/dos, and leaves the object as $@. The length
# of that directory's path is part of what wpp386 compiles: a module
# compiles in C:\GAME\<module>, a path whose length gives the original's
# bytes.
define WCC
rm -rf build/dos/$(3) && mkdir -p build/dos/$(3) $(@D) && cp $< $(4) build/dos/$(3)/
bin/dos -d 'C:\$(subst /,\,$(3))' -e 'INCLUDE=C:\WATCOM\H;C:\WATCOM\H\NT' $(5) $(1) $(2) $(<F) >/dev/null
mv build/dos/$(3)/$(basename $(<F)).obj $@
endef

# The stamp's rule unpacks zlib's sources.
$(ZLIB_MODULES:%=build/zlib/%.c): | build/zlib/.ok ;

# zlib compiles once for every program that links it.
$(ORIGINAL)/zlib/%.obj: build/zlib/%.c build/zlib/.ok build/dos/.ok bin/dos
	$(call WCC,wcc386,$(ZLIB_CFLAGS),zlib/$*,build/zlib/*.h)

$(ORIGINAL)/teudogar/%.obj: src/base/%.cpp src/base/base.h build/dos/.ok bin/dos
	$(call WCC,wpp386,$(BASE_CFLAGS),game/$*,src/base/*.h)

$(ORIGINAL)/teudogar/%.obj: src/teudogar/%.cpp src/base/base.h build/dos/.ok bin/dos
	$(call WCC,wpp386,$(BASE_CFLAGS),game/$*,src/base/*.h)

# The game links in C:\WORK\TEUDOGAR at DATE. cmd/rc writes the bitmap
# and the icons of the release's teudogar.exe and prints the script for
# its resources, which must equal teudogar.rc, and wrc compiles it. The
# link leaves teudogar.map beside the program.
$(WORK)/teudogar/teudogar.exe: $(ZLIB_OBJS) $(ORIGINAL_OBJS) src/teudogar/teudogar.lnk src/teudogar/teudogar.rc \
		build/zip/.ok bin/dos bin/wlink $(BIN)/pe $(BIN)/rc
	rm -rf $(@D) && mkdir -p $(@D) && cp $(filter %.obj %.lnk %.rc,$^) $(@D)/
	$(BIN)/rc $(RELEASE)/teudogar.exe $(@D) | cmp -s - src/teudogar/teudogar.rc
	bin/dos -d 'C:\WORK\TEUDOGAR' wrc -r -q -bt=nt teudogar.rc >/dev/null
	bin/wlink 'C:\WORK\TEUDOGAR' teudogar.lnk $@ '$(DATE)'

# The release marked each copy sold in the bytes that sold.txt lists, by
# address and length, which dd copies from the release's teudogar.exe.
$(ORIGINAL)/teudogar.exe: $(WORK)/teudogar/teudogar.exe src/teudogar/sold.txt $(BIN)/pe
	cp $< $@.tmp
	set -- $$($(BIN)/pe $@.tmp | awk '$$1 == "section" && $$3 == "DGROUP" { print $$4, $$7 }') && \
		while read -r a n rest; do \
			off=$$((0x$$a - 0x$$1 + $$2)) && \
			dd if=$(RELEASE)/teudogar.exe of=$@.tmp bs=1 skip=$$off seek=$$off count=$$n conv=notrunc status=none || exit 1; \
		done <src/teudogar/sold.txt
	mv $@.tmp $@

# The uninstaller compiles and links in C:\WORK\UNINSTAL at
# UNINSTAL_TIME. cmd/xor then XORs its strings, from first to last, with
# f7, as its decrypt expects.
$(ORIGINAL)/uninstal.exe: $(wildcard src/uninstal/*) build/dos/.ok build/zip/.ok bin/dos bin/wlink $(BIN)/pe $(BIN)/rc $(BIN)/xor
	rm -rf $(WORK)/uninstal && mkdir -p $(WORK)/uninstal $(@D) && cp src/uninstal/* $(WORK)/uninstal/
	bin/dos -d 'C:\WORK\UNINSTAL' -e 'INCLUDE=C:\WATCOM\H;C:\WATCOM\H\NT' wpp386 $(UNINSTAL_CFLAGS) uninstal.cpp >/dev/null
	$(BIN)/rc $(RELEASE)/uninstal.exe $(WORK)/uninstal | cmp -s - src/uninstal/uninstal.rc
	bin/dos -d 'C:\WORK\UNINSTAL' wrc -r -q -bt=nt uninstal.rc >/dev/null
	bin/wlink 'C:\WORK\UNINSTAL' uninstal.lnk $(WORK)/uninstal/uninstal.exe '$(UNINSTAL_TIME)'
	$(BIN)/xor f7 $$(sed -n 's/^\([0-9a-f]*\)[+*]* *char near first\[\].*$$/\1/p' $(WORK)/uninstal/uninstal.map) \
		$$(sed -n 's/^\([0-9a-f]*\)[+*]* *char near last\[\].*$$/\1/p' $(WORK)/uninstal/uninstal.map) \
		$(WORK)/uninstal/uninstal.exe >$@

# original rebuilds the programs of 2008 and checks their bytes:
# uninstal.exe has the release's sha256, and teudogar.exe that of
# sum/rebuild.sha256, the bytes of the rebuild until its last functions
# match the release's. A commit that matches another function pins the
# new bytes there.
original: $(ORIGINAL)/.ok

$(ORIGINAL)/.ok: $(ORIGINAL)/teudogar.exe $(ORIGINAL)/uninstal.exe sum/rebuild.sha256 sum/teudogar.sha256
	cd $(ORIGINAL) && sha256sum --quiet -c '$(CURDIR)/sum/rebuild.sha256'
	sed -n 's|  teudogar/uninstal.exe$$|  uninstal.exe|p' sum/teudogar.sha256 | (cd $(ORIGINAL) && sha256sum --quiet -c)
	touch $@

ifeq ($(CC),watcom)
ifneq ($(OS)-$(ARCH),windows-386)
$(error CC=watcom builds OS=windows ARCH=386)
endif

# The game with FEATURES. wpp386 takes FEATURE_<NAME> for each feature
# from its variable WPP386, since DOS holds a command line to 127
# characters.
GAME_FEATURES = $(sort $(FEATURES) $(if $(filter fixes,$(FEATURES)),faults pace))
FEATURE_MACROS := $(patsubst %,-dFEATURE_%=1,$(shell echo $(GAME_FEATURES) | tr '[:lower:]' '[:upper:]'))
FEATURE_ENV = -e 'WPP386=$(FEATURE_MACROS)'
FEATURE_OBJS = $(patsubst %,$(O)/%.obj,$(BASE_MODULES) $(GAME_MODULES))
FEATURE_DEPS = src/base/base.h build/features build/dos/.ok bin/dos

# A module compiles in C:\FEAT\<module>, as long as C:\GAME\<module>.
$(O)/%.obj: src/base/%.cpp $(FEATURE_DEPS)
	$(call WCC,wpp386,$(BASE_CFLAGS),feat/$*,src/base/*.h,$(FEATURE_ENV))

$(O)/%.obj: src/teudogar/%.cpp $(FEATURE_DEPS)
	$(call WCC,wpp386,$(BASE_CFLAGS),feat/$*,src/base/*.h,$(FEATURE_ENV))

# The game with FEATURES links in C:\WORK\GAME at DATE, as the original
# does in its own directory.
$(WORK)/game/teudogar.exe: $(ZLIB_OBJS) $(FEATURE_OBJS) src/teudogar/teudogar.lnk src/teudogar/teudogar.rc \
		build/zip/.ok bin/dos bin/wlink $(BIN)/pe $(BIN)/rc
	rm -rf $(@D) && mkdir -p $(@D) && cp $(filter %.obj %.lnk %.rc,$^) $(@D)/
	$(BIN)/rc $(RELEASE)/teudogar.exe $(@D) >/dev/null
	bin/dos -d 'C:\WORK\GAME' wrc -r -q -bt=nt teudogar.rc >/dev/null
	bin/wlink 'C:\WORK\GAME' teudogar.lnk $@ '$(DATE)'

# cmd/seal does what the author's release step did: it copies the
# licence bytes of sold.txt from the release, object by object, which
# cmd/wmap's dumps of the two link maps place, seals the program, and
# writes its copy word into teudogar.dat. The build starts from the
# original's bytes (make original): a hook that changes them stops it.
$(O)/teudogar.exe: $(WORK)/game/teudogar.exe $(ORIGINAL)/.ok src/teudogar/sold.txt $(BIN)/wmap $(BIN)/seal
	cp $< $@.tmp && cp $(RELEASE)/teudogar.dat $(O)/teudogar.dat.tmp
	$(BIN)/wmap $(WORK)/teudogar/teudogar.map >$(O)/release.syms && $(BIN)/wmap $(WORK)/game/teudogar.map >$(O)/game.syms
	$(BIN)/seal src/teudogar/sold.txt $(O)/release.syms $(RELEASE)/teudogar.exe $(O)/game.syms $@.tmp $(O)/teudogar.dat.tmp
	mv $(O)/teudogar.dat.tmp $(O)/teudogar.dat && mv $@.tmp $@

# all builds the sealed program.
all: $(O)/teudogar.exe

# The distribution is the release's folder with the sealed program, its
# teudogar.dat, ddraw.dll, which runs it on Windows 8 and later, and
# LICENSE, the licence of this tree's code, as license.txt, less the
# release's labels that call its programs "not for use". uninstal.exe
# stays, since checkfiles requires it. The program runs from it as the
# release's does. Wine keeps its own ddraw unless WINEDLLOVERRIDES
# names the folder's.
$(D)/teudogar.exe: $(O)/teudogar.exe build/zip/.ok build/ddraw/ddraw.dll LICENSE bin/lock
	rm -rf $(D).new && mkdir -p $(D).new
	for f in $(RELEASE)/*; do \
		case $$f in *'not for use'*) continue ;; esac; \
		cp -p "$$f" $(D).new/ || exit 1; \
	done
	cp $(O)/teudogar.exe $(O)/teudogar.dat build/ddraw/ddraw.dll $(D).new/
	cp LICENSE $(D).new/license.txt
	$(SWAP)

# dist builds the distribution and its archive.
dist: $(D)/teudogar.exe $(ARCHIVE)
endif
