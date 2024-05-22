cmake_minimum_required(VERSION 3.10)
project(YourProjectName)

# Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED)

# Add include directories for ROOT
include_directories(${ROOT_INCLUDE_DIRS})
include_directories("${CMAKE_SOURCE_DIR}/Libs/Libraries")

# Add your precompiled libraries to your project
file(GLOB LIBRARY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Libs/Libraries/*.cpp")
add_library(Libraries STATIC ${LIBRARY_SOURCES})

# Add an executable target for each .cpp file in the Macros directory
file(GLOB MACRO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Macros/*.cpp")
foreach(MACRO_SOURCE ${MACRO_SOURCES})
    # Get the name of the file without the extension
    get_filename_component(MACRO_EXECUTABLE ${MACRO_SOURCE} NAME_WE)
    # Add the executable target
    add_executable(${MACRO_EXECUTABLE} ${MACRO_SOURCE})
    # Link the executable target with your precompiled libraries, fmt, and ROOT libraries
    target_link_libraries(${MACRO_EXECUTABLE} Libraries ${ROOT_LIBRARIES})
endforeach()
