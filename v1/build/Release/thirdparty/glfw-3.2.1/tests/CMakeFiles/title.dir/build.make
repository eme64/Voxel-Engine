# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/emanuel/Documents/Voxel-Engine/v1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/emanuel/Documents/Voxel-Engine/v1/build/Release

# Include any dependencies generated for this target.
include thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/depend.make

# Include the progress variables for this target.
include thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/progress.make

# Include the compile flags for this target's objects.
include thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/flags.make

thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/title.c.o: thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/flags.make
thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/title.c.o: ../../thirdparty/glfw-3.2.1/tests/title.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/emanuel/Documents/Voxel-Engine/v1/build/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/title.c.o"
	cd /home/emanuel/Documents/Voxel-Engine/v1/build/Release/thirdparty/glfw-3.2.1/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/title.dir/title.c.o   -c /home/emanuel/Documents/Voxel-Engine/v1/thirdparty/glfw-3.2.1/tests/title.c

thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/title.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/title.dir/title.c.i"
	cd /home/emanuel/Documents/Voxel-Engine/v1/build/Release/thirdparty/glfw-3.2.1/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/emanuel/Documents/Voxel-Engine/v1/thirdparty/glfw-3.2.1/tests/title.c > CMakeFiles/title.dir/title.c.i

thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/title.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/title.dir/title.c.s"
	cd /home/emanuel/Documents/Voxel-Engine/v1/build/Release/thirdparty/glfw-3.2.1/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/emanuel/Documents/Voxel-Engine/v1/thirdparty/glfw-3.2.1/tests/title.c -o CMakeFiles/title.dir/title.c.s

thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/title.c.o.requires:

.PHONY : thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/title.c.o.requires

thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/title.c.o.provides: thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/title.c.o.requires
	$(MAKE) -f thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/build.make thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/title.c.o.provides.build
.PHONY : thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/title.c.o.provides

thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/title.c.o.provides.build: thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/title.c.o


thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/__/deps/glad.c.o: thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/flags.make
thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/__/deps/glad.c.o: ../../thirdparty/glfw-3.2.1/deps/glad.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/emanuel/Documents/Voxel-Engine/v1/build/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/__/deps/glad.c.o"
	cd /home/emanuel/Documents/Voxel-Engine/v1/build/Release/thirdparty/glfw-3.2.1/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/title.dir/__/deps/glad.c.o   -c /home/emanuel/Documents/Voxel-Engine/v1/thirdparty/glfw-3.2.1/deps/glad.c

thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/__/deps/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/title.dir/__/deps/glad.c.i"
	cd /home/emanuel/Documents/Voxel-Engine/v1/build/Release/thirdparty/glfw-3.2.1/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/emanuel/Documents/Voxel-Engine/v1/thirdparty/glfw-3.2.1/deps/glad.c > CMakeFiles/title.dir/__/deps/glad.c.i

thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/__/deps/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/title.dir/__/deps/glad.c.s"
	cd /home/emanuel/Documents/Voxel-Engine/v1/build/Release/thirdparty/glfw-3.2.1/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/emanuel/Documents/Voxel-Engine/v1/thirdparty/glfw-3.2.1/deps/glad.c -o CMakeFiles/title.dir/__/deps/glad.c.s

thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/__/deps/glad.c.o.requires:

.PHONY : thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/__/deps/glad.c.o.requires

thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/__/deps/glad.c.o.provides: thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/__/deps/glad.c.o.requires
	$(MAKE) -f thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/build.make thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/__/deps/glad.c.o.provides.build
.PHONY : thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/__/deps/glad.c.o.provides

thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/__/deps/glad.c.o.provides.build: thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/__/deps/glad.c.o


# Object files for target title
title_OBJECTS = \
"CMakeFiles/title.dir/title.c.o" \
"CMakeFiles/title.dir/__/deps/glad.c.o"

# External object files for target title
title_EXTERNAL_OBJECTS =

thirdparty/glfw-3.2.1/tests/title: thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/title.c.o
thirdparty/glfw-3.2.1/tests/title: thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/__/deps/glad.c.o
thirdparty/glfw-3.2.1/tests/title: thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/build.make
thirdparty/glfw-3.2.1/tests/title: thirdparty/glfw-3.2.1/src/libglfw3.a
thirdparty/glfw-3.2.1/tests/title: /usr/lib/x86_64-linux-gnu/librt.so
thirdparty/glfw-3.2.1/tests/title: /usr/lib/x86_64-linux-gnu/libm.so
thirdparty/glfw-3.2.1/tests/title: /usr/lib/x86_64-linux-gnu/libX11.so
thirdparty/glfw-3.2.1/tests/title: /usr/lib/x86_64-linux-gnu/libXrandr.so
thirdparty/glfw-3.2.1/tests/title: /usr/lib/x86_64-linux-gnu/libXinerama.so
thirdparty/glfw-3.2.1/tests/title: /usr/lib/x86_64-linux-gnu/libXxf86vm.so
thirdparty/glfw-3.2.1/tests/title: /usr/lib/x86_64-linux-gnu/libXcursor.so
thirdparty/glfw-3.2.1/tests/title: thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/emanuel/Documents/Voxel-Engine/v1/build/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable title"
	cd /home/emanuel/Documents/Voxel-Engine/v1/build/Release/thirdparty/glfw-3.2.1/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/title.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/build: thirdparty/glfw-3.2.1/tests/title

.PHONY : thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/build

thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/requires: thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/title.c.o.requires
thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/requires: thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/__/deps/glad.c.o.requires

.PHONY : thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/requires

thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/clean:
	cd /home/emanuel/Documents/Voxel-Engine/v1/build/Release/thirdparty/glfw-3.2.1/tests && $(CMAKE_COMMAND) -P CMakeFiles/title.dir/cmake_clean.cmake
.PHONY : thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/clean

thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/depend:
	cd /home/emanuel/Documents/Voxel-Engine/v1/build/Release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/emanuel/Documents/Voxel-Engine/v1 /home/emanuel/Documents/Voxel-Engine/v1/thirdparty/glfw-3.2.1/tests /home/emanuel/Documents/Voxel-Engine/v1/build/Release /home/emanuel/Documents/Voxel-Engine/v1/build/Release/thirdparty/glfw-3.2.1/tests /home/emanuel/Documents/Voxel-Engine/v1/build/Release/thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : thirdparty/glfw-3.2.1/tests/CMakeFiles/title.dir/depend

