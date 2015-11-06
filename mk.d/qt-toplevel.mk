#################
### Top-level ###
#################

release: QMAKE_FLAGS += config+=RELEASE
debug:   QMAKE_FLAGS += config+=DEBUG

debug release: post-build

pre-build:
	$(call blue,"==== Building project $(PROJECT)")

$(BINARY): pre-build $(DEPENDENCIES) qmake qbuild
	@mkdir -p $(@D)
	$(call yellow,(CP)    $@)
	$(call invoke,yellow,cp $(TMP_DIR)/$(PRODUCT) $@)

post-build: $(BINARY)
	$(call blue,"==== Build suceeded")

.PHONY: help
help:
	$(call blue,"==== Help to build project $(PROJECT)")
	$(call white,     debug release: build project (qmake, qbuild and qdist))
	$(call white,     clean:         remove qmake directory '$(TMP_DIR)' and '$(BIN_DIR)')
	$(call white,     qmake:         run qmake (creates directory '$(TMP_DIR)'))
	$(call white,     qbuild:        build qmake'd project)
	$(call white,     qclean:        clean qmake'd project)

.PHONY: clean
clean:
	$(call blue,"==== Cleaning project $(PROJECT)")
	@rm -rf $(TMP_DIR) $(BIN_DIR)

.PHONY: depclean
depclean: $(DEPENDENCIES_CLEAN)
