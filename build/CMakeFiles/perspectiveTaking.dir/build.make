# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/human/Desktop/PerspectiveTaking

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/human/Desktop/PerspectiveTaking/build

# Include any dependencies generated for this target.
include CMakeFiles/perspectiveTaking.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/perspectiveTaking.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/perspectiveTaking.dir/flags.make

CMakeFiles/perspectiveTaking.dir/src/CRForestEstimator.cpp.o: CMakeFiles/perspectiveTaking.dir/flags.make
CMakeFiles/perspectiveTaking.dir/src/CRForestEstimator.cpp.o: ../src/CRForestEstimator.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/human/Desktop/PerspectiveTaking/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/perspectiveTaking.dir/src/CRForestEstimator.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/perspectiveTaking.dir/src/CRForestEstimator.cpp.o -c /home/human/Desktop/PerspectiveTaking/src/CRForestEstimator.cpp

CMakeFiles/perspectiveTaking.dir/src/CRForestEstimator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/perspectiveTaking.dir/src/CRForestEstimator.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/human/Desktop/PerspectiveTaking/src/CRForestEstimator.cpp > CMakeFiles/perspectiveTaking.dir/src/CRForestEstimator.cpp.i

CMakeFiles/perspectiveTaking.dir/src/CRForestEstimator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/perspectiveTaking.dir/src/CRForestEstimator.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/human/Desktop/PerspectiveTaking/src/CRForestEstimator.cpp -o CMakeFiles/perspectiveTaking.dir/src/CRForestEstimator.cpp.s

CMakeFiles/perspectiveTaking.dir/src/CRForestEstimator.cpp.o.requires:
.PHONY : CMakeFiles/perspectiveTaking.dir/src/CRForestEstimator.cpp.o.requires

CMakeFiles/perspectiveTaking.dir/src/CRForestEstimator.cpp.o.provides: CMakeFiles/perspectiveTaking.dir/src/CRForestEstimator.cpp.o.requires
	$(MAKE) -f CMakeFiles/perspectiveTaking.dir/build.make CMakeFiles/perspectiveTaking.dir/src/CRForestEstimator.cpp.o.provides.build
.PHONY : CMakeFiles/perspectiveTaking.dir/src/CRForestEstimator.cpp.o.provides

CMakeFiles/perspectiveTaking.dir/src/CRForestEstimator.cpp.o.provides.build: CMakeFiles/perspectiveTaking.dir/src/CRForestEstimator.cpp.o

CMakeFiles/perspectiveTaking.dir/src/CRTree.cpp.o: CMakeFiles/perspectiveTaking.dir/flags.make
CMakeFiles/perspectiveTaking.dir/src/CRTree.cpp.o: ../src/CRTree.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/human/Desktop/PerspectiveTaking/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/perspectiveTaking.dir/src/CRTree.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/perspectiveTaking.dir/src/CRTree.cpp.o -c /home/human/Desktop/PerspectiveTaking/src/CRTree.cpp

CMakeFiles/perspectiveTaking.dir/src/CRTree.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/perspectiveTaking.dir/src/CRTree.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/human/Desktop/PerspectiveTaking/src/CRTree.cpp > CMakeFiles/perspectiveTaking.dir/src/CRTree.cpp.i

CMakeFiles/perspectiveTaking.dir/src/CRTree.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/perspectiveTaking.dir/src/CRTree.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/human/Desktop/PerspectiveTaking/src/CRTree.cpp -o CMakeFiles/perspectiveTaking.dir/src/CRTree.cpp.s

CMakeFiles/perspectiveTaking.dir/src/CRTree.cpp.o.requires:
.PHONY : CMakeFiles/perspectiveTaking.dir/src/CRTree.cpp.o.requires

CMakeFiles/perspectiveTaking.dir/src/CRTree.cpp.o.provides: CMakeFiles/perspectiveTaking.dir/src/CRTree.cpp.o.requires
	$(MAKE) -f CMakeFiles/perspectiveTaking.dir/build.make CMakeFiles/perspectiveTaking.dir/src/CRTree.cpp.o.provides.build
.PHONY : CMakeFiles/perspectiveTaking.dir/src/CRTree.cpp.o.provides

CMakeFiles/perspectiveTaking.dir/src/CRTree.cpp.o.provides.build: CMakeFiles/perspectiveTaking.dir/src/CRTree.cpp.o

