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
include thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/depend.make

# Include the progress variables for this target.
include thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/progress.make

# Include the compile flags for this target's objects.
include thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/flags.make

thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/particles.c.o: thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/flags.make
thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/particles.c.o: ../../thirdparty/glfw-3.2.1/examples/particles.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/emanuel/Documents/Voxel-Engine/v1/build/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/particles.c.o"
	cd /home/emanuel/Documents/Voxel-Engine/v1/build/Release/thirdparty/glfw-3.2.1/examples && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/particles.dir/particles.c.o   -c /home/emanuel/Documents/Voxel-Engine/v1/thirdparty/glfw-3.2.1/examples/particles.c

thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/particles.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/particles.dir/particles.c.i"
	cd /home/emanuel/Documents/Voxel-Engine/v1/build/Release/thirdparty/glfw-3.2.1/examples && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/emanuel/Documents/Voxel-Engine/v1/thirdparty/glfw-3.2.1/examples/particles.c > CMakeFiles/particles.dir/particles.c.i

thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/particles.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/particles.dir/particles.c.s"
	cd /home/emanuel/Documents/Voxel-Engine/v1/build/Release/thirdparty/glfw-3.2.1/examples && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/emanuel/Documents/Voxel-Engine/v1/thirdparty/glfw-3.2.1/examples/particles.c -o CMakeFiles/particles.dir/particles.c.s

thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/particles.c.o.requires:

.PHONY : thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/particles.c.o.requires

thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/particles.c.o.provides: thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/particles.c.o.requires
	$(MAKE) -f thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/build.make thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/particles.c.o.provides.build
.PHONY : thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/particles.c.o.provides

thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/particles.c.o.provides.build: thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/particles.c.o


thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/tinycthread.c.o: thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/flags.make
thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/tinycthread.c.o: ../../thirdparty/glfw-3.2.1/deps/tinycthread.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/emanuel/Documents/Voxel-Engine/v1/build/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/tinycthread.c.o"
	cd /home/emanuel/Documents/Voxel-Engine/v1/build/Release/thirdparty/glfw-3.2.1/examples && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/particles.dir/__/deps/tinycthread.c.o   -c /home/emanuel/Documents/Voxel-Engine/v1/thirdparty/glfw-3.2.1/deps/tinycthread.c

thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/tinycthread.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/particles.dir/__/deps/tinycthread.c.i"
	cd /home/emanuel/Documents/Voxel-Engine/v1/build/Release/thirdparty/glfw-3.2.1/examples && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/emanuel/Documents/Voxel-Engine/v1/thirdparty/glfw-3.2.1/deps/tinycthread.c > CMakeFiles/particles.dir/__/deps/tinycthread.c.i

thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/tinycthread.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/particles.dir/__/deps/tinycthread.c.s"
	cd /home/emanuel/Documents/Voxel-Engine/v1/build/Release/thirdparty/glfw-3.2.1/examples && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/emanuel/Documents/Voxel-Engine/v1/thirdparty/glfw-3.2.1/deps/tinycthread.c -o CMakeFiles/particles.dir/__/deps/tinycthread.c.s

thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/tinycthread.c.o.requires:

.PHONY : thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/tinycthread.c.o.requires

thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/tinycthread.c.o.provides: thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/tinycthread.c.o.requires
	$(MAKE) -f thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/build.make thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/tinycthread.c.o.provides.build
.PHONY : thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/tinycthread.c.o.provides

thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/tinycthread.c.o.provides.build: thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/tinycthread.c.o


thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/getopt.c.o: thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/flags.make
thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/getopt.c.o: ../../thirdparty/glfw-3.2.1/deps/getopt.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/emanuel/Documents/Voxel-Engine/v1/build/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/getopt.c.o"
	cd /home/emanuel/Documents/Voxel-Engine/v1/build/Release/thirdparty/glfw-3.2.1/examples && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/particles.dir/__/deps/getopt.c.o   -c /home/emanuel/Documents/Voxel-Engine/v1/thirdparty/glfw-3.2.1/deps/getopt.c

thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/getopt.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/particles.dir/__/deps/getopt.c.i"
	cd /home/emanuel/Documents/Voxel-Engine/v1/build/Release/thirdparty/glfw-3.2.1/examples && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/emanuel/Documents/Voxel-Engine/v1/thirdparty/glfw-3.2.1/deps/getopt.c > CMakeFiles/particles.dir/__/deps/getopt.c.i

thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/getopt.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/particles.dir/__/deps/getopt.c.s"
	cd /home/emanuel/Documents/Voxel-Engine/v1/build/Release/thirdparty/glfw-3.2.1/examples && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/emanuel/Documents/Voxel-Engine/v1/thirdparty/glfw-3.2.1/deps/getopt.c -o CMakeFiles/particles.dir/__/deps/getopt.c.s

thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/getopt.c.o.requires:

.PHONY : thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/getopt.c.o.requires

thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/getopt.c.o.provides: thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/getopt.c.o.requires
	$(MAKE) -f thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/build.make thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/getopt.c.o.provides.build
