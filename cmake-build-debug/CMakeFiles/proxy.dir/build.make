# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_SOURCE_DIR = /mnt/c/Users/User/CLionProjects/untitled

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/User/CLionProjects/untitled/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/proxy.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/proxy.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/proxy.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/proxy.dir/flags.make

CMakeFiles/proxy.dir/CodeProxy/proxy.c.o: CMakeFiles/proxy.dir/flags.make
CMakeFiles/proxy.dir/CodeProxy/proxy.c.o: /mnt/c/Users/User/CLionProjects/untitled/CodeProxy/proxy.c
CMakeFiles/proxy.dir/CodeProxy/proxy.c.o: CMakeFiles/proxy.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/User/CLionProjects/untitled/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/proxy.dir/CodeProxy/proxy.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/proxy.dir/CodeProxy/proxy.c.o -MF CMakeFiles/proxy.dir/CodeProxy/proxy.c.o.d -o CMakeFiles/proxy.dir/CodeProxy/proxy.c.o -c /mnt/c/Users/User/CLionProjects/untitled/CodeProxy/proxy.c

CMakeFiles/proxy.dir/CodeProxy/proxy.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proxy.dir/CodeProxy/proxy.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/User/CLionProjects/untitled/CodeProxy/proxy.c > CMakeFiles/proxy.dir/CodeProxy/proxy.c.i

CMakeFiles/proxy.dir/CodeProxy/proxy.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proxy.dir/CodeProxy/proxy.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/User/CLionProjects/untitled/CodeProxy/proxy.c -o CMakeFiles/proxy.dir/CodeProxy/proxy.c.s

CMakeFiles/proxy.dir/CodeProxy/proxyAPI.c.o: CMakeFiles/proxy.dir/flags.make
CMakeFiles/proxy.dir/CodeProxy/proxyAPI.c.o: /mnt/c/Users/User/CLionProjects/untitled/CodeProxy/proxyAPI.c
CMakeFiles/proxy.dir/CodeProxy/proxyAPI.c.o: CMakeFiles/proxy.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/User/CLionProjects/untitled/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/proxy.dir/CodeProxy/proxyAPI.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/proxy.dir/CodeProxy/proxyAPI.c.o -MF CMakeFiles/proxy.dir/CodeProxy/proxyAPI.c.o.d -o CMakeFiles/proxy.dir/CodeProxy/proxyAPI.c.o -c /mnt/c/Users/User/CLionProjects/untitled/CodeProxy/proxyAPI.c

CMakeFiles/proxy.dir/CodeProxy/proxyAPI.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proxy.dir/CodeProxy/proxyAPI.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/User/CLionProjects/untitled/CodeProxy/proxyAPI.c > CMakeFiles/proxy.dir/CodeProxy/proxyAPI.c.i

CMakeFiles/proxy.dir/CodeProxy/proxyAPI.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proxy.dir/CodeProxy/proxyAPI.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/User/CLionProjects/untitled/CodeProxy/proxyAPI.c -o CMakeFiles/proxy.dir/CodeProxy/proxyAPI.c.s

# Object files for target proxy
proxy_OBJECTS = \
"CMakeFiles/proxy.dir/CodeProxy/proxy.c.o" \
"CMakeFiles/proxy.dir/CodeProxy/proxyAPI.c.o"

# External object files for target proxy
proxy_EXTERNAL_OBJECTS =

proxy: CMakeFiles/proxy.dir/CodeProxy/proxy.c.o
proxy: CMakeFiles/proxy.dir/CodeProxy/proxyAPI.c.o
proxy: CMakeFiles/proxy.dir/build.make
proxy: libapisocket.a
proxy: CMakeFiles/proxy.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/User/CLionProjects/untitled/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable proxy"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/proxy.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/proxy.dir/build: proxy
.PHONY : CMakeFiles/proxy.dir/build

CMakeFiles/proxy.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/proxy.dir/cmake_clean.cmake
.PHONY : CMakeFiles/proxy.dir/clean

CMakeFiles/proxy.dir/depend:
	cd /mnt/c/Users/User/CLionProjects/untitled/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/User/CLionProjects/untitled /mnt/c/Users/User/CLionProjects/untitled /mnt/c/Users/User/CLionProjects/untitled/cmake-build-debug /mnt/c/Users/User/CLionProjects/untitled/cmake-build-debug /mnt/c/Users/User/CLionProjects/untitled/cmake-build-debug/CMakeFiles/proxy.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/proxy.dir/depend