CMakeFiles/perspectiveTaking.dir/src/main.cpp.o: CMakeFiles/perspectiveTaking.dir/flags.make
CMakeFiles/perspectiveTaking.dir/src/main.cpp.o: ../src/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/human/Desktop/PerspectiveTaking/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/perspectiveTaking.dir/src/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/perspectiveTaking.dir/src/main.cpp.o -c /home/human/Desktop/PerspectiveTaking/src/main.cpp

CMakeFiles/perspectiveTaking.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/perspectiveTaking.dir/src/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/human/Desktop/PerspectiveTaking/src/main.cpp > CMakeFiles/perspectiveTaking.dir/src/main.cpp.i

CMakeFiles/perspectiveTaking.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/perspectiveTaking.dir/src/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/human/Desktop/PerspectiveTaking/src/main.cpp -o CMakeFiles/perspectiveTaking.dir/src/main.cpp.s

CMakeFiles/perspectiveTaking.dir/src/main.cpp.o.requires:
.PHONY : CMakeFiles/perspectiveTaking.dir/src/main.cpp.o.requires

CMakeFiles/perspectiveTaking.dir/src/main.cpp.o.provides: CMakeFiles/perspectiveTaking.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/perspectiveTaking.dir/build.make CMakeFiles/perspectiveTaking.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/perspectiveTaking.dir/src/main.cpp.o.provides

CMakeFiles/perspectiveTaking.dir/src/main.cpp.o.provides.build: CMakeFiles/perspectiveTaking.dir/src/main.cpp.o

# Object files for target perspectiveTaking
perspectiveTaking_OBJECTS = \
"CMakeFiles/perspectiveTaking.dir/src/CRForestEstimator.cpp.o" \
"CMakeFiles/perspectiveTaking.dir/src/CRTree.cpp.o" \
"CMakeFiles/perspectiveTaking.dir/src/main.cpp.o"

# External object files for target perspectiveTaking
perspectiveTaking_EXTERNAL_OBJECTS =

perspectiveTaking: CMakeFiles/perspectiveTaking.dir/src/CRForestEstimator.cpp.o
perspectiveTaking: CMakeFiles/perspectiveTaking.dir/src/CRTree.cpp.o
perspectiveTaking: CMakeFiles/perspectiveTaking.dir/src/main.cpp.o
perspectiveTaking: /usr/lib/x86_64-linux-gnu/libGLU.so
perspectiveTaking: /usr/lib/x86_64-linux-gnu/libGL.so
perspectiveTaking: /usr/lib/x86_64-linux-gnu/libSM.so
perspectiveTaking: /usr/lib/x86_64-linux-gnu/libICE.so
perspectiveTaking: /usr/lib/x86_64-linux-gnu/libX11.so
perspectiveTaking: /usr/lib/x86_64-linux-gnu/libXext.so
perspectiveTaking: /usr/lib/x86_64-linux-gnu/libglut.so
perspectiveTaking: /usr/lib/x86_64-linux-gnu/libXmu.so
perspectiveTaking: /usr/lib/x86_64-linux-gnu/libXi.so
perspectiveTaking: CMakeFiles/perspectiveTaking.dir/build.make
perspectiveTaking: CMakeFiles/perspectiveTaking.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable perspectiveTaking"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/perspectiveTaking.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/perspectiveTaking.dir/build: perspectiveTaking
.PHONY : CMakeFiles/perspectiveTaking.dir/build

CMakeFiles/perspectiveTaking.dir/requires: CMakeFiles/perspectiveTaking.dir/src/CRForestEstimator.cpp.o.requires
CMakeFiles/perspectiveTaking.dir/requires: CMakeFiles/perspectiveTaking.dir/src/CRTree.cpp.o.requires
CMakeFiles/perspectiveTaking.dir/requires: CMakeFiles/perspectiveTaking.dir/src/main.cpp.o.requires
.PHONY : CMakeFiles/perspectiveTaking.dir/requires

CMakeFiles/perspectiveTaking.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/perspectiveTaking.dir/cmake_clean.cmake
.PHONY : CMakeFiles/perspectiveTaking.dir/clean

CMakeFiles/perspectiveTaking.dir/depend:
	cd /home/human/Desktop/PerspectiveTaking/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/human/Desktop/PerspectiveTaking /home/human/Desktop/PerspectiveTaking /home/human/Desktop/PerspectiveTaking/build /home/human/Desktop/PerspectiveTaking/build /home/human/Desktop/PerspectiveTaking/build/CMakeFiles/perspectiveTaking.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/perspectiveTaking.dir/depend

