# port.mk compiles the modules of the game as C99 with gcc or clang, for
# the target that OS and ARCH name.

# mingw-w64's name of each architecture, which its compilers carry.
MINGW_386 = i686-w64-mingw32
MINGW_amd64 = x86_64-w64-mingw32

# COMPILE is the compiler's command. gcc and clang take the architecture
# as a flag on Linux, and mingw-w64 names it in the command.
CC_linux_386 = $(CC) -m32
CC_linux_amd64 = $(CC) -m64
CC_windows_386 = $(MINGW_386)-$(CC)
CC_windows_amd64 = $(MINGW_amd64)-$(CC)
COMPILE = $(CC_$(OS)_$(ARCH))

# The warnings that one compiler alone gives on the game's forms: clang
# on the prelude's macro long, which LP64 alone defines, and each
# compiler under its own name on locals that it guesses unset, which
# -ftrivial-auto-var-init=zero sets to zero.
WARN_gcc = -Wno-maybe-uninitialized
WARN_clang = -Wno-sometimes-uninitialized
WARN_clang_amd64 = -Wno-keyword-macro
WARN = $(WARN_$(CC)_$(ARCH)) $(WARN_$(CC))

# MACROS gives the modules FEATURE_<NAME> for each feature. With an
# empty list they keep the original's gameplay, with faults and pace,
# which every module compiled as C99 takes.
MACROS := $(patsubst %,-DFEATURE_%,$(shell echo $(FEATURES) | tr '[:lower:]' '[:upper:]'))

# The flags. The game's modules compile as C99 with the prelude of
# src/sys, which gives their names Watcom's meaning, and with
# -fpack-struct=1, Watcom's -zp1. The -Wno flags leave the forms that
# the matching demands. -ffile-prefix-map writes the tree's path as . in
# the debug information, so that a build gives the same bytes wherever
# the tree lies.
CFLAGS = -std=c99 -pedantic-errors -Wall -O2 -g -ffile-prefix-map='$(CURDIR)'=. -funsigned-char -fwrapv \
	-fno-strict-aliasing
GAME_CFLAGS = $(CFLAGS) -Werror -fpack-struct=1 -ftrivial-auto-var-init=zero \
	-Wno-unused-variable -Wno-unused-but-set-variable -Wno-parentheses -Wno-char-subscripts \
	-Wno-address-of-packed-member -Wno-missing-braces -Wno-dangling-else \
	-Wno-unused-function -Wno-unused-label \
	-include src/sys/prelude.h -DPORT -DFEATURE_FAULTS -DFEATURE_PACE $(MACROS) \
	-Isrc/sys/include -Isrc/base

# The objects: the modules of the game.
OBJS = $(patsubst %,$(O)/%.o,$(BASE_MODULES) $(GAME_MODULES))

$(O):
	mkdir -p $@

# A module of the game compiles as C, with the headers it reads listed
# beside its object (-MMD).
$(O)/%.o: src/base/%.cpp build/features | $(O)
	$(COMPILE) -x c $(GAME_CFLAGS) $(WARN) -MMD -MP -c -o $@ $<

$(O)/%.o: src/teudogar/%.cpp build/features | $(O)
	$(COMPILE) -x c $(GAME_CFLAGS) $(WARN) -MMD -MP -c -o $@ $<

-include $(wildcard $(O)/*.d)

# all compiles the modules.
all: $(OBJS)
