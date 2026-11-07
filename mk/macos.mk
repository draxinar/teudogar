# macos.mk builds what a macOS target adds to the game on SDL3: SDL3 and
# FluidSynth from their sources, for the target's architecture, and the
# files of the app, Teudogar.app, which the distribution holds.

# MACOS_MIN is the oldest macOS that the programs run on: Big Sur, the
# first for Apple silicon, and the oldest that Xcode 26 builds for.
# MACARCH is Apple's name of each architecture.
MACOS_MIN = 11.0
MACARCH_arm64 = arm64
MACARCH_amd64 = x86_64
MACARCH = $(MACARCH_$(ARCH))

# MAC is the prefix of the target's libraries, with include/ and lib/,
# and MACSRC holds their sources.
MAC = build/macos/$(ARCH)
MACSRC = build/macos/src

# CMAKE configures a library for the target: its architecture, its
# oldest macOS, and the install names under @rpath, already in the build
# tree, so that the install copies each dylib as the linker signed it.
# -ffile-prefix-map keeps the tree's path out of the libraries' strings,
# so that every build gives the same bytes.
CMAKE = cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES=$(MACARCH) \
	-DCMAKE_OSX_DEPLOYMENT_TARGET=$(MACOS_MIN) -DCMAKE_INSTALL_PREFIX='$(CURDIR)/$(MAC)' \
	-DCMAKE_INSTALL_NAME_DIR=@rpath -DCMAKE_BUILD_WITH_INSTALL_NAME_DIR=ON \
	-DCMAKE_C_FLAGS='-ffile-prefix-map=$(CURDIR)=.' -DCMAKE_CXX_FLAGS='-ffile-prefix-map=$(CURDIR)=.'

# MACSRC holds SDL3's source, and FluidSynth's with gcem beside it.
# macOS's tar, libarchive's, reads the zip file of gcem too, and keeps
# its names as they are.
$(MACSRC)/.sdl: $(SDL_SRC) sum/sdl.sha256
	$(call CHECK,$<)
	rm -rf $(MACSRC)/sdl && mkdir -p $(MACSRC)/sdl && tar -xzf $< -C $(MACSRC)/sdl --strip-components=1
	touch $@

$(MACSRC)/.fluidsynth: $(FLUIDSYNTH_SRC) $(GCEM_ZIP) sum/fluidsynth.sha256
	$(call CHECK,$(FLUIDSYNTH_SRC))
	$(call CHECK,$(GCEM_ZIP))
	rm -rf $(MACSRC)/fluidsynth $(MACSRC)/gcem && mkdir -p $(MACSRC)/fluidsynth $(MACSRC)/gcem && \
		tar -xzf $(FLUIDSYNTH_SRC) -C $(MACSRC)/fluidsynth --strip-components=1 && \
		tar -xf $(GCEM_ZIP) -C $(MACSRC)/gcem
	touch $@

# SDL3 builds its shared library alone, libSDL3.0.dylib, with the
# offscreen video and the dummy audio that the tests use, and without
# OpenGL, which macOS deprecated: its renderer draws with Metal. With
# OpenGL each window of macOS asks for it, and the offscreen video of
# the tests and of bin/smoke loads the EGL that macOS lacks. A + line
# gets make's jobserver, which make 3.81 closes to the others.
$(MAC)/.sdl: $(MACSRC)/.sdl
	rm -rf $(MAC)/sdl && $(CMAKE) -S $(MACSRC)/sdl -B $(MAC)/sdl -DSDL_SHARED=ON -DSDL_STATIC=OFF \
		-DSDL_TEST_LIBRARY=OFF -DSDL_TESTS=OFF -DSDL_EXAMPLES=OFF -DSDL_OPENGL=OFF -DSDL_OPENGLES=OFF
	+cmake --build $(MAC)/sdl
	cmake --install $(MAC)/sdl
	touch $@

