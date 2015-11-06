include ../mk.d/functions.mk

# If CROSS is set to 1, apply lib's overrides
ifeq ($(CROSS),1)
define libmake
$(call invoke,white,$(MAKE) $($1_OVERRIDES) --no-print-directory $2 -C $($1_DIR))
endef
# Else, don't touch anything
else
define libmake
$(call invoke,white,$(MAKE) --no-print-directory $2 -C $($1_DIR))
endef
endif

ifeq ($(CROSS),1)
define libbin
$1-cross
endef
else
define libbin
$1
endef
endif

force_look:
	@true

###############
### libconf ###
###############

LIBCONF_DIR=libconf
LIBCONF_INCDIR=include
LIBCONF_BINDIR=bin
LIBCONF_OVERRIDES=CXX=arm-cortex_a8-linux-gnueabi-g++ AR=arm-cortex_a8-linux-gnueabi-ar BIN_DIR=$(call libbin,$(LIBCONF_BINDIR))

.PHONY: libconf
libconf: force_look
	$(call libmake,LIBCONF,)

.PHONY: libconf_clean
libconf_clean: force_look
	$(call libmake,LIBCONF,clean)

.PHONY: libconf_incdir
libconf_incdir: force_look
	@echo $(LIBCONF_DIR)/$(LIBCONF_INCDIR)

.PHONY: libconf_bindir
libconf_bindir: force_look
	@echo $(LIBCONF_DIR)/$(call libbin,$(LIBCONF_BINDIR))
