# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/anna/Yadro

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/anna/Yadro/cmake-build-debug

# Include any dependencies generated for this target.
include Google_tests/CMakeFiles/unit_tests_run.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include Google_tests/CMakeFiles/unit_tests_run.dir/compiler_depend.make

# Include the progress variables for this target.
include Google_tests/CMakeFiles/unit_tests_run.dir/progress.make

# Include the compile flags for this target's objects.
include Google_tests/CMakeFiles/unit_tests_run.dir/flags.make

Google_tests/CMakeFiles/unit_tests_run.dir/test.cpp.o: Google_tests/CMakeFiles/unit_tests_run.dir/flags.make
Google_tests/CMakeFiles/unit_tests_run.dir/test.cpp.o: /home/anna/Yadro/Google_tests/test.cpp
Google_tests/CMakeFiles/unit_tests_run.dir/test.cpp.o: Google_tests/CMakeFiles/unit_tests_run.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/anna/Yadro/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Google_tests/CMakeFiles/unit_tests_run.dir/test.cpp.o"
	cd /home/anna/Yadro/cmake-build-debug/Google_tests && /usr/bin/clang++-16 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Google_tests/CMakeFiles/unit_tests_run.dir/test.cpp.o -MF CMakeFiles/unit_tests_run.dir/test.cpp.o.d -o CMakeFiles/unit_tests_run.dir/test.cpp.o -c /home/anna/Yadro/Google_tests/test.cpp

Google_tests/CMakeFiles/unit_tests_run.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/unit_tests_run.dir/test.cpp.i"
	cd /home/anna/Yadro/cmake-build-debug/Google_tests && /usr/bin/clang++-16 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/anna/Yadro/Google_tests/test.cpp > CMakeFiles/unit_tests_run.dir/test.cpp.i

Google_tests/CMakeFiles/unit_tests_run.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/unit_tests_run.dir/test.cpp.s"
	cd /home/anna/Yadro/cmake-build-debug/Google_tests && /usr/bin/clang++-16 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/anna/Yadro/Google_tests/test.cpp -o CMakeFiles/unit_tests_run.dir/test.cpp.s

# Object files for target unit_tests_run
unit_tests_run_OBJECTS = \
"CMakeFiles/unit_tests_run.dir/test.cpp.o"

# External object files for target unit_tests_run
unit_tests_run_EXTERNAL_OBJECTS =

Google_tests/unit_tests_run: Google_tests/CMakeFiles/unit_tests_run.dir/test.cpp.o
Google_tests/unit_tests_run: Google_tests/CMakeFiles/unit_tests_run.dir/build.make
Google_tests/unit_tests_run: lib/libgtest.a
Google_tests/unit_tests_run: lib/libgtest_main.a
Google_tests/unit_tests_run: lib/libgtest.a
Google_tests/unit_tests_run: Google_tests/CMakeFiles/unit_tests_run.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/anna/Yadro/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable unit_tests_run"
	cd /home/anna/Yadro/cmake-build-debug/Google_tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/unit_tests_run.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Google_tests/CMakeFiles/unit_tests_run.dir/build: Google_tests/unit_tests_run
.PHONY : Google_tests/CMakeFiles/unit_tests_run.dir/build

Google_tests/CMakeFiles/unit_tests_run.dir/clean:
	cd /home/anna/Yadro/cmake-build-debug/Google_tests && $(CMAKE_COMMAND) -P CMakeFiles/unit_tests_run.dir/cmake_clean.cmake
.PHONY : Google_tests/CMakeFiles/unit_tests_run.dir/clean

Google_tests/CMakeFiles/unit_tests_run.dir/depend:
	cd /home/anna/Yadro/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/anna/Yadro /home/anna/Yadro/Google_tests /home/anna/Yadro/cmake-build-debug /home/anna/Yadro/cmake-build-debug/Google_tests /home/anna/Yadro/cmake-build-debug/Google_tests/CMakeFiles/unit_tests_run.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : Google_tests/CMakeFiles/unit_tests_run.dir/depend

