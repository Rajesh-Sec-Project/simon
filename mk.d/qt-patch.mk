###############
### Patches ###
##############

PRO_FILE=$(wildcard $(PRODUCT)/*.pro)
BINARY=$(BIN_DIR)/$(PRODUCT)
DEPENDENCIES_LDFLAGS := $(patsubst -L%,-L../%,$(DEPENDENCIES_LDFLAGS))
QMAKE_FLAGS+=INCLUDEPATH+='$(DEPENDENCIES_INCDIRS)' LIBS+='$(DEPENDENCIES_LDFLAGS)'
