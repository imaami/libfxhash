# SPDX-License-Identifier: MIT
override INSTALL := $(or $(if $(filter default,$(origin INSTALL)),,$(INSTALL)),install)
override PREFIX  := $(or $(if $(filter default,$(origin PREFIX)),,$(PREFIX)),/usr/local)
override STRIP   := $(or $(if $(filter default,$(origin STRIP)),,$(STRIP)),strip)

override INSTALL_STRIP := $(INSTALL) -s --strip-program=$(STRIP)

all: fxhash-c fxhash-rs libfxhash

install: | install-bin install-dev install-lib

install-bin: src/fxhash-c
	$(INSTALL_STRIP) -Dm 0755 -t $(DESTDIR)$(PREFIX)/bin $^

install-dev: include/fxhash.h include/libfxhash.h
	$(INSTALL) -Dm 0644 -t $(DESTDIR)$(PREFIX)/include/fxhash $^

install-lib: src/libfxhash.so
	$(INSTALL_STRIP) -Dm 0644 -t $(DESTDIR)$(PREFIX)/lib $^

fxhash-c:  | src/fxhash-c
fxhash-rs: | target/$(if $(DEBUG),debug,release)/fxhash-rs
libfxhash: | src/libfxhash.so
fxhash-c fxhash-rs:
	$(if $(wildcard $@),@:,ln -nfs $(firstword $|) $@)

src/fxhash-c: src/main.c
src/libfxhash.so: src/fxhash.c

src/fxhash-c src/libfxhash.so: include/fxhash.h
	+$(MAKE) -C src $(@:src/%=%)

target/$(if $(DEBUG),debug,release)/fxhash-rs: src/main.rs
	$(if $(RUSTFLAGS),RUSTFLAGS="$(RUSTFLAGS)" )cargo build$(if $(DEBUG),, --release)

clean: | .clean .clean-src .clean-target ; @:
.clean:; $(RM) fxhash-c fxhash-rs
.clean-src:; +$(MAKE) -C src clean
.clean-target:; $(RM) -r target/

.PHONY: all clean .clean .clean-src .clean-target install \
        install-bin install-dev install-lib libfxhash
