# port.mk builds the game on SDL3 with gcc or clang, for the target that
# OS and ARCH name: the modules of the game as C99, and src/sys, the
# layer that gives them Win32, DirectX, and WinMM.

# mingw-w64's name of each architecture, which its compilers and SDL's
# release carry.
MINGW_386 = i686-w64-mingw32
MINGW_amd64 = x86_64-w64-mingw32
MINGW = $(MINGW_$(ARCH))

# COMPILE is the compiler's command. gcc and clang take the architecture
# as a flag on Linux, where the 32-bit libraries of SDL3, FluidSynth,
# and zlib lie in /usr/lib, mingw-w64 names it in the command, and
# Apple's clang takes it with the oldest macOS, MACOS_MIN (mk/macos.mk).
# A pair of OS and ARCH outside the table stops make where a rule needs
# its compiler.
CC_linux_386 = $(CC) -m32
CC_linux_amd64 = $(CC) -m64
CC_macos_amd64 = $(CC) -arch x86_64 -mmacosx-version-min=$(MACOS_MIN)
CC_macos_arm64 = $(CC) -arch arm64 -mmacosx-version-min=$(MACOS_MIN)
CC_windows_386 = $(MINGW_386)-$(CC)
CC_windows_amd64 = $(MINGW_amd64)-$(CC)
COMPILE = $(or $(CC_$(OS)_$(ARCH)),$(error OS=$(OS) ARCH=$(ARCH): no such target on SDL3)) $(SANITIZE_$(SANITIZE))

# SANITIZE=asan builds with AddressSanitizer and
# UndefinedBehaviorSanitizer, each report fatal. -fno-sanitize=alignment
# lets the packed structures misalign their fields as Watcom's do, and
# LOOKCHECK holds the wide view's sight to lookaround's (world.cpp) at
# every step of the game. SANITIZE=tsan builds with ThreadSanitizer,
# which watches the audio threads of src/sys beside the game's.
SANITIZE_asan = -fsanitize=address,undefined -fno-sanitize=alignment -fno-sanitize-recover=all \
	-fno-omit-frame-pointer -DLOOKCHECK
SANITIZE_tsan = -fsanitize=thread -fno-omit-frame-pointer

# A Linux program links the system's SDL3, FluidSynth, and zlib, whose
# deflate streams the original and every target read alike. A Windows
# program links SDL3 and FluidSynth from build/, whose DLLs lie beside
# it under lowercase names, and the static zlib of mingw-w64, and
# windres gives it the window's icon. --no-insert-timestamp gives a
# Windows program the stamp 0 in place of the time of its link, so that
# every build gives the same bytes. A macOS program links SDL3 and
# FluidSynth from MAC (mk/macos.mk), whose dylibs lie in the app's
# Frameworks, which its rpath names, and the system's zlib. -Wl,-S
# leaves out the debug map, which names each object by its path and
# its time, so that every build gives the same bytes. The tests of a
# macOS target find the dylibs beside the program, in O.
LDLIBS_linux = -lSDL3 -lfluidsynth -lz -lm
LDLIBS_macos = -L$(MAC)/lib -lSDL3 -lfluidsynth -lz -lm -Wl,-rpath,@executable_path/../Frameworks -Wl,-S
LDLIBS_windows = -mwindows -static-libgcc -Wl,--no-insert-timestamp \
	-Lbuild/sdl/$(MINGW)/lib -Lbuild/fluidsynth/$(ARCH)/lib -lfluidsynth-3 -lSDL3 -l:libz.a
LDLIBS = $(LDLIBS_$(OS))
TESTLIBS_macos = -Wl,-rpath,@executable_path/..
INCLUDES_macos = -I$(MAC)/include
INCLUDES_windows = -Ibuild/sdl/$(MINGW)/include -Ibuild/fluidsynth/$(ARCH)/include
LIBS_macos = $(MAC)/.sdl $(MAC)/.fluidsynth
LIBS_windows = build/sdl/.ok build/fluidsynth/$(ARCH)/.ok
DLLS_macos = $(O)/libSDL3.0.dylib $(O)/libfluidsynth.3.dylib
DLLS_windows = $(O)/sdl3.dll $(O)/libfluidsynth-3.dll $(O)/sndfile.dll
LICENCES_macos = $(O)/fluidsynth-license.txt
LICENCES_windows = $(O)/fluidsynth-license.txt
RES_windows = $(O)/res.o

# The warnings that one compiler alone gives on the game's forms: clang
# on the prelude's macro long, which LP64 alone defines, and each
# compiler under its own name on locals that it guesses unset, which
# -ftrivial-auto-var-init=zero sets to zero.
WARN_gcc = -Wno-maybe-uninitialized
WARN_clang = -Wno-sometimes-uninitialized
WARN_clang_amd64 = -Wno-keyword-macro
WARN_clang_arm64 = -Wno-keyword-macro
WARN = $(WARN_$(CC)_$(ARCH)) $(WARN_$(CC))

