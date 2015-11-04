##########################
### Remote target only ###
##########################

push: post-build
	$(call yellow,"==== Pushing binary to remote")
	$(call invoke,yellow,export PREFIX="$(shell pwd)" && cd ../debug && ./push.sh)

gdb: push
	$(call yellow,"==== Starting debugging session")
	@export PREFIX="$(shell pwd)" && cd ../debug && ./debug.sh
	$(call yellow,"==== Debugging session ended")

.PHONY: shell
shell:
	$(call yellow,"==== Starting remote shell")
	@-export PREFIX="$(shell pwd)" && cd ../debug && ./shell.sh
	$(call yellow,"==== Remote shell exited")
