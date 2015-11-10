# External dependencies rule names are dependency-<name>
release: DEPENDENCIES := $(addsuffix -release,$(addprefix dependency-,$(DEPENDS)))
debug:   DEPENDENCIES := $(addsuffix -debug,$(addprefix dependency-,$(DEPENDS)))

DEPENDENCIES_CLEAN = $(addsuffix -clean,$(addprefix dependency-,$(DEPENDS)))

#.PHONY: $(DEPENDENCIES)

dependency-%-release:
	$(call green,(DEP) $*)
	$(call invoke,white,$(MAKE) CROSS=$(CROSS) --no-print-directory $*_release -C $(LIB_DIR))

dependency-%-debug:
	$(call green,(DEP) $*)
	$(call invoke,white,$(MAKE) CROSS=$(CROSS) --no-print-directory $*_debug -C $(LIB_DIR))

# .PHONY: $(DEPENDENCIES_CLEAN)
dependency-%-clean:
	$(call blue,"==== Cleaning external dependency $*")
	$(call invoke,white,$(MAKE) CROSS=$(CROSS) --no-print-directory $*_clean -C $(LIB_DIR))

# Include dirs for external dependencies
DEPENDENCIES_INCDIRS = $(addprefix $(LIB_DIR)/,$(foreach DEP,$(DEPENDS),$(shell $(MAKE) CROSS=$(CROSS) --no-print-directory $(DEP)_incdir -C $(LIB_DIR))))

# Binary dirs for external dependencies
DEPENDENCIES_BINDIRS = $(addprefix $(LIB_DIR)/,$(foreach DEP,$(DEPENDS),$(shell $(MAKE) CROSS=$(CROSS) --no-print-directory $(DEP)_bindir -C $(LIB_DIR))))

# Turn them into flags (added in patch.mk)
DEPENDENCIES_INCFLAGS = $(addprefix -I,$(DEPENDENCIES_INCDIRS))
DEPENDENCIES_LDFLAGS  = $(addprefix -L,$(DEPENDENCIES_BINDIRS))

.PHONY: dependencies
dependencies:
	@if [[ "$(DEPENDENCIES)" != "" ]]; then $(MAKE) --no-print-directory $(DEPENDENCIES); fi
