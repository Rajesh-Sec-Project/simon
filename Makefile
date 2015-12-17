include mk.d/colors.mk

SUBDIRS:=$(filter-out $(wildcard *.d/),$(wildcard */))

define descend
@for dir in $(SUBDIRS); do ($(MAKE) $1 --no-print-directory -C $$dir); done
endef

all:
	$(call red,Please specify a target to build)

release: force_look
	$(call descend,release)

debug: force_look
	$(call descend,debug)

clean: force_look
	$(call descend,depclean)
	$(call descend,clean)

force_look:
	@true
