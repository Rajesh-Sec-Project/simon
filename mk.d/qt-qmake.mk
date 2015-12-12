#####################
### QMake targets ###
#####################

.PHONY: qmake
qmake:
	@mkdir -p $(TMP_DIR)
	$(call green,(QMAKE) $(PRODUCT))
	$(call invoke,green,cd $(TMP_DIR); \
	qmake ../$(PRO_FILE) -r -spec linux-g++ $(QMAKE_FLAGS))

.PHONY: qbuild
qbuild:
	$(call green,(MAKE)  $(PRODUCT))
	$(call invoke,green,make --no-print-directory -C $(TMP_DIR))

.PHONY: qclean
qclean:
	$(call blue,"==== Cleaning Qt project $(PRODUCT)")
	$(call invoke,blue,$(MAKE) clean --no-print-directory -C $(TMP_DIR))
