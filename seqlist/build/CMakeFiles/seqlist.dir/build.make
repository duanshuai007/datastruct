# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.12.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.12.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/duanshuai/Downloads/Linux/datastruct/seqlist

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/duanshuai/Downloads/Linux/datastruct/seqlist/build

# Include any dependencies generated for this target.
include CMakeFiles/seqlist.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/seqlist.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/seqlist.dir/flags.make

CMakeFiles/seqlist.dir/seqlist_ptr.o: CMakeFiles/seqlist.dir/flags.make
CMakeFiles/seqlist.dir/seqlist_ptr.o: ../seqlist_ptr.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/duanshuai/Downloads/Linux/datastruct/seqlist/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/seqlist.dir/seqlist_ptr.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/seqlist.dir/seqlist_ptr.o   -c /Users/duanshuai/Downloads/Linux/datastruct/seqlist/seqlist_ptr.c

CMakeFiles/seqlist.dir/seqlist_ptr.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/seqlist.dir/seqlist_ptr.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/duanshuai/Downloads/Linux/datastruct/seqlist/seqlist_ptr.c > CMakeFiles/seqlist.dir/seqlist_ptr.i

CMakeFiles/seqlist.dir/seqlist_ptr.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/seqlist.dir/seqlist_ptr.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/duanshuai/Downloads/Linux/datastruct/seqlist/seqlist_ptr.c -o CMakeFiles/seqlist.dir/seqlist_ptr.s

CMakeFiles/seqlist.dir/main.o: CMakeFiles/seqlist.dir/flags.make
CMakeFiles/seqlist.dir/main.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/duanshuai/Downloads/Linux/datastruct/seqlist/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/seqlist.dir/main.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/seqlist.dir/main.o   -c /Users/duanshuai/Downloads/Linux/datastruct/seqlist/main.c

CMakeFiles/seqlist.dir/main.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/seqlist.dir/main.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/duanshuai/Downloads/Linux/datastruct/seqlist/main.c > CMakeFiles/seqlist.dir/main.i

CMakeFiles/seqlist.dir/main.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/seqlist.dir/main.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/duanshuai/Downloads/Linux/datastruct/seqlist/main.c -o CMakeFiles/seqlist.dir/main.s

# Object files for target seqlist
seqlist_OBJECTS = \
"CMakeFiles/seqlist.dir/seqlist_ptr.o" \
"CMakeFiles/seqlist.dir/main.o"

# External object files for target seqlist
seqlist_EXTERNAL_OBJECTS =

seqlist: CMakeFiles/seqlist.dir/seqlist_ptr.o
seqlist: CMakeFiles/seqlist.dir/main.o
seqlist: CMakeFiles/seqlist.dir/build.make
seqlist: CMakeFiles/seqlist.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/duanshuai/Downloads/Linux/datastruct/seqlist/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable seqlist"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/seqlist.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/seqlist.dir/build: seqlist

.PHONY : CMakeFiles/seqlist.dir/build

CMakeFiles/seqlist.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/seqlist.dir/cmake_clean.cmake
.PHONY : CMakeFiles/seqlist.dir/clean

CMakeFiles/seqlist.dir/depend:
	cd /Users/duanshuai/Downloads/Linux/datastruct/seqlist/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/duanshuai/Downloads/Linux/datastruct/seqlist /Users/duanshuai/Downloads/Linux/datastruct/seqlist /Users/duanshuai/Downloads/Linux/datastruct/seqlist/build /Users/duanshuai/Downloads/Linux/datastruct/seqlist/build /Users/duanshuai/Downloads/Linux/datastruct/seqlist/build/CMakeFiles/seqlist.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/seqlist.dir/depend

