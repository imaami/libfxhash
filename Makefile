# SPDX-License-Identifier: MIT
override INSTALL := $(or $(if $(filter default,$(origin INSTALL)),,$(INSTALL)),install)
override STRIP   := $(or $(if $(filter default,$(origin STRIP)),,$(STRIP)),strip)

all: fxhash-c fxhash-rs libfxhash.so

fxhash-c: src/fxhash-c
fxhash-rs: target/$(if $(DEBUG),debug,release)/fxhash-rs
libfxhash.so: src/libfxhash.so
src/fxhash-c: src/main.c
src/libfxhash.so: src/fxhash.c

fxhash-c fxhash-rs libfxhash.so:
	$(INSTALL)$(if $(DEBUG),, -s --strip-program=$(STRIP)) -T $< $@

src/fxhash-c src/libfxhash.so: include/fxhash/fxhash.h
	+$(MAKE) -C src $(@:src/%=%)

target/$(if $(DEBUG),debug,release)/fxhash-rs: src/main.rs
	cargo build$(if $(DEBUG),, --release)

clean: | .clean .clean-src .clean-target ; @:
.clean:; $(RM) fxhash-c fxhash-rs libfxhash.so
.clean-src:; +$(MAKE) -C src clean
.clean-target:; $(RM) -r target/

.PHONY: all clean .clean .clean-src .clean-target
