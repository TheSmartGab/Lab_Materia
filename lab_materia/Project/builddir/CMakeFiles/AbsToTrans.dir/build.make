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
include CMakeFiles/AbsToTrans.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/AbsToTrans.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/AbsToTrans.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/AbsToTrans.dir/flags.make

CMakeFiles/AbsToTrans.dir/Macros/AbsToTrans.cpp.o: CMakeFiles/AbsToTrans.dir/flags.make
CMakeFiles/AbsToTrans.dir/Macros/AbsToTrans.cpp.o: ../Macros/AbsToTrans.cpp
CMakeFiles/AbsToTrans.dir/Macros/AbsToTrans.cpp.o: CMakeFiles/AbsToTrans.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gabri/uni/Lab_Materia_copy/lab_materia/Project/builddir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/AbsToTrans.dir/Macros/AbsToTrans.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/AbsToTrans.dir/Macros/AbsToTrans.cpp.o -MF CMakeFiles/AbsToTrans.dir/Macros/AbsToTrans.cpp.o.d -o CMakeFiles/AbsToTrans.dir/Macros/AbsToTrans.cpp.o -c /home/gabri/uni/Lab_Materia_copy/lab_materia/Project/Macros/AbsToTrans.cpp

CMakeFiles/AbsToTrans.dir/Macros/AbsToTrans.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AbsToTrans.dir/Macros/AbsToTrans.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gabri/uni/Lab_Materia_copy/lab_materia/Project/Macros/AbsToTrans.cpp > CMakeFiles/AbsToTrans.dir/Macros/AbsToTrans.cpp.i

CMakeFiles/AbsToTrans.dir/Macros/AbsToTrans.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AbsToTrans.dir/Macros/AbsToTrans.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gabri/uni/Lab_Materia_copy/lab_materia/Project/Macros/AbsToTrans.cpp -o CMakeFiles/AbsToTrans.dir/Macros/AbsToTrans.cpp.s

# Object files for target AbsToTrans
AbsToTrans_OBJECTS = \
"CMakeFiles/AbsToTrans.dir/Macros/AbsToTrans.cpp.o"

# External object files for target AbsToTrans
AbsToTrans_EXTERNAL_OBJECTS =

AbsToTrans: CMakeFiles/AbsToTrans.dir/Macros/AbsToTrans.cpp.o
AbsToTrans: CMakeFiles/AbsToTrans.dir/build.make
AbsToTrans: libLibraries.a
AbsToTrans: /home/gabri/root/lib/libCore.so
AbsToTrans: /home/gabri/root/lib/libImt.so
AbsToTrans: /home/gabri/root/lib/libRIO.so
AbsToTrans: /home/gabri/root/lib/libNet.so
AbsToTrans: /home/gabri/root/lib/libHist.so
AbsToTrans: /home/gabri/root/lib/libGraf.so
AbsToTrans: /home/gabri/root/lib/libGraf3d.so
AbsToTrans: /home/gabri/root/lib/libGpad.so
AbsToTrans: /home/gabri/root/lib/libROOTDataFrame.so
AbsToTrans: /home/gabri/root/lib/libTree.so
AbsToTrans: /home/gabri/root/lib/libTreePlayer.so
AbsToTrans: /home/gabri/root/lib/libRint.so
AbsToTrans: /home/gabri/root/lib/libPostscript.so
AbsToTrans: /home/gabri/root/lib/libMatrix.so
AbsToTrans: /home/gabri/root/lib/libPhysics.so
AbsToTrans: /home/gabri/root/lib/libMathCore.so
AbsToTrans: /home/gabri/root/lib/libThread.so
AbsToTrans: /home/gabri/root/lib/libMultiProc.so
AbsToTrans: /home/gabri/root/lib/libROOTVecOps.so
AbsToTrans: CMakeFiles/AbsToTrans.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/gabri/uni/Lab_Materia_copy/lab_materia/Project/builddir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable AbsToTrans"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/AbsToTrans.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/AbsToTrans.dir/build: AbsToTrans
.PHONY : CMakeFiles/AbsToTrans.dir/build

CMakeFiles/AbsToTrans.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/AbsToTrans.dir/cmake_clean.cmake
.PHONY : CMakeFiles/AbsToTrans.dir/clean

CMakeFiles/AbsToTrans.dir/depend:
	cd /home/gabri/uni/Lab_Materia_copy/lab_materia/Project/builddir && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gabri/uni/Lab_Materia_copy/lab_materia/Project /home/gabri/uni/Lab_Materia_copy/lab_materia/Project /home/gabri/uni/Lab_Materia_copy/lab_materia/Project/builddir /home/gabri/uni/Lab_Materia_copy/lab_materia/Project/builddir /home/gabri/uni/Lab_Materia_copy/lab_materia/Project/builddir/CMakeFiles/AbsToTrans.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/AbsToTrans.dir/depend

