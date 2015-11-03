###########################################
### Color definitions for sub Makefiles ###
###########################################

define colored
@tput setaf $1;
@echo "$2";
@tput sgr0;
endef

define green
$(call colored,2,$1)
endef

define blue
$(call colored,4,$1)
endef

define red
$(call colored,1,$1)
endef

define yellow
$(call colored,3,$1)
endef
