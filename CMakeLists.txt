cmake_minimum_required(VERSION 3.12)
project(CGPraktikum)

###############################################################################
## file globbing ##############################################################
###############################################################################

# these instructions search the directory tree when cmake is
# invoked and put all files that match the pattern in the variables 
# `sources` and `data`
file(GLOB sources src/*.cpp src/*.hpp)
# you can use set(sources src/main.cpp) etc if you don't want to
# use globing to find files automatically

# set the default build type if none was set
if( NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES )
    message( STATUS "Setting build type to 'Debug' as none was specified." )
    set( CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE )
    # set the possible values of build type for cmake-gui
    set_property( CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo" )
endif()

###############################################################################
## target definitions #########################################################
###############################################################################



# add the data to the target, so it becomes visible in some IDE
add_executable( CGPraktikum ${sources} ${data})

# just for example add some compiler flags
target_compile_options(CGPraktikum PUBLIC -std=c++14 -Wall -Wfloat-conversion)

set_target_properties( CGPraktikum PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


###############################################################################
## dependencies ###############################################################
###############################################################################

# find external assimp library
find_library( ASSIMP_LIBRARY assimp )
find_path( ASSIMP_INCLUDE_DIR assimp/Importer.hpp )

if(ASSIMP_INCLUDE_DIR AND ASSIMP_LIBRARY)
    add_library( ASSIMP INTERFACE IMPORTED )
    set_property( TARGET ASSIMP PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${ASSIMP_INCLUDE_DIR} )
    set_property( TARGET ASSIMP PROPERTY INTERFACE_LINK_LIBRARIES ${ASSIMP_LIBRARY} )
    message( STATUS "!!! ASSIMP found and added to dependencies" )
    target_link_libraries( CGPraktikum PUBLIC ASSIMP )

else()
    message( STATUS "Did not include plugin AssImpMeshImporter due to missing libraries")
endif()




# Parallelisierung mit OpenMP
find_package( OpenMP )
if( OPENMP_FOUND )
     target_link_libraries(CGPraktikum PUBLIC OpenMP::OpenMP_CXX)
    message( STATUS "!!! OpenMP found and added to dependencies" )
else()
    message( STATUS "!!! OpenMP not found | OpenMP should be installed for exercise 3 and 4" )
endif()

