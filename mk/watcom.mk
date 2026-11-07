# watcom.mk builds with Watcom C/C++ under DOSBox (bin/dos): the
# programs of 2008, byte for byte.

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
