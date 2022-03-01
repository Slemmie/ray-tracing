# makefile for building and/or exporting vendor modules

################
### [ glfw ] ###
################

## utiliy

# reset submodule to its original state
.PHONY: glfw_reset_submodule
glfw_reset_submodule:
	git submodule deinit -f ./glfw/
	git submodule update --init ./glfw/
