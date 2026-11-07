# Makefile builds Teudogar and the Alliance with Rome from its matching
# source and from teudogar.zip, the author's release of version 1.05.
# Plain make lists the goals.

# ZIP is the author's release, and PASSWORD the file whose first line is
# its password.
ZIP = teudogar.zip
PASSWORD = password

# DATE is the time of the game's link in UTC, which bin/wlink stamps
# teudogar.exe with.
DATE = 2008-05-20T08:23:29Z

# BASE_MODULES lists the modules of the author's library, and
# GAME_MODULES the game's own, in their link order.
BASE_MODULES = basio memio str
GAME_MODULES = zpack map dialog datfile texts input actions demotext graphics trade system combat cursor menus mainmenu npc unittype snd unpack world plt keys lang sysinfo deco panel screen

# BIN holds the Go commands, and RELEASE the release's files.
BIN = build/bin
RELEASE = build/zip/teudogar

.DELETE_ON_ERROR:
.PHONY: usage original ddraw check lint fetch tools clean distclean FORCE

# usage lists the goals.
usage::
	@echo 'usage: make goal ...'
	@echo ''
	@echo 'the programs of 2008:'
	@echo '  original   rebuild teudogar.exe and uninstal.exe with Watcom C/C++ and'
	@echo '             check their bytes'
	@echo '  ddraw      build ddraw.dll, which runs them on Windows 8 and later'
	@echo ''
	@echo 'checks:'
	@echo '  check      run lint and original'
	@echo '  lint       run shellcheck, gofmt, and go vet'
	@echo ''
	@echo 'inputs and tools:'
	@echo '  fetch      download what the builds need into dl/'
	@echo '  tools      build the Go commands into build/bin'
	@echo ''
	@echo '  clean      remove build/'
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

# lint runs shellcheck on bin/, and gofmt and go vet on cmd/ and
# internal/.
lint:
	shellcheck bin/*
	test -z "$$(gofmt -l cmd internal)" || { gofmt -l cmd internal >&2; exit 1; }
	go vet ./cmd/... ./internal/...

# check runs the linters, and rebuilds the programs of 2008 to check
# their bytes.
check: lint original

# clean removes what make built, and keeps the downloads.
clean:
	rm -rf build

# distclean removes the downloads too, each by its name.
distclean: clean
	rm -rf $(FETCHED) $(DL)/tmp

# The other rules: the inputs, and the builds with Watcom C/C++.
include mk/fetch.mk
include mk/watcom.mk
