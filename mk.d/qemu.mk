sim-%: %
	$(call blue,"==== Starting QEmu for project $(PROJECT)")
	@$(QEMU_ARM) -L '$(QEMU_SYSROOT)' $(PROGRAM)
	$(call blue,"==== QEmu session stopped")
