# Makefile builds Teudogar and the Alliance with Rome from its matching
# source and from teudogar.zip, the author's release of version 1.05.
# Plain make lists the goals.

# OS, ARCH, and CC name the target that all and dist build, and test for
# a program on SDL3: linux, macos, or windows, 386, amd64, or arm64, and
# gcc, clang, or watcom, the compiler of 2008, which builds windows on
# 386. macos builds on amd64 and arm64 with clang alone. OS and ARCH
# default to the host's, by uname's names in the tables below, and else
# to linux and amd64, and CC to gcc, or to clang on macOS.
# SANITIZE=asan or SANITIZE=tsan adds clang's sanitizers to a Linux
# target.
HOST := $(shell uname -sm)
OS_Linux = linux
OS_Darwin = macos
ARCH_x86_64 = amd64
ARCH_i686 = 386
ARCH_arm64 = arm64
ARCH_aarch64 = arm64
OS = $(or $(OS_$(word 1,$(HOST))),linux)
ARCH = $(or $(ARCH_$(word 2,$(HOST))),amd64)
CC = $(if $(filter macos,$(OS)),clang,gcc)
SANITIZE =

# FEATURES lists the features of the game that all builds: french, the
# French language, errata, the fixes of the German, English, and Italian
# texts, faults, the fixes of the code's faults, pace, the game's pace
# at any refresh, fixes, the fixes of the gameplay, which takes faults
# and pace with it, and slots, 23 slots of saved games and three
# AutoSaves in three pages of the menus. A program on SDL3 takes faults
# and pace whatever the list holds. LANGFLAGS passes flags to cmd/lang:
# -p gives French the English of the texts that lang/fr lacks.
FEATURES = french errata fixes slots
LANGFLAGS =

# ZIP is the author's release, and PASSWORD the file whose first line is
# its password.
ZIP = teudogar.zip
PASSWORD = password

# VERSION is the release's version, which names the archives of dist:
# the game's version, 1.05, then the release's number. DATE is the time
# of the game's link in UTC: bin/wlink stamps teudogar.exe with it, and
# the files of the archives take it.
VERSION = 1.05.1
DATE = 2008-05-20T08:23:29Z

# BASE_MODULES lists the modules of the author's library, and
# GAME_MODULES the game's own, in their link order.
BASE_MODULES = basio memio str
GAME_MODULES = zpack map dialog datfile texts input actions demotext graphics trade system combat cursor menus mainmenu npc unittype snd unpack world plt keys lang sysinfo deco panel screen

# TARGET names what all builds, O the directory of its program and its
# objects, and D its distribution, whose archive is ARCHIVE. A Windows
# program ends in .exe. BIN holds the Go commands, and RELEASE the
# release's files.
TARGET = $(OS)-$(ARCH)-$(CC)$(SANITIZE:%=-%)
O = build/$(TARGET)
D = dist/$(TARGET)
EXE_windows = .exe
EXE = $(EXE_$(OS))
ARCHIVE_linux = .tar.gz
ARCHIVE_macos = .zip
ARCHIVE_windows = .zip
ARCHIVE = dist/teudogar-$(VERSION)-$(TARGET)$(ARCHIVE_$(OS))
BIN = build/bin
RELEASE = build/zip/teudogar

# The places of a distribution, below D: the program's, PROGDIR, its
# libraries', LIBDIR, and the game's data's, DATADIR. Each is D itself
# but on macOS, where D holds the app, BUNDLE. PROGRAM is the program of
# the distribution.
BUNDLE_macos = /Teudogar.app
APP_macos = $(BUNDLE_macos)/Contents
PROGDIR_macos = $(APP_macos)/MacOS
LIBDIR_macos = $(APP_macos)/Frameworks
DATADIR_macos = $(APP_macos)/Resources
PROGRAM = $(D)$(PROGDIR_$(OS))/teudogar$(EXE)

# make keeps the target's variables out of the recipes' environment,
# where CC names the C compiler of go build.
unexport OS ARCH CC SANITIZE

