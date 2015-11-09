$(PROGRAM): $(DEPENDENCIES) $(OBJECTS)
	@mkdir -p $(@D)
	$(call yellow,(AR)  $@)
	$(call invoke,yellow,$(AR) rcs $@ $(filter-out $(DEPENDENCIES),$^))

-include $(DEPS)

$(TMP_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(call green,(CX)  $<)
	$(call invoke,green,$(CX) $(CX_FLAGS) -c -MMD $< -o $@)

