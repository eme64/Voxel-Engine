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
CMAKE_SOURCE_DIR = /home/emanuel/Documents/voxel_engine/v1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/emanuel/Documents/voxel_engine/v1

# Include any dependencies generated for this target.
include thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/depend.make

# Include the progress variables for this target.
include thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/progress.make

# Include the compile flags for this target's objects.
include thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/flags.make

thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/clipboard.c.o: thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/flags.make
thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/clipboard.c.o: thirdparty/glfw-3.2.1/tests/clipboard.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/emanuel/Documents/voxel_engine/v1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/clipboard.c.o"
	cd /home/emanuel/Documents/voxel_engine/v1/thirdparty/glfw-3.2.1/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/clipboard.dir/clipboard.c.o   -c /home/emanuel/Documents/voxel_engine/v1/thirdparty/glfw-3.2.1/tests/clipboard.c

thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/clipboard.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/clipboard.dir/clipboard.c.i"
	cd /home/emanuel/Documents/voxel_engine/v1/thirdparty/glfw-3.2.1/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/emanuel/Documents/voxel_engine/v1/thirdparty/glfw-3.2.1/tests/clipboard.c > CMakeFiles/clipboard.dir/clipboard.c.i

thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/clipboard.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/clipboard.dir/clipboard.c.s"
	cd /home/emanuel/Documents/voxel_engine/v1/thirdparty/glfw-3.2.1/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/emanuel/Documents/voxel_engine/v1/thirdparty/glfw-3.2.1/tests/clipboard.c -o CMakeFiles/clipboard.dir/clipboard.c.s

thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/clipboard.c.o.requires:

.PHONY : thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/clipboard.c.o.requires

thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/clipboard.c.o.provides: thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/clipboard.c.o.requires
	$(MAKE) -f thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/build.make thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/clipboard.c.o.provides.build
.PHONY : thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/clipboard.c.o.provides

thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/clipboard.c.o.provides.build: thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/clipboard.c.o


thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/getopt.c.o: thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/flags.make
thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/getopt.c.o: thirdparty/glfw-3.2.1/deps/getopt.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/emanuel/Documents/voxel_engine/v1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/getopt.c.o"
	cd /home/emanuel/Documents/voxel_engine/v1/thirdparty/glfw-3.2.1/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/clipboard.dir/__/deps/getopt.c.o   -c /home/emanuel/Documents/voxel_engine/v1/thirdparty/glfw-3.2.1/deps/getopt.c

thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/getopt.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/clipboard.dir/__/deps/getopt.c.i"
	cd /home/emanuel/Documents/voxel_engine/v1/thirdparty/glfw-3.2.1/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/emanuel/Documents/voxel_engine/v1/thirdparty/glfw-3.2.1/deps/getopt.c > CMakeFiles/clipboard.dir/__/deps/getopt.c.i

thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/getopt.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/clipboard.dir/__/deps/getopt.c.s"
	cd /home/emanuel/Documents/voxel_engine/v1/thirdparty/glfw-3.2.1/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/emanuel/Documents/voxel_engine/v1/thirdparty/glfw-3.2.1/deps/getopt.c -o CMakeFiles/clipboard.dir/__/deps/getopt.c.s

thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/getopt.c.o.requires:

.PHONY : thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/getopt.c.o.requires

thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/getopt.c.o.provides: thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/getopt.c.o.requires
	$(MAKE) -f thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/build.make thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/getopt.c.o.provides.build
.PHONY : thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/getopt.c.o.provides

thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/getopt.c.o.provides.build: thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/getopt.c.o


thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/glad.c.o: thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/flags.make
thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/glad.c.o: thirdparty/glfw-3.2.1/deps/glad.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/emanuel/Documents/voxel_engine/v1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/glad.c.o"
	cd /home/emanuel/Documents/voxel_engine/v1/thirdparty/glfw-3.2.1/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/clipboard.dir/__/deps/glad.c.o   -c /home/emanuel/Documents/voxel_engine/v1/thirdparty/glfw-3.2.1/deps/glad.c

thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/clipboard.dir/__/deps/glad.c.i"
	cd /home/emanuel/Documents/voxel_engine/v1/thirdparty/glfw-3.2.1/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/emanuel/Documents/voxel_engine/v1/thirdparty/glfw-3.2.1/deps/glad.c > CMakeFiles/clipboard.dir/__/deps/glad.c.i

thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/clipboard.dir/__/deps/glad.c.s"
	cd /home/emanuel/Documents/voxel_engine/v1/thirdparty/glfw-3.2.1/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/emanuel/Documents/voxel_engine/v1/thirdparty/glfw-3.2.1/deps/glad.c -o CMakeFiles/clipboard.dir/__/deps/glad.c.s

thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/glad.c.o.requires:

.PHONY : thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/glad.c.o.requires

thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/glad.c.o.provides: thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/glad.c.o.requires
	$(MAKE) -f thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/build.make thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/glad.c.o.provides.build
.PHONY : thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/glad.c.o.provides

thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/glad.c.o.provides.build: thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/glad.c.o


# Object files for target clipboard
clipboard_OBJECTS = \
"CMakeFiles/clipboard.dir/clipboard.c.o" \
"CMakeFiles/clipboard.dir/__/deps/getopt.c.o" \
"CMakeFiles/clipboard.dir/__/deps/glad.c.o"

# External object files for target clipboard
clipboard_EXTERNAL_OBJECTS =

thirdparty/glfw-3.2.1/tests/clipboard: thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/clipboard.c.o
thirdparty/glfw-3.2.1/tests/clipboard: thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/getopt.c.o
thirdparty/glfw-3.2.1/tests/clipboard: thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/glad.c.o
thirdparty/glfw-3.2.1/tests/clipboard: thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/build.make
thirdparty/glfw-3.2.1/tests/clipboard: thirdparty/glfw-3.2.1/src/libglfw3.a
thirdparty/glfw-3.2.1/tests/clipboard: /usr/lib/x86_64-linux-gnu/librt.so
thirdparty/glfw-3.2.1/tests/clipboard: /usr/lib/x86_64-linux-gnu/libm.so
thirdparty/glfw-3.2.1/tests/clipboard: /usr/lib/x86_64-linux-gnu/libX11.so
thirdparty/glfw-3.2.1/tests/clipboard: /usr/lib/x86_64-linux-gnu/libXrandr.so
thirdparty/glfw-3.2.1/tests/clipboard: /usr/lib/x86_64-linux-gnu/libXinerama.so
thirdparty/glfw-3.2.1/tests/clipboard: /usr/lib/x86_64-linux-gnu/libXxf86vm.so
thirdparty/glfw-3.2.1/tests/clipboard: /usr/lib/x86_64-linux-gnu/libXcursor.so
thirdparty/glfw-3.2.1/tests/clipboard: thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/emanuel/Documents/voxel_engine/v1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable clipboard"
	cd /home/emanuel/Documents/voxel_engine/v1/thirdparty/glfw-3.2.1/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/clipboard.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/build: thirdparty/glfw-3.2.1/tests/clipboard

.PHONY : thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/build

thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/requires: thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/clipboard.c.o.requires
thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/requires: thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/getopt.c.o.requires
thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/requires: thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/__/deps/glad.c.o.requires

.PHONY : thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/requires

thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/clean:
	cd /home/emanuel/Documents/voxel_engine/v1/thirdparty/glfw-3.2.1/tests && $(CMAKE_COMMAND) -P CMakeFiles/clipboard.dir/cmake_clean.cmake
.PHONY : thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/clean

thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/depend:
	cd /home/emanuel/Documents/voxel_engine/v1 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/emanuel/Documents/voxel_engine/v1 /home/emanuel/Documents/voxel_engine/v1/thirdparty/glfw-3.2.1/tests /home/emanuel/Documents/voxel_engine/v1 /home/emanuel/Documents/voxel_engine/v1/thirdparty/glfw-3.2.1/tests /home/emanuel/Documents/voxel_engine/v1/thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : thirdparty/glfw-3.2.1/tests/CMakeFiles/clipboard.dir/depend

