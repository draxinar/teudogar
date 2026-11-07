# fetch.mk gives make its inputs: teudogar.zip unpacked, and the files
# that make downloads into DL, each checked against its sum in sum/.

# The versions that make fetches: Watcom C/C++, which built the game,
# zlib, which the game links, SDL3 and FluidSynth, built for Windows and
# as sources for macOS (a Linux target takes the system's), gcem, the
# library of FluidSynth's source that its build reads, by its version
# and by the commit that FluidSynth names, and GeneralUser GS, the
# SoundFont of the music, by its version and by the commit of its
# repository that holds that version.
WATCOM_VERSION = 11.0
ZLIB_VERSION = 1.1.4
SDL_VERSION = 3.4.20
FLUIDSYNTH_VERSION = 2.6.2
GCEM_VERSION = 1.18.0
GCEM_COMMIT = 012ae73c6d0a2cb09ffe86475f5c6fba3926e200
SOUNDFONT_VERSION = 2.0.3
SOUNDFONT_COMMIT = 97049183643d5fc5a9322a69c5b09efb667c6c3a

# DL holds the downloads. make clean keeps it, and several trees can
# share one.
DL = dl

# CURL fetches a URL, and tries again when the network or the server
# fails.
CURL = curl -fsSL --retry 5 --retry-all-errors

# The downloads, under the names that sum/ gives them. FluidSynth names
# its builds x86 and x64.
WATCOM_ISO = $(DL)/watcom-$(WATCOM_VERSION).iso
ZLIB_TGZ = $(DL)/zlib-$(ZLIB_VERSION).tar.gz
SDL_TGZ = $(DL)/sdl3-devel-$(SDL_VERSION)-mingw.tar.gz
SDL_SRC = $(DL)/sdl3-$(SDL_VERSION).tar.gz
FLUIDSYNTH_386 = $(DL)/fluidsynth-v$(FLUIDSYNTH_VERSION)-win10-x86-cpp11.zip
FLUIDSYNTH_amd64 = $(DL)/fluidsynth-v$(FLUIDSYNTH_VERSION)-win10-x64-cpp11.zip
FLUIDSYNTH_LICENCE = $(DL)/fluidsynth-v$(FLUIDSYNTH_VERSION)-license.txt
FLUIDSYNTH_SRC = $(DL)/fluidsynth-$(FLUIDSYNTH_VERSION).tar.gz
GCEM_ZIP = $(DL)/gcem-$(GCEM_VERSION).zip
SOUNDFONT = $(DL)/generaluser-gs-$(SOUNDFONT_VERSION).sf2
SOUNDFONT_LICENCE = $(DL)/generaluser-gs-$(SOUNDFONT_VERSION)-license.txt
FETCHED = $(WATCOM_ISO) $(ZLIB_TGZ) $(SDL_TGZ) $(SDL_SRC) $(FLUIDSYNTH_386) $(FLUIDSYNTH_amd64) \
	$(FLUIDSYNTH_LICENCE) $(FLUIDSYNTH_SRC) $(GCEM_ZIP) $(SOUNDFONT) $(SOUNDFONT_LICENCE)

# fetch downloads every file that a build needs.
fetch: $(FETCHED)

# SHA256SUM checks files against lines of their sums: GNU's sha256sum,
# or on macOS shasum, since macOS's own sha256sum takes no --quiet.
SHA256SUM := $(if $(filter Darwin,$(word 1,$(HOST))),shasum -a 256,sha256sum) --quiet -c

