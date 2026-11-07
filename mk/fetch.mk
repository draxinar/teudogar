# fetch.mk gives make its inputs: teudogar.zip unpacked, and the files
# that make downloads into DL, each checked against its sum in sum/.

# The versions that make fetches: Watcom C/C++, which built the game,
# and zlib, which the game links.
WATCOM_VERSION = 11.0
ZLIB_VERSION = 1.1.4

# DL holds the downloads. make clean keeps it, and several trees can
# share one.
DL = dl

# CURL fetches a URL, and tries again when the network or the server
# fails.
CURL = curl -fsSL --retry 5 --retry-all-errors

# The downloads, under the names that sum/ gives them.
WATCOM_ISO = $(DL)/watcom-$(WATCOM_VERSION).iso
ZLIB_TGZ = $(DL)/zlib-$(ZLIB_VERSION).tar.gz
FETCHED = $(WATCOM_ISO) $(ZLIB_TGZ)

# fetch downloads every file that a build needs.
fetch: $(FETCHED)

# CHECK holds the file $(1) of DL to its line in sum/.
CHECK = cd $(dir $(1)) && grep -h '  $(notdir $(1))$$' '$(CURDIR)'/sum/*.sha256 | sha256sum --quiet -c

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

# build/zip holds the release unpacked with lowercase names: teudogar.zip
# against sum/zip.sha256, and its 29 files against sum/teudogar.sha256.
build/zip/.ok: $(ZIP) $(PASSWORD) sum/zip.sha256 sum/teudogar.sha256 bin/unpack
	sed 's|  .*|  $(ZIP)|' sum/zip.sha256 | sha256sum --quiet -c
	rm -rf build/zip && mkdir -p build && bin/unpack -p '$(PASSWORD)' '$(ZIP)' build/zip
	cd build/zip && sha256sum --quiet -c '$(CURDIR)/sum/teudogar.sha256'
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