# MACROS gives the modules FEATURE_<NAME> for each feature. With an
# empty list the program keeps the original's gameplay, with faults and
# pace, which every program on SDL3 takes.
MACROS := $(patsubst %,-DFEATURE_%,$(shell echo $(FEATURES) | tr '[:lower:]' '[:upper:]'))

# The flags. The game's modules compile as C99 with the prelude of
# src/sys, which gives their names Watcom's meaning, and with
# -fpack-struct=1, Watcom's -zp1. The -Wno flags leave the forms that
# the matching demands. -ffile-prefix-map writes the tree's path as . in
# the debug information, so that a build gives the same bytes wherever
# the tree lies. src/sys compiles with every warning an error.
CFLAGS = -std=c99 -pedantic-errors -Wall -O2 -g -ffile-prefix-map='$(CURDIR)'=. -funsigned-char -fwrapv \
	-fno-strict-aliasing
GAME_CFLAGS = $(CFLAGS) -Werror -fpack-struct=1 -ftrivial-auto-var-init=zero \
	-Wno-unused-variable -Wno-unused-but-set-variable -Wno-parentheses -Wno-char-subscripts \
	-Wno-address-of-packed-member -Wno-missing-braces -Wno-dangling-else \
	-Wno-unused-function -Wno-unused-label \
	-include src/sys/prelude.h -DPORT -DFEATURE_FAULTS -DFEATURE_PACE $(MACROS) \
	-Isrc/sys/include -Isrc/base
SYS_CFLAGS = $(CFLAGS) -Wextra -Werror -DPORT -Isrc/sys/include -Isrc/sys

# The objects, in their link order: the modules of the game, in sys/ the
# layer's, and sold.o and icon.o, which carry the release's licence
# bytes and icon.
SYS_MODULES = main win draw sound midi file reg kernel fmt zlib replay
SYS_OBJS = $(SYS_MODULES:%=$(O)/sys/%.o)
OBJS = $(patsubst %,$(O)/%.o,$(BASE_MODULES) $(GAME_MODULES)) $(SYS_OBJS) $(O)/sold.o $(O)/icon.o

$(O) $(O)/sys $(O)/test build/gen:
	mkdir -p $@

# cmd/sold writes the licence bytes of the release's teudogar.exe as C,
# at the objects that src/teudogar/sold.txt names.
build/gen/sold.c: src/teudogar/sold.txt build/zip/.ok $(BIN)/sold | build/gen
	$(BIN)/sold src/teudogar/sold.txt $(RELEASE)/teudogar.exe >$@

# cmd/rc writes the release's icons, and cmd/icon the window's, icon 10,
# as the pixels that SDL takes.
build/gen/10.ico: build/zip/.ok $(BIN)/rc | build/gen
	$(BIN)/rc $(RELEASE)/teudogar.exe build/gen >/dev/null

build/gen/icon.c: build/gen/10.ico $(BIN)/icon
	$(BIN)/icon $< >$@

# A module of the game compiles as C, with the headers it reads listed
# beside its object (-MMD).
$(O)/%.o: src/base/%.cpp build/features | $(O)
	$(COMPILE) -x c $(GAME_CFLAGS) $(WARN) -MMD -MP -c -o $@ $<

$(O)/%.o: src/teudogar/%.cpp build/features | $(O)
	$(COMPILE) -x c $(GAME_CFLAGS) $(WARN) -MMD -MP -c -o $@ $<

$(O)/%.o: build/gen/%.c | $(O)
	$(COMPILE) $(CFLAGS) -c -o $@ $<

# A Windows or macOS object of src/sys reads the headers of SDL3 and
# FluidSynth in build/.
$(O)/sys/%.o: src/sys/%.c $(LIBS_$(OS)) | $(O)/sys
	$(COMPILE) $(SYS_CFLAGS) $(INCLUDES_$(OS)) -MMD -MP -c -o $@ $<

# main.o gives SDL the release's VERSION, and compiles again when the
# Makefile, which sets it, changes.
$(O)/sys/main.o: SYS_CFLAGS += -DVERSION='"$(VERSION)"'
$(O)/sys/main.o: Makefile

# res.o gives a Windows program the window's icon.
$(O)/res.o: src/sys/icon.rc build/gen/10.ico | $(O)
	$(MINGW)-windres -I build/gen $< -o $@

