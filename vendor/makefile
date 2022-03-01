# makefile for building and/or exporting vendor modules
# exports to ./bin/...

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

## shared library

# main build
.PHONY: glfw_shared
glfw_shared:
	# prepare
	make glfw_error_check
	make glfw_reset_submodule
	if [ ! -d "./bin/glfw/shared/" ]; then mkdir -p ./bin/glfw/shared/; fi;
	
	# build
	mkdir ./glfw/build/ && cmake -S ./glfw/ -B ./glfw/build/ -DBUILD_SHARED_LIBS=ON
	cd ./glfw/build/ && make glfw
	
	# export
	cp ./glfw/build/src/libglfw.so.3 ./bin/glfw/shared/libglfw.so.3
	
	# reset submodule
	make glfw_reset_submodule
	
	@echo "OK"

# export header
.PHONY: glfw_header
glfw_header:
	# prepare
	make glfw_error_check
	make glfw_reset_submodule
	if [ ! -d "./bin/glfw/include/" ]; then mkdir -p ./bin/glfw/include/; fi;
	
	# export
	cp ./glfw/include/GLFW/glfw3.h ./bin/glfw/include/glfw3.h
	
	# reset submodule
	make glfw_reset_submodule
	
	@echo "OK"
