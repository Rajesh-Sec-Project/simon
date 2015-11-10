TESTS_SOURCES=$(wildcard $(TST_DIR)/*.cpp)
TESTS_OBJECTS=$(filter-out $(TMP_DIR)/main.o,$(OBJECTS))
TESTS=$(patsubst $(TST_DIR)/%.cpp,test-%,$(TESTS_SOURCES))
TESTS_BUILD=$(addsuffix -build,$(TESTS))
TESTS_RUN=$(addsuffix -run,$(TESTS))

.PHONY: pre-tests-build pre-tests-run post-tests
pre-tests-build:
	$(call blue,==== Building tests)

pre-tests-run:
	$(call blue,==== Running tests)

post-tests:
	$(call blue,==== Done with testing)

.PHONY: tests tests-debug tests-release
in-tests: pre-tests-build $(TESTS_BUILD) pre-tests-run $(TESTS_RUN) post-tests
tests-debug:   debug   in-tests
tests-release: release in-tests

test-%-build: $(TMP_DIR)/$(TST_DIR)/%.o
	@mkdir -p $(BIN_DIR)/$(TST_DIR)
	$(call yellow,(LD)  $*)
	$(call invoke,yellow,$(LD) $(TESTS_OBJECTS) $< -o $(BIN_DIR)/$(TST_DIR)/$* $(LD_FLAGS))

test-%-run:
	$(call yellow,(TST) $*)
	$(call invoke,yellow,$(QEMU_ARM) -L '$(QEMU_SYSROOT)' $(BIN_DIR)/$(TST_DIR)/$*)

.PRECIOUS: $(TMP_DIR)/$(TST_DIR)/%.o
$(TMP_DIR)/$(TST_DIR)/%.o: $(TST_DIR)/%.cpp
	@mkdir -p $(@D)
	$(call green,(CX)  $<)
	$(call invoke,green,$(CX) $(CX_FLAGS) -c -MMD $< -o $@)
