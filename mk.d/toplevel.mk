#######################
### Top-level rules ###
#######################

all: debug
debug release: post-build

.PHONY: pre-build
pre-build:
	$(call blue,"==== Building project $(PROJECT)")

binaries: pre-build $(PROGRAM)

.PHONY: post-build
post-build: binaries
	$(call blue,"==== Build successful")

###################
### Phony rules ###
###################

.PHONY: clean
clean:
	$(call blue,"==== Cleaning project $(PROJECT)")
	@rm -rf $(TMP_DIR) $(BIN_DIR)

.PHONY: mk_debug
mk_debug:
	@printf '-- Sources --\n'
	@printf "SUBDIRS=$(SUBDIRS)\n"
	@printf "SOURCES=$(SOURCES)\n"
	@printf "OBJECTS=$(OBJECTS)\n"
	@printf "DEPS=$(DEPS)\n"
	@printf '-- Flags --\n'
	@printf "CX_FLAGS=$(CX_FLAGS)\n"
	@printf "LD_FLAGS=$(LD_FLAGS)\n"