.DELETE_ON_ERROR:
.PHONY: usage all dist test original ddraw check lint spell fetch tools text fmtref clean distclean FORCE

# usage lists the goals.
usage::
	@echo 'usage: make goal ... [OS=linux|macos|windows] [ARCH=386|amd64|arm64] [CC=gcc|clang|watcom]'
	@echo "                     [FEATURES='french errata fixes slots']"
	@echo ''
	@echo "the game, for the target that OS, ARCH, and CC name (the host's, with gcc,"
	@echo 'or clang on macOS):'
	@echo '  all        build the program into build/<os>-<arch>-<cc>'
	@echo '  dist       build its distribution into dist/<os>-<arch>-<cc>, the program'
	@echo '             with the data of the game, and the archive of that folder'
	@echo '  test       build the unit tests of the program, and run them on Linux and'
	@echo '             macOS'
	@echo ''
	@echo 'the programs of 2008:'
	@echo '  original   rebuild teudogar.exe and uninstal.exe with Watcom C/C++ and'
	@echo '             check their bytes'
	@echo '  ddraw      build ddraw.dll, which runs them on Windows 8 and later'
	@echo ''
	@echo 'checks:'
	@echo '  check      run lint, original, and test'
	@echo '  lint       run shellcheck, gofmt, and go vet'
	@echo '  spell      check the spelling of lang/fr with hunspell'
	@echo ''
	@echo 'inputs and tools:'
	@echo '  fetch      download what the builds need into dl/'
	@echo '  tools      build the Go commands into build/bin'
	@echo '  text       write the texts of the release into build/text'
	@echo '  fmtref     build build/fmtref.exe, the reference of the fmt test'
	@echo ''
	@echo '  clean      remove build/ and dist/'
	@echo '  distclean  remove the downloads too'

# A Go command of cmd/ builds into build/bin. go build decides whether it
# is stale, and the binary keeps its time unless its bytes change, so
# that only what a changed command made builds again. The flags keep
# the commit and the tree's path out of the bytes. A rule that depends
# on FORCE always runs.
$(BIN)/%: FORCE
	@mkdir -p $(BIN) && go build -trimpath -buildvcs=false -o $@.$$$$ ./cmd/$* && \
		{ cmp -s $@.$$$$ $@ && rm -f $@.$$$$ || mv $@.$$$$ $@; }

FORCE:

