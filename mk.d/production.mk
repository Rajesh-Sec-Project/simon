$(PROGRAM): dependencies $(OBJECTS)
	@mkdir -p $(@D)
	$(call yellow,(LD)  $@)
	$(call invoke,yellow,$(LD) $(filter-out dependencies,$^) -o $@ $(LD_FLAGS))

-include $(DEPS)

$(TMP_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(call green,(CX)  $<)
	$(call invoke,green,$(CX) $(CX_FLAGS) -c -MMD $< -o $@)

