ifeq ($(VERBOSE),1)
define invoke
@$2;
@echo "      -> $2"
endef
else
define invoke
@$2;
endef
endif