# tools builds every Go command.
tools: $(patsubst cmd/%,$(BIN)/%,$(wildcard cmd/*))

# build/features holds FEATURES and LANGFLAGS, and changes only when
# they do, so that a build with other features compiles again.
build/features: FORCE
	@mkdir -p build && echo 'FEATURES=$(FEATURES) LANGFLAGS=$(LANGFLAGS)' | cmp -s - $@ || \
		echo 'FEATURES=$(FEATURES) LANGFLAGS=$(LANGFLAGS)' >$@

# cmd/lang checks the sources of the features in lang/ and writes into
# build/lang what the features change: data/, the files of the game's
# folder, and include/, the headers of src/feature. slots comes after
# french, whose manual it changes too. internal/lang's copy of french.c,
# through which it checks the elisions, must equal src/feature's.
build/lang/.ok: build/features build/zip/.ok $(shell find lang -type f) $(wildcard src/teudogar/*.cpp) \
		internal/lang/french.c src/feature/french.c $(BIN)/lang
	sed '1,4d' internal/lang/french.c | cmp -s - src/feature/french.c || \
		{ echo 'make: internal/lang/french.c differs from src/feature/french.c: run go generate ./internal/lang' >&2; exit 1; }
	rm -rf build/lang && $(BIN)/lang -o build/lang -f '$(filter french errata,$(FEATURES)) $(filter slots,$(FEATURES))' -r lang/buffers.txt $(LANGFLAGS) $(RELEASE) src/teudogar lang
	touch $@

# SWAP puts the folder $(D).new in the place of $(D) under the lock of
# dist/.lock. A program that runs under bin/lock -s dist/.lock keeps its
# folder until it ends.
SWAP = bin/lock dist/.lock sh -c 'rm -rf $(D).old && { [ ! -d $(D) ] || mv $(D) $(D).old; } && mv $(D).new $(D) && rm -rf $(D).old'

# cmd/pack writes the distribution as an archive whose files take DATE,
# so that the same folder gives the same bytes: a gzipped tar file for
# Linux and a zip file for macOS and Windows.
$(ARCHIVE): $(PROGRAM) $(BIN)/pack
	$(BIN)/pack -t $(DATE) $(D) $@

# ddraw builds ddraw.dll, DirectDraw for the Watcom programs on Windows 8
# and later, and test.exe, its test, which runs beside it under Wine
# with WINEDLLOVERRIDES=ddraw=n,b. --no-insert-timestamp gives each the
# stamp 0 in place of the time of its link, so that every build gives
# the same bytes.
ddraw: build/ddraw/ddraw.dll build/ddraw/test.exe

build/ddraw/ddraw.dll: src/ddraw/ddraw.c
	mkdir -p $(@D) && i686-w64-mingw32-gcc -std=gnu99 -O2 -Wall -Wextra -shared -static-libgcc \
		-Wl,--kill-at,--no-insert-timestamp -o $@ $< -ldwmapi -lgdi32 -lwinmm

build/ddraw/test.exe: src/ddraw/test.c
	mkdir -p $(@D) && i686-w64-mingw32-gcc -std=gnu99 -O2 -Wall -Wextra -static-libgcc \
		-Wl,--no-insert-timestamp -o $@ $< -luser32

# text writes the German, English, and Italian texts of the release into
# build/text in the forms that lang/ keeps, as the translators'
# reference: build/text/<code>/<name>.txt for each text and readme, the
# language's strings of the program's texts by offset in
# build/text/<code>/texts.txt, and the groups of pick by French slot in
# build/text/texts.txt (cmd/lang -x).
text: build/text/.ok

build/text/.ok: build/zip/.ok $(wildcard src/teudogar/*.cpp) $(BIN)/lang
	rm -rf build/text && $(BIN)/lang -x build/text $(RELEASE) src/teudogar
	touch $@

# spell checks the spelling of lang/fr with hunspell's fr_FR dictionary
# and lang/fr/words.txt, the names, quotations, titles, and file names
# that the French keeps. cmd/lang -s gives it the French free of the
# game's marks and terms. hunspell reads its UTF-8 in the C.UTF-8
# locale, whatever the caller's. spell prints each word that the two
# reject, and fails when there is one or when hunspell fails.
spell: build/zip/.ok $(BIN)/lang
	$(BIN)/lang -s $(RELEASE) lang >build/spell.txt
	LC_ALL=C.UTF-8 hunspell -d fr_FR -p lang/fr/words.txt -l build/spell.txt >build/spell.raw
	sort -u build/spell.raw >build/spell.bad && cat build/spell.bad && test ! -s build/spell.bad

# lint runs shellcheck on bin/, and gofmt and go vet on cmd/ and
# internal/.
lint:
	shellcheck bin/*
	test -z "$$(gofmt -l cmd internal)" || { gofmt -l cmd internal >&2; exit 1; }
	go vet ./cmd/... ./internal/...

# check runs the linters, rebuilds the programs of 2008 to check their
# bytes, and runs the unit tests of the target.
check: lint original test

# clean removes what make built, and keeps the downloads.
clean:
	rm -rf build dist

# distclean removes the downloads too, each by its name.
distclean: clean
	rm -rf $(FETCHED) $(DL)/tmp

# The other rules: the inputs, the builds with Watcom C/C++, what a
# macOS target adds, and the builds on SDL3 with gcc and clang.
include mk/fetch.mk
include mk/watcom.mk
ifeq ($(OS),macos)
include mk/macos.mk
endif
ifneq ($(CC),watcom)
include mk/port.mk
endif
