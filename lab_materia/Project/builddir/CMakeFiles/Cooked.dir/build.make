# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/gabri/uni/Lab_Materia_copy/lab_materia/Project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gabri/uni/Lab_Materia_copy/lab_materia/Project/builddir

# Include any dependencies generated for this target.
include CMakeFiles/Cooked.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Cooked.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Cooked.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Cooked.dir/flags.make

CMakeFiles/Cooked.dir/Macros/Cooked.cpp.o: CMakeFiles/Cooked.dir/flags.make
CMakeFiles/Cooked.dir/Macros/Cooked.cpp.o: ../Macros/Cooked.cpp
CMakeFiles/Cooked.dir/Macros/Cooked.cpp.o: CMakeFiles/Cooked.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gabri/uni/Lab_Materia_copy/lab_materia/Project/builddir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Cooked.dir/Macros/Cooked.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Cooked.dir/Macros/Cooked.cpp.o -MF CMakeFiles/Cooked.dir/Macros/Cooked.cpp.o.d -o CMakeFiles/Cooked.dir/Macros/Cooked.cpp.o -c /home/gabri/uni/Lab_Materia_copy/lab_materia/Project/Macros/Cooked.cpp

CMakeFiles/Cooked.dir/Macros/Cooked.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Cooked.dir/Macros/Cooked.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gabri/uni/Lab_Materia_copy/lab_materia/Project/Macros/Cooked.cpp > CMakeFiles/Cooked.dir/Macros/Cooked.cpp.i

CMakeFiles/Cooked.dir/Macros/Cooked.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Cooked.dir/Macros/Cooked.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gabri/uni/Lab_Materia_copy/lab_materia/Project/Macros/Cooked.cpp -o CMakeFiles/Cooked.dir/Macros/Cooked.cpp.s

# Object files for target Cooked
Cooked_OBJECTS = \
"CMakeFiles/Cooked.dir/Macros/Cooked.cpp.o"

# External object files for target Cooked
Cooked_EXTERNAL_OBJECTS =

Cooked: CMakeFiles/Cooked.dir/Macros/Cooked.cpp.o
Cooked: CMakeFiles/Cooked.dir/build.make
Cooked: libLibraries.a
Cooked: /home/gabri/root/lib/libCore.so
Cooked: /home/gabri/root/lib/libImt.so
Cooked: /home/gabri/root/lib/libRIO.so
Cooked: /home/gabri/root/lib/libNet.so
Cooked: /home/gabri/root/lib/libHist.so
Cooked: /home/gabri/root/lib/libGraf.so
Cooked: /home/gabri/root/lib/libGraf3d.so
Cooked: /home/gabri/root/lib/libGpad.so
Cooked: /home/gabri/root/lib/libROOTDataFrame.so
Cooked: /home/gabri/root/lib/libTree.so
Cooked: /home/gabri/root/lib/libTreePlayer.so
Cooked: /home/gabri/root/lib/libRint.so
Cooked: /home/gabri/root/lib/libPostscript.so
Cooked: /home/gabri/root/lib/libMatrix.so
Cooked: /home/gabri/root/lib/libPhysics.so
Cooked: /home/gabri/root/lib/libMathCore.so
Cooked: /home/gabri/root/lib/libThread.so
Cooked: /home/gabri/root/lib/libMultiProc.so
Cooked: /home/gabri/root/lib/libROOTVecOps.so
Cooked: CMakeFiles/Cooked.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/gabri/uni/Lab_Materia_copy/lab_materia/Project/builddir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Cooked"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Cooked.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Cooked.dir/build: Cooked
.PHONY : CMakeFiles/Cooked.dir/build

CMakeFiles/Cooked.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Cooked.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Cooked.dir/clean

CMakeFiles/Cooked.dir/depend:
	cd /home/gabri/uni/Lab_Materia_copy/lab_materia/Project/builddir && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gabri/uni/Lab_Materia_copy/lab_materia/Project /home/gabri/uni/Lab_Materia_copy/lab_materia/Project /home/gabri/uni/Lab_Materia_copy/lab_materia/Project/builddir /home/gabri/uni/Lab_Materia_copy/lab_materia/Project/builddir /home/gabri/uni/Lab_Materia_copy/lab_materia/Project/builddir/CMakeFiles/Cooked.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Cooked.dir/depend