# FluidSynth builds its library alone, libfluidsynth.3.dylib, with the
# threads of C++11 and every option that the game leaves unused off:
# midi.c drives the synthesizer and loads an .sf2, so the library takes
# gcem, the threads, libm, and libc++, whatever else the host holds. The
# install of its two components leaves out the program fluidsynth. The
# empty DEFAULT_SOUNDFONT keeps the install's path out of the library,
# which names it as the SoundFont that the program fluidsynth loads.
FLUIDSYNTH_OPTIONS = -DBUILD_SHARED_LIBS=ON -Dosal=cpp11 -Denable-framework=OFF \
	-Denable-libsndfile=OFF -Denable-readline=OFF -Denable-openmp=OFF -Denable-sdl3=OFF \
	-Denable-coreaudio=OFF -Denable-coremidi=OFF -Denable-alsa=OFF -Denable-oss=OFF \
	-Denable-midishare=OFF -Denable-dbus=OFF -Denable-jack=OFF -Denable-pulseaudio=OFF \
	-Denable-pipewire=OFF -Denable-portaudio=OFF -Denable-network=OFF -Denable-ladspa=OFF \
	-Denable-aufile=OFF -Denable-signalsmith=OFF -DDEFAULT_SOUNDFONT= \
	-DGCEM_INCLUDE_DIR='$(CURDIR)/$(MACSRC)/gcem/gcem-$(GCEM_COMMIT)/include'

$(MAC)/.fluidsynth: $(MACSRC)/.fluidsynth
	rm -rf $(MAC)/fluidsynth && $(CMAKE) -S $(MACSRC)/fluidsynth -B $(MAC)/fluidsynth $(FLUIDSYNTH_OPTIONS)
	+cmake --build $(MAC)/fluidsynth --target libfluidsynth
	cmake --install $(MAC)/fluidsynth --component fluidsynth_runtime && \
		cmake --install $(MAC)/fluidsynth --component fluidsynth_development
	touch $@

# The dylibs of a macOS program lie beside it in O, for its tests and
# bin/symcheck, under the names that dyld asks for: cp follows the link
# libfluidsynth.3.dylib to its file. codesign signs each again, since an
# install may rewrite a file that the linker signed, and Apple silicon
# runs signed code alone.
$(O)/libSDL3.0.dylib: $(MAC)/.sdl | $(O)
	cp $(MAC)/lib/libSDL3.0.dylib $@ && codesign --force --sign - $@

$(O)/libfluidsynth.3.dylib: $(MAC)/.fluidsynth | $(O)
	cp $(MAC)/lib/libfluidsynth.3.dylib $@ && codesign --force --sign - $@

# The app's Info.plist takes the release's VERSION and MACOS_MIN, and
# its icon is the window's, which cmd/icns enlarges.
$(O)/info.plist: src/sys/info.plist Makefile mk/macos.mk | $(O)
	sed -e 's/@VERSION@/$(VERSION)/g' -e 's/@MACOS@/$(MACOS_MIN)/g' src/sys/info.plist >$@

build/gen/teudogar.icns: build/gen/10.ico $(BIN)/icns
	$(BIN)/icns $< >$@

# SEAL_macos ends the distribution's app: it puts Info.plist and the
# icon in place, clears the extended attributes that cp copies, whose
# Finder information and resource forks codesign refuses, and signs the
# dylibs, then the app, which seals its Resources. APPFILES_macos lists
# what it copies.
APPFILES_macos = $(O)/info.plist build/gen/teudogar.icns
SEAL_macos = cp $(O)/info.plist $(D).new$(APP_macos)/Info.plist && \
	cp build/gen/teudogar.icns $(D).new$(DATADIR_macos)/teudogar.icns && \
	xattr -cr $(D).new$(BUNDLE_macos) && \
	codesign --force --sign - $(D).new$(LIBDIR_macos)/*.dylib && \
	codesign --force --sign - $(D).new$(BUNDLE_macos)
