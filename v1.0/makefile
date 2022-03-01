# for building v1.0 release/debug

# compilation flags
CXX_FLAGS = -Wshadow -Wall -std=c++17
CXX_RELEASE_FLAGS = -O2
CXX_DEBUG_FLAGS = -g -fsanitize=address -fsanitize=undefined -D_GLIBCXX_DEBUG

# main target just builds release
all: release

# all cpp source files
SOURCE_FILES =                 \
$(wildcard ./src/*.c)          \
$(wildcard ./src/graphics/*.c)

# all object files
OBJECT_FILES = ${SOURCE_FILES:.c=.o}

release:
	
