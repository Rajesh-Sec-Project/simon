include ../mk.d/sh.mk

include ../mk.d/functions.mk

# If CROSS is set to 1, apply lib's overrides
ifeq ($(CROSS),1)
define libmake
$(call invoke,white,$(MAKE) $1 $($2_OVERRIDES) --no-print-directory $3 -C $($2_DIR))
endef
# Else, don't touch anything
else
define libmake
$(call invoke,white,$(MAKE) $1 --no-print-directory $3 -C $($2_DIR))
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
LIBCONF_BUILDDIR=build
LIBCONF_OVERRIDES=CXX=arm-cortex_a8-linux-gnueabi-g++ \
                  AR=arm-cortex_a8-linux-gnueabi-ar \
                  BIN_DIR=$(call libbin,$(LIBCONF_BINDIR)) \
                  BUILD_DIR=$(call libbin,$(LIBCONF_BUILDDIR))

.PHONY: libconf_release
libconf_release: force_look
	$(call libmake,release=1,LIBCONF,)

.PHONY: libconf_debug
libconf_debug: force_look
	$(call libmake,release=0,LIBCONF,)

.PHONY: libconf_clean
libconf_clean: force_look
	$(call libmake,,LIBCONF,clean)

.PHONY: libconf_incdir
libconf_incdir: force_look
	@echo $(LIBCONF_DIR)/$(LIBCONF_INCDIR)

.PHONY: libconf_bindir
libconf_bindir: force_look
	@echo $(LIBCONF_DIR)/$(call libbin,$(LIBCONF_BINDIR))

###############
### libcomm ###
###############

LIBCOMM_DIR=libcomm
LIBCOMM_INCDIR=inc
LIBCOMM_BINDIR=bin
LIBCOMM_TMPDIR=build
LIBCOMM_OVERRIDES=CX=arm-cortex_a8-linux-gnueabi-g++ \
                  AR=arm-cortex_a8-linux-gnueabi-ar \
                  BIN_DIR=$(call libbin,$(LIBCOMM_BINDIR)) \
                  TMP_DIR=$(call libbin,$(LIBCOMM_TMPDIR))

.PHONY: libcomm_release
libcomm_release: force_look
	$(call libmake,,LIBCOMM,release)

.PHONY: libcomm_debug
libcomm_debug: force_look
	$(call libmake,,LIBCOMM,debug)

.PHONY: libcomm_clean
libcomm_clean: force_look
	$(call libmake,,LIBCOMM,clean)

.PHONY: libcomm_incdir
libcomm_incdir: force_look
	@echo $(LIBCOMM_DIR)/$(LIBCOMM_INCDIR)

.PHONY: libcomm_bindir
libcomm_bindir: force_look
	@echo $(LIBCOMM_DIR)/$(call libbin,$(LIBCOMM_BINDIR))



###############
### libcontrol ###
###############

LIBCOMM_DIR=libcontrol
LIBCOMM_INCDIR=inc
LIBCOMM_BINDIR=bin
LIBCOMM_TMPDIR=build
LIBCOMM_OVERRIDES=CX=arm-cortex_a8-linux-gnueabi-g++ \
                  AR=arm-cortex_a8-linux-gnueabi-ar \
                  BIN_DIR=$(call libbin,$(LIBCONTROL_BINDIR)) \
                  TMP_DIR=$(call libbin,$(LIBCONTROL_TMPDIR))

.PHONY: libcontrol_release
libcontrol_release: force_look
	$(call libmake,,LIBCONTROL,release)

.PHONY: libcontrol_debug
libcontrol_debug: force_look
	$(call libmake,,LIBCONTROL,debug)

.PHONY: libcontrol_clean
libcontrol_clean: force_look
	$(call libmake,,LIBCONTROL,clean)

.PHONY: libcontrol_incdir
libcontrol_incdir: force_look
	@echo $(LIBCONTROL_DIR)/$(LIBCONTROL_INCDIR)

.PHONY: libcontrol_bindir
libcontrol_bindir: force_look
	@echo $(LIBCONTROL_DIR)/$(call libbin,$(LIBCONTROL_BINDIR))