-include $(wildcard $(O)/*.d $(O)/sys/*.d)

# The DLLs of a Windows program: SDL3's, FluidSynth's, and sndfile.dll,
# which FluidSynth loads.
$(O)/sdl3.dll: build/sdl/.ok | $(O)
	cp build/sdl/$(MINGW)/bin/SDL3.dll $@

$(O)/libfluidsynth-3.dll $(O)/sndfile.dll: $(O)/%.dll: build/fluidsynth/$(ARCH)/.ok | $(O)
	cp build/fluidsynth/$(ARCH)/bin/$*.dll $@

# fluidsynth-license.txt goes beside them: the LGPL 2.1 of FluidSynth's
# DLL and of sndfile.dll.
$(O)/fluidsynth-license.txt: $(FLUIDSYNTH_LICENCE) | $(O)
	$(call CHECK,$<)
	cp $< $@

# bin/symcheck checks the symbols of the linked program: a Linux program
# exports its startup symbols alone, so that every library calls its own
# functions, a Windows program and its DLLs import the system's DLLs and
# those of their folder, and a macOS program and its dylibs load the
# system's libraries and those of their folder, built for the target.
$(O)/teudogar$(EXE): $(OBJS) $(RES_$(OS)) $(DLLS_$(OS)) bin/symcheck
	$(COMPILE) -o $@ $(filter %.o,$^) $(LDLIBS)
	bin/symcheck $(TARGET) $@

# all builds the program.
all: $(O)/teudogar$(EXE)

# The distribution holds the program, the release's data files, the
# SoundFont and its licence, the libraries of a Windows or macOS program
# with theirs, and LICENSE, the licence of this tree's code, as
# license.txt, each in its place (Makefile), which SEAL ends for a macOS
# app (mk/macos.mk). DATA is the place of the data in the new folder.
# The folder is read only for the game, which keeps its saved games in
# the user's own.
DATA = $(D).new$(DATADIR_$(OS))
$(PROGRAM): $(O)/teudogar$(EXE) build/zip/.ok $(SOUNDFONT) $(SOUNDFONT_LICENCE) \
		$(LICENCES_$(OS)) $(APPFILES_$(OS)) LICENSE bin/lock
	rm -rf $(D).new && mkdir -p $(D).new$(PROGDIR_$(OS)) $(D).new$(LIBDIR_$(OS)) $(DATA)
	for f in $(RELEASE)/*; do \
		case $$f in *.exe | *'not for use'*) continue ;; esac; \
		cp -p "$$f" $(DATA)/ || exit 1; \
	done
	$(call CHECK,$(SOUNDFONT))
	$(call CHECK,$(SOUNDFONT_LICENCE))
	ln -f $(SOUNDFONT) $(DATA)/generaluser-gs.sf2 2>/dev/null || cp $(SOUNDFONT) $(DATA)/generaluser-gs.sf2
	cp $(SOUNDFONT_LICENCE) $(DATA)/generaluser-gs-license.txt
	cp LICENSE $(DATA)/license.txt
	cp $(O)/teudogar$(EXE) $(D).new$(PROGDIR_$(OS))/
	$(if $(DLLS_$(OS)),cp $(DLLS_$(OS)) $(D).new$(LIBDIR_$(OS))/)
	$(if $(LICENCES_$(OS)),cp $(LICENCES_$(OS)) $(DATA)/)
	$(SEAL_$(OS))
	$(SWAP)

# dist builds the distribution and its archive.
dist: $(PROGRAM) $(ARCHIVE)

# The unit tests of src/sys/test, each linked with the objects of
# src/sys but main.o, whose globals stub.c gives.
TESTS = file draw wide fmt replay
TEST_OBJS = $(filter-out $(O)/sys/main.o,$(SYS_OBJS))

$(O)/test/%$(EXE): src/sys/test/%.c src/sys/test/stub.c src/sys/test/test.h $(TEST_OBJS) | $(O)/test $(DLLS_$(OS))
	$(COMPILE) $(SYS_CFLAGS) $(INCLUDES_$(OS)) -o $@ $< src/sys/test/stub.c $(TEST_OBJS) \
		$(filter-out -mwindows,$(LDLIBS)) $(TESTLIBS_$(OS))

# test builds the unit tests and, for Linux and macOS, runs them through
# bin/headless, each in a folder of its own, and compares the table that
# fmt prints with src/sys/test/fmt.txt, which USER32's wsprintfA prints
# for it. The tests of a Windows target run on Windows or under Wine,
# beside the program's DLLs.
test: $(TESTS:%=$(O)/test/%$(EXE))
ifneq ($(OS),windows)
	for t in $(filter-out fmt,$(TESTS)); do \
		d=$(O)/test/run/$$t && rm -rf $$d && mkdir -p $$d && \
		bin/headless $(O)/test/$$t $$d >$$d.log 2>&1 || { cat $$d.log >&2; echo "make: test: $$t failed" >&2; exit 1; }; \
	done
	bin/headless $(O)/test/fmt | cmp - src/sys/test/fmt.txt
endif

# fmtref builds build/fmtref.exe, the table of src/sys/test/fmt.c
# through USER32's own wsprintfA, against mingw-w64's headers. Under
# Wine it writes the table of src/sys/test/fmt.txt:
#
#	wine build/fmtref.exe | tr -d '\r' >src/sys/test/fmt.txt
fmtref: build/fmtref.exe

build/fmtref.exe: src/sys/test/fmt.c
	mkdir -p $(@D) && $(MINGW_amd64)-gcc -std=c99 -O2 -Wl,--no-insert-timestamp -o $@ $< -luser32
