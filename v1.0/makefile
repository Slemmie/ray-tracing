# for building v1.0 release/debug

# compilation flags
CXX_FLAGS = -Wshadow -Wall -std=c++17
CXX_RELEASE_FLAGS = -O2
CXX_DEBUG_FLAGS = -g -fsanitize=address -fsanitize=undefined -D_GLIBCXX_DEBUG

# main target just builds release
all: release

# all cpp source files
SOURCE_FILES =                   \
$(wildcard ./src/*.cpp)          \
$(wildcard ./src/graphics/*.cpp) \
$(wildcard ./src/util/*.cpp)

# all header files
HEADER_FILES =                 \
$(wildcard ./src/*.h)          \
$(wildcard ./src/graphics/*.h) \
$(wildcard ./src/util/*.h)

# all object files
OBJECT_FILES = ${SOURCE_FILES:.cpp=.o}

# seperate debug object files
DEBUG_OBJECT_FILES = ${SOURCE_FILES:.cpp=_debug.o}

# vendor binaries/headers directory path
VENDOR_PATH = $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))/../vendor/bin/

# links
LINKS = \
-L $(VENDOR_PATH)glfw/shared/ -Wl,-rpath=$(VENDOR_PATH)glfw/shared/ $(VENDOR_PATH)glfw/shared/libglfw.so.3   \
-L $(VENDOR_PATH)glew/shared/ -Wl,-rpath=$(VENDOR_PATH)glew/shared/ $(VENDOR_PATH)glew/shared/libGLEW.so.2.2 \
-lGL -lpthread

# main release target
release: ${OBJECT_FILES}
	g++ $(CXX_FLAGS) $^ -o $@ $(CXX_RELEASE_FLAGS) $(LINKS)
	make clean

# general target for creating object files
%.o: %.cpp ${HEADER_FILES}
	g++ $(CXX_FLAGS) -c $< -o $@ $(CXX_RELEASE_FLAGS) -I $(VENDOR_PATH)

# main debug target
debug: ${DEBUG_OBJECT_FILES}
	g++ $(CXX_FLAGS) $^ -o $@ $(CXX_DEBUG_FLAGS) $(LINKS)
	make clean

# general DEBUG target for creating object files
%_debug.o: %.cpp ${HEADER_FILES}
	g++ $(CXX_FLAGS) -c $< -o $@ $(CXX_DEBUG_FLAGS) -I $(VENDOR_PATH)

# cleaning up temporary files
.PHONY: clean
clean:
	find . -name "*.o" -type f -delete
