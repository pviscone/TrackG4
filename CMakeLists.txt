 
## Set the minumum version
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

set(PROJECT "Cosmic Ray on CMS tracker modules")
set(EXECUTABLE_NAME "CosmicMuSimulation")
set(MAIN_FILE "G4main.cc")

## Project name
project(${PROJECT})


##Uncomment to enable Verbose Make
#set(CMAKE_VERBOSE_MAKEFILE ON)

## Set Cpp Standard, pedantic and root flags
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g3 -pthread -gdwarf-4 -fPIC -Wno-deprecated -pipe -fno-elide-type -fdiagnostics-show-template-tree -Wall  -Wextra -Wpedantic -Wvla -Wextra-semi -Wnull-dereference -pthread -m64 -L/usr/lib/root -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lROOTVecOps -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -lROOTDataFrame -pthread -lm -ldl -rdynamic")

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

################# Tests section ####################

## Create the executable of the tests
file(GLOB testcpp ${PROJECT_SOURCE_DIR}/test/*/*.cpp ${PROJECT_SOURCE_DIR}/test/*)
add_executable(tests_executable ${testcpp} ${sources} ${headers})
target_link_libraries(tests_executable ${Geant4_LIBRARIES})
####################################################

add_executable(track ./trackfit.cc)
target_link_libraries(track ${Geant4_LIBRARIES})
############# Documentation Section ################

# first we can indicate the documentation build as an option and set it to ON by default
option(BUILD_DOC "Build documentation" OFF)

# check if Doxygen is installed
if(BUILD_DOC)
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # set input Doxyfile
    set(DOXYGEN_OUT ${PROJECT_SOURCE_DIR}/docs/Doxyfile)

    message("Doxygen build started")
    # the option ALL allows to build the docs together with the application
    add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/docs
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)
endif (BUILD_DOC)

####################################################


#----------------------------------------------------------------------------
# Copy all scripts to the build directory
set(SCRIPTS
    vis.mac
    run.mac
    )




foreach(_script ${SCRIPTS})
    configure_file(
        ${PROJECT_SOURCE_DIR}/macro/${_script}
        ${PROJECT_BINARY_DIR}/${_script}
        COPYONLY
    )
endforeach()




#add_custom_target(Simulation DEPENDS sim)
