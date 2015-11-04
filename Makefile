include mk.d/colors.mk

SUBDIRS:=$(filter-out mk.d/,$(wildcard */))
SUBDIRS:=$(filter-out debug/,$(SUBDIRS))

define descend
@for dir in $(SUBDIRS); do (cd $$dir; $(MAKE) --no-print-directory $1); done
endef

.PHONY: release
release: force_look
	$(call descend,release)

.PHONY: debug
debug: force_look
	$(call descend,debug)

.PHONY: clean
clean: force_look
	$(call descend,clean)

force_look:
	@true
