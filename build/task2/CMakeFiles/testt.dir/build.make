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

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/team_theta/ROS/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/team_theta/ROS/build

# Include any dependencies generated for this target.
include task2/CMakeFiles/testt.dir/depend.make

# Include the progress variables for this target.
include task2/CMakeFiles/testt.dir/progress.make

# Include the compile flags for this target's objects.
include task2/CMakeFiles/testt.dir/flags.make

task2/CMakeFiles/testt.dir/src/testt.cpp.o: task2/CMakeFiles/testt.dir/flags.make
task2/CMakeFiles/testt.dir/src/testt.cpp.o: /home/team_theta/ROS/src/task2/src/testt.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/team_theta/ROS/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object task2/CMakeFiles/testt.dir/src/testt.cpp.o"
	cd /home/team_theta/ROS/build/task2 && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/testt.dir/src/testt.cpp.o -c /home/team_theta/ROS/src/task2/src/testt.cpp

task2/CMakeFiles/testt.dir/src/testt.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testt.dir/src/testt.cpp.i"
	cd /home/team_theta/ROS/build/task2 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/team_theta/ROS/src/task2/src/testt.cpp > CMakeFiles/testt.dir/src/testt.cpp.i

task2/CMakeFiles/testt.dir/src/testt.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testt.dir/src/testt.cpp.s"
	cd /home/team_theta/ROS/build/task2 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/team_theta/ROS/src/task2/src/testt.cpp -o CMakeFiles/testt.dir/src/testt.cpp.s

task2/CMakeFiles/testt.dir/src/testt.cpp.o.requires:
.PHONY : task2/CMakeFiles/testt.dir/src/testt.cpp.o.requires

task2/CMakeFiles/testt.dir/src/testt.cpp.o.provides: task2/CMakeFiles/testt.dir/src/testt.cpp.o.requires
	$(MAKE) -f task2/CMakeFiles/testt.dir/build.make task2/CMakeFiles/testt.dir/src/testt.cpp.o.provides.build
.PHONY : task2/CMakeFiles/testt.dir/src/testt.cpp.o.provides

task2/CMakeFiles/testt.dir/src/testt.cpp.o.provides.build: task2/CMakeFiles/testt.dir/src/testt.cpp.o

# Object files for target testt
testt_OBJECTS = \
"CMakeFiles/testt.dir/src/testt.cpp.o"

# External object files for target testt
testt_EXTERNAL_OBJECTS =

/home/team_theta/ROS/devel/lib/task2/testt: task2/CMakeFiles/testt.dir/src/testt.cpp.o
/home/team_theta/ROS/devel/lib/task2/testt: task2/CMakeFiles/testt.dir/build.make
/home/team_theta/ROS/devel/lib/task2/testt: /opt/ros/indigo/lib/libroscpp_serialization.so
/home/team_theta/ROS/devel/lib/task2/testt: /opt/ros/indigo/lib/librostime.so
/home/team_theta/ROS/devel/lib/task2/testt: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
/home/team_theta/ROS/devel/lib/task2/testt: /opt/ros/indigo/lib/libcpp_common.so
/home/team_theta/ROS/devel/lib/task2/testt: /usr/lib/x86_64-linux-gnu/libboost_system.so
/home/team_theta/ROS/devel/lib/task2/testt: /usr/lib/x86_64-linux-gnu/libboost_thread.so
/home/team_theta/ROS/devel/lib/task2/testt: /usr/lib/x86_64-linux-gnu/libpthread.so
/home/team_theta/ROS/devel/lib/task2/testt: /usr/lib/x86_64-linux-gnu/libconsole_bridge.so
/home/team_theta/ROS/devel/lib/task2/testt: task2/CMakeFiles/testt.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable /home/team_theta/ROS/devel/lib/task2/testt"
	cd /home/team_theta/ROS/build/task2 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/testt.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
task2/CMakeFiles/testt.dir/build: /home/team_theta/ROS/devel/lib/task2/testt
.PHONY : task2/CMakeFiles/testt.dir/build

task2/CMakeFiles/testt.dir/requires: task2/CMakeFiles/testt.dir/src/testt.cpp.o.requires
.PHONY : task2/CMakeFiles/testt.dir/requires

task2/CMakeFiles/testt.dir/clean:
	cd /home/team_theta/ROS/build/task2 && $(CMAKE_COMMAND) -P CMakeFiles/testt.dir/cmake_clean.cmake
.PHONY : task2/CMakeFiles/testt.dir/clean

task2/CMakeFiles/testt.dir/depend:
	cd /home/team_theta/ROS/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/team_theta/ROS/src /home/team_theta/ROS/src/task2 /home/team_theta/ROS/build /home/team_theta/ROS/build/task2 /home/team_theta/ROS/build/task2/CMakeFiles/testt.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : task2/CMakeFiles/testt.dir/depend
