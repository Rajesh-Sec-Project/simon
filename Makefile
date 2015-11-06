include mk.d/colors.mk

SUBDIRS:=$(filter-out $(wildcard *.d/),$(wildcard */))

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
