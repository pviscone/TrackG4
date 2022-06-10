## Set the minumum version
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

set(PROJECT "Cosmic Ray on CMS tracker modules")
set(EXECUTABLE_NAME "CosmicMuSimulation")
set(MAIN_FILE "G4main.cc")

## Project name
project(${PROJECT})


##Uncomment to enable Verbose Make
#set(CMAKE_VERBOSE_MAKEFILE ON)

## Set Cpp Standard
set (CMAKE_CXX_STANDARD 17)


##Set g++ as compiler if exists
find_program(GPP g++)
if(GPP)
    set(CMAKE_CXX_COMPILER g++)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fdiagnostics-color=auto")
endif()

## Check for ccache to speed up builds
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()


find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})



## Set the include directories and the file to compile
include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh ${PROJECT_SOURCE_DIR}/include/*.h)



## Create the executable
add_executable(${EXECUTABLE_NAME} ${MAIN_FILE} ${sources} ${headers})
target_link_libraries(${EXECUTABLE_NAME} ${Geant4_LIBRARIES})


#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(SCRIPTS
    vis.mac
    )


foreach(_script ${SCRIPTS})
    configure_file(
        ${PROJECT_SOURCE_DIR}/${_script}
        ${PROJECT_BINARY_DIR}/${_script}
        COPYONLY
    )
endforeach()





#add_custom_target(Simulation DEPENDS sim)
