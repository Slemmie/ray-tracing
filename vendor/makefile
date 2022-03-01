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

# check for missing bits
.PHONY: glfw_error_check
glfw_error_check:
	@if [ ! -d "./glfw/" ]; then         \
		echo "! glfw submodule missing"; \
		false;                           \
	fi;
	@echo "OK"