.PHONY : thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/getopt.c.o.provides

thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/getopt.c.o.provides.build: thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/getopt.c.o


thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/glad.c.o: thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/flags.make
thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/glad.c.o: ../../thirdparty/glfw-3.2.1/deps/glad.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/emanuel/Documents/Voxel-Engine/v1/build/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/glad.c.o"
	cd /home/emanuel/Documents/Voxel-Engine/v1/build/Release/thirdparty/glfw-3.2.1/examples && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/particles.dir/__/deps/glad.c.o   -c /home/emanuel/Documents/Voxel-Engine/v1/thirdparty/glfw-3.2.1/deps/glad.c

thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/particles.dir/__/deps/glad.c.i"
	cd /home/emanuel/Documents/Voxel-Engine/v1/build/Release/thirdparty/glfw-3.2.1/examples && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/emanuel/Documents/Voxel-Engine/v1/thirdparty/glfw-3.2.1/deps/glad.c > CMakeFiles/particles.dir/__/deps/glad.c.i

thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/particles.dir/__/deps/glad.c.s"
	cd /home/emanuel/Documents/Voxel-Engine/v1/build/Release/thirdparty/glfw-3.2.1/examples && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/emanuel/Documents/Voxel-Engine/v1/thirdparty/glfw-3.2.1/deps/glad.c -o CMakeFiles/particles.dir/__/deps/glad.c.s

thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/glad.c.o.requires:

.PHONY : thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/glad.c.o.requires

thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/glad.c.o.provides: thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/glad.c.o.requires
	$(MAKE) -f thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/build.make thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/glad.c.o.provides.build
.PHONY : thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/glad.c.o.provides

thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/glad.c.o.provides.build: thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/glad.c.o


# Object files for target particles
particles_OBJECTS = \
"CMakeFiles/particles.dir/particles.c.o" \
"CMakeFiles/particles.dir/__/deps/tinycthread.c.o" \
"CMakeFiles/particles.dir/__/deps/getopt.c.o" \
"CMakeFiles/particles.dir/__/deps/glad.c.o"

# External object files for target particles
particles_EXTERNAL_OBJECTS =

thirdparty/glfw-3.2.1/examples/particles: thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/particles.c.o
thirdparty/glfw-3.2.1/examples/particles: thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/tinycthread.c.o
thirdparty/glfw-3.2.1/examples/particles: thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/getopt.c.o
thirdparty/glfw-3.2.1/examples/particles: thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/glad.c.o
thirdparty/glfw-3.2.1/examples/particles: thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/build.make
thirdparty/glfw-3.2.1/examples/particles: thirdparty/glfw-3.2.1/src/libglfw3.a
thirdparty/glfw-3.2.1/examples/particles: /usr/lib/x86_64-linux-gnu/librt.so
thirdparty/glfw-3.2.1/examples/particles: /usr/lib/x86_64-linux-gnu/libm.so
thirdparty/glfw-3.2.1/examples/particles: /usr/lib/x86_64-linux-gnu/libX11.so
thirdparty/glfw-3.2.1/examples/particles: /usr/lib/x86_64-linux-gnu/libXrandr.so
thirdparty/glfw-3.2.1/examples/particles: /usr/lib/x86_64-linux-gnu/libXinerama.so
thirdparty/glfw-3.2.1/examples/particles: /usr/lib/x86_64-linux-gnu/libXxf86vm.so
thirdparty/glfw-3.2.1/examples/particles: /usr/lib/x86_64-linux-gnu/libXcursor.so
thirdparty/glfw-3.2.1/examples/particles: thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/emanuel/Documents/Voxel-Engine/v1/build/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable particles"
	cd /home/emanuel/Documents/Voxel-Engine/v1/build/Release/thirdparty/glfw-3.2.1/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/particles.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/build: thirdparty/glfw-3.2.1/examples/particles

.PHONY : thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/build

thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/requires: thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/particles.c.o.requires
thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/requires: thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/tinycthread.c.o.requires
thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/requires: thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/getopt.c.o.requires
thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/requires: thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/__/deps/glad.c.o.requires

.PHONY : thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/requires

thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/clean:
	cd /home/emanuel/Documents/Voxel-Engine/v1/build/Release/thirdparty/glfw-3.2.1/examples && $(CMAKE_COMMAND) -P CMakeFiles/particles.dir/cmake_clean.cmake
.PHONY : thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/clean

thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/depend:
	cd /home/emanuel/Documents/Voxel-Engine/v1/build/Release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/emanuel/Documents/Voxel-Engine/v1 /home/emanuel/Documents/Voxel-Engine/v1/thirdparty/glfw-3.2.1/examples /home/emanuel/Documents/Voxel-Engine/v1/build/Release /home/emanuel/Documents/Voxel-Engine/v1/build/Release/thirdparty/glfw-3.2.1/examples /home/emanuel/Documents/Voxel-Engine/v1/build/Release/thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : thirdparty/glfw-3.2.1/examples/CMakeFiles/particles.dir/depend

