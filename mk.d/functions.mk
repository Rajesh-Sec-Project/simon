ifeq ($(VERBOSE),1)
define invoke
@echo "      -> $2"
@$2;
endef
else
define invoke
@$2 1> /dev/null;
endef
endif
