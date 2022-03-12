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

################
### [ glew ] ###
################

## utiliy

# reset submodule to its original state
.PHONY: glew_reset_submodule
glew_reset_submodule:
	git submodule deinit -f ./glew/
	git submodule update --init ./glew/

# check for missing bits
.PHONY: glew_error_check
glew_error_check:
	@if [ ! -d "./glew/" ]; then         \
		echo "! glew submodule missing"; \
		false;                           \
	fi;

## shared library

# main build
# glew generates the header based on the build; also export header from here
.PHONY: glew_shared
glew_shared:
	# prepare
	make glew_error_check
	make glew_reset_submodule
	if [ ! -d "./bin/glew/shared/" ]; then mkdir -p ./bin/glew/shared/; fi;
	if [ ! -d "./bin/glew/include/" ]; then mkdir -p ./bin/glew/include/; fi;
	
	# build
	cd ./glew/auto/ && make
	cd ./glew/ && make
	
	# export
	cp ./glew/lib/libGLEW.so.2.2 ./bin/glew/shared/libGLEW.so.2.2
	cp ./glew/include/GL/glew.h ./bin/glew/include/glew.h
	
	# reset submodule
	make glew_reset_submodule
	
	@echo "OK"

###############
### [ glm ] ###
###############

## utiliy

# reset submodule to its original state
.PHONY: glm_reset_submodule
glm_reset_submodule:
	git submodule deinit -f ./glm/
	git submodule update --init ./glm/

# check for missing bits
.PHONY: glm_error_check
glm_error_check:
	@if [ ! -d "./glm/" ]; then         \
		echo "! glm submodule missing"; \
		false;                          \
	fi;

# export headers
.PHONY: glm_headers
glm_headers:
	# prepare
	make glm_error_check
	make glm_reset_submodule
	if [ ! -d "./bin/glm/include/" ]; then mkdir -p ./bin/glm/include/; fi;
	
	# export
	cp -r ./glm/glm/* ./bin/glm/include/
	
	# reset submodule
	make glm_reset_submodule
	
	@echo "OK"

###############
### [ stb ] ###
###############

## utiliy

# reset submodule to its original state
.PHONY: stb_reset_submodule
stb_reset_submodule:
	git submodule deinit -f ./stb/
	git submodule update --init ./stb/
