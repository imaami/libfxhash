# SPDX-License-Identifier: MIT
override BIN := fxhash-c
override LIB := libfxhash.so

override BIN_SRC := main.c
override LIB_SRC := fxhash.c
override BIN_OBJ := $(BIN_SRC:%=%.o)
override LIB_OBJ := $(LIB_SRC:%=%.o)
override BIN_CDB := $(BIN_OBJ:%=.%.json)
override LIB_CDB := $(LIB_OBJ:%=.%.json)

override DEP := $(BIN_SRC:%=%.d) $(LIB_SRC:%=%.d)

override CC  := $(or $(if $(filter default,$(origin CC)),,$(CC)),$(if $(USE_CLANG),clang,gcc))

override WARNFLAGS := $(strip -Wall -Wextra -Wformat=2 \
  -Wno-declaration-after-statement $(if $(USE_CLANG),  \
  -Weverything                                         \
  -Wno-c++98-compat                                    \
  -Wno-disabled-macro-expansion                        \
  -Wno-gnu-statement-expression                        \
  -Wno-gnu-statement-expression-from-macro-expansion   \
  -Wno-gnu-zero-variadic-macro-arguments               \
  -Wno-switch-enum                                     \
  -Wno-unknown-warning-option                          \
  -Wno-unsafe-buffer-usage                             \
  -Wno-used-but-marked-unused                          \
) -Wno-unknown-pragmas)

override CFLAGS := $(strip $(if $(DEBUG),,-DNDEBUG)    \
  -flto -O$(if $(DEBUG),$(if $(USE_CLANG),0,g) -g3,2)  \
  $(SANITIZE:%=-fsanitize=%) $(WARNFLAGS) -pipe        \
  $(if $(filter default,$(origin CFLAGS)),,            \
       $(filter-out -fPIC -shared,$(CFLAGS)))          \
)
override LIB_CFLAGS := $(CFLAGS) -fPIC

all: $(BIN) $(LIB)

$(BIN): $(BIN_OBJ)
	$(CC) $(CFLAGS) -o $@ $^
	$(if $(USE_CLANG),@+$(MAKE) .$(BIN).json)

$(LIB) $(LIB_OBJ): override CFLAGS := $(LIB_CFLAGS)

$(LIB): $(LIB_OBJ)
	$(CC) $(CFLAGS) -shared -o $@ $^
	$(if $(USE_CLANG),@+$(MAKE) .$(LIB).json)

%.c.o: %.c
	$(CC) $(CFLAGS) -o $@ -c -I. -I../include -MMD -MQ $@ -MF $<.d$(if $(USE_CLANG), -MJ .$@.json) $<

ifdef USE_CLANG
.%.o.json: | %.o; @:

.$(BIN).json: $(BIN_CDB)
.$(LIB).json: $(LIB_CDB)
.$(BIN).json \
.$(LIB).json \
	:; cat $^ > $@
endif

-include $(DEP)

.PHONY: all clean
clean:
	$(RM) $(BIN) $(LIB) $(BIN_OBJ) $(LIB_OBJ) $(BIN_CDB) $(LIB_CDB) $(DEP)
