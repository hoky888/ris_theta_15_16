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
include cob_people_perception/libnite2/CMakeFiles/NiTE2.dir/depend.make

# Include the progress variables for this target.
include cob_people_perception/libnite2/CMakeFiles/NiTE2.dir/progress.make

# Include the compile flags for this target's objects.
include cob_people_perception/libnite2/CMakeFiles/NiTE2.dir/flags.make

# Object files for target NiTE2
NiTE2_OBJECTS =

# External object files for target NiTE2
NiTE2_EXTERNAL_OBJECTS =

/home/team_theta/ROS/devel/lib/libNiTE2.so: cob_people_perception/libnite2/CMakeFiles/NiTE2.dir/build.make
/home/team_theta/ROS/devel/lib/libNiTE2.so: cob_people_perception/libnite2/CMakeFiles/NiTE2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library /home/team_theta/ROS/devel/lib/libNiTE2.so"
	cd /home/team_theta/ROS/build/cob_people_perception/libnite2 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/NiTE2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
cob_people_perception/libnite2/CMakeFiles/NiTE2.dir/build: /home/team_theta/ROS/devel/lib/libNiTE2.so
.PHONY : cob_people_perception/libnite2/CMakeFiles/NiTE2.dir/build

cob_people_perception/libnite2/CMakeFiles/NiTE2.dir/requires:
.PHONY : cob_people_perception/libnite2/CMakeFiles/NiTE2.dir/requires

cob_people_perception/libnite2/CMakeFiles/NiTE2.dir/clean:
	cd /home/team_theta/ROS/build/cob_people_perception/libnite2 && $(CMAKE_COMMAND) -P CMakeFiles/NiTE2.dir/cmake_clean.cmake
.PHONY : cob_people_perception/libnite2/CMakeFiles/NiTE2.dir/clean

cob_people_perception/libnite2/CMakeFiles/NiTE2.dir/depend:
	cd /home/team_theta/ROS/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/team_theta/ROS/src /home/team_theta/ROS/src/cob_people_perception/libnite2 /home/team_theta/ROS/build /home/team_theta/ROS/build/cob_people_perception/libnite2 /home/team_theta/ROS/build/cob_people_perception/libnite2/CMakeFiles/NiTE2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : cob_people_perception/libnite2/CMakeFiles/NiTE2.dir/depend