# CHECK holds the file $(1) of DL to its line in sum/.
CHECK = cd $(dir $(1)) && grep -h '  $(notdir $(1))$$' '$(CURDIR)'/sum/*.sha256 | $(SHA256SUM)

# FETCH downloads the URL $(1) as $@: into DL/tmp first, where CHECK
# holds it to its sum before it takes its place.
define FETCH
mkdir -p $(DL)/tmp && $(CURL) -o $(DL)/tmp/$(@F) '$(1)'
$(call CHECK,$(DL)/tmp/$(@F))
mv $(DL)/tmp/$(@F) $@
endef

# archive.org holds the CD image of Watcom C/C++.
$(WATCOM_ISO):
	$(call FETCH,https://archive.org/download/Watcom_C_$(WATCOM_VERSION)/Watcom_C%2B%2B_$(WATCOM_VERSION).iso)

$(ZLIB_TGZ):
	$(call FETCH,https://zlib.net/fossils/$(@F))

# SDL's development release for mingw-w64.
$(SDL_TGZ):
	$(call FETCH,https://github.com/libsdl-org/SDL/releases/download/release-$(SDL_VERSION)/SDL3-devel-$(SDL_VERSION)-mingw.tar.gz)

# SDL's source release, which a macOS target builds.
$(SDL_SRC):
	$(call FETCH,https://github.com/libsdl-org/SDL/releases/download/release-$(SDL_VERSION)/SDL3-$(SDL_VERSION).tar.gz)

# FluidSynth's builds for Windows with the threads of C++11.
$(FLUIDSYNTH_386) $(FLUIDSYNTH_amd64):
	$(call FETCH,https://github.com/FluidSynth/fluidsynth/releases/download/v$(FLUIDSYNTH_VERSION)/$(@F))

# FluidSynth's source, which a macOS target builds: GitHub's archive of
# the release's tag, which leaves out the submodule gcem, and gcem's
# archive of the commit that FluidSynth names.
$(FLUIDSYNTH_SRC):
	$(call FETCH,https://github.com/FluidSynth/fluidsynth/archive/refs/tags/v$(FLUIDSYNTH_VERSION).tar.gz)

$(GCEM_ZIP):
	$(call FETCH,https://github.com/kthohr/gcem/archive/$(GCEM_COMMIT).zip)

# FluidSynth's licence, the text of the LGPL 2.1, under which FluidSynth
# and libsndfile release the DLLs that a Windows program carries. It
# comes from FluidSynth's repository at the release's tag, since the
# release's zip files hold the programs alone.
$(FLUIDSYNTH_LICENCE):
	$(call FETCH,https://raw.githubusercontent.com/FluidSynth/fluidsynth/v$(FLUIDSYNTH_VERSION)/LICENSE)

$(SOUNDFONT):
	$(call FETCH,https://raw.githubusercontent.com/mrbumpy409/GeneralUser-GS/$(SOUNDFONT_COMMIT)/GeneralUser-GS.sf2)

$(SOUNDFONT_LICENCE):
	$(call FETCH,https://raw.githubusercontent.com/mrbumpy409/GeneralUser-GS/$(SOUNDFONT_COMMIT)/documentation/LICENSE.txt)

# build/zip holds the release unpacked with lowercase names: teudogar.zip
# against sum/zip.sha256, and its 29 files against sum/teudogar.sha256.
build/zip/.ok: $(ZIP) $(PASSWORD) sum/zip.sha256 sum/teudogar.sha256 bin/unpack
	sed 's|  .*|  $(ZIP)|' sum/zip.sha256 | $(SHA256SUM)
	rm -rf build/zip && mkdir -p build && bin/unpack -p '$(PASSWORD)' '$(ZIP)' build/zip
	cd build/zip && $(SHA256SUM) '$(CURDIR)/sum/teudogar.sha256'
	touch $@

# build/watcom/<version> holds the CD of Watcom C/C++, which bin/watcom
# extracts from its image once the image has its sum.
build/watcom/.ok: $(WATCOM_ISO) sum/watcom.sha256 bin/watcom | $(BIN)/iso
	rm -rf build/watcom && bin/watcom $(DL) build/watcom $(WATCOM_VERSION)
	touch $@

# build/zlib holds the source of zlib.
build/zlib/.ok: $(ZLIB_TGZ) sum/zlib.sha256 bin/unpack
	$(call CHECK,$<)
	rm -rf build/zlib build/zlib.tmp && mkdir -p build/zlib.tmp && gzip -dc $< >build/zlib.tmp/zlib.tar
	bin/unpack build/zlib.tmp/zlib.tar build/zlib.tmp/src
	mv build/zlib.tmp/src/zlib-$(ZLIB_VERSION) build/zlib && rm -rf build/zlib.tmp && touch $@

# build/sdl holds SDL3 for mingw-w64, each architecture under mingw-w64's
# name for it: the headers, the link library, and SDL3.dll. tar keeps
# the names of the release, which the compilers need, such as
# SDL3/SDL.h.
build/sdl/.ok: $(SDL_TGZ) sum/sdl.sha256
	$(call CHECK,$<)
	rm -rf build/sdl && mkdir -p build/sdl && tar -xzf $< -C build/sdl --strip-components=1
	touch $@

# build/fluidsynth/<arch> holds FluidSynth for Windows: the headers, the
# link library, libfluidsynth-3.dll, and sndfile.dll, which it loads.
# MSVC built the DLLs with its C runtime inside.
build/fluidsynth/$(ARCH)/.ok: $(FLUIDSYNTH_$(ARCH)) sum/fluidsynth.sha256 bin/unpack
	$(call CHECK,$<)
	rm -rf $(@D) $(@D).tmp && mkdir -p build/fluidsynth && bin/unpack $< $(@D).tmp
	mv $(@D).tmp/* $(@D) && rmdir $(@D).tmp && touch $@
