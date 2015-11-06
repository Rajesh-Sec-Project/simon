include mk.d/colors.mk

SUBDIRS:=$(filter-out mk.d/,$(wildcard */))
SUBDIRS:=$(filter-out debug/,$(SUBDIRS))

define descend
@for dir in $(SUBDIRS); do (cd $$dir; $(MAKE) --no-print-directory $1); done
endef

all: debug

release: force_look
	$(call descend,release)

debug: force_look
	$(call descend,debug)

clean: force_look
	$(call descend,clean)

force_look:
	@true
