#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.16...3.21)
project(PCT)

set(CMAKE_CXX_STANDARD 17) # Set C++ standard to 17
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Require the specified standard
#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

find_package(ROOT)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include(${ROOT_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(examplePCT examplePCT.cc ${sources} ${headers})
target_link_libraries(examplePCT ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build PCT. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(EXAMPLEPCT_SCRIPTS
  examplePCT.in
  examplePCT.out
  init_vis.mac
  run1.mac
  run2.mac
  vis.mac
  tsg_offscreen.mac
  plotNtuple.C
  run_loop.mac
  onloop.mac
  anglelogs.txt
  parallelbeam.mac
  gaussianbeam.mac
  )

set(PCT_DIRECTORY_SCRIPTS
  phantom
)

foreach(_script ${EXAMPLEPCT_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

foreach(_script ${PCT_DIRECTORY_SCRIPTS})
  file(COPY 
    ${PROJECT_SOURCE_DIR}/${_script}
    DESTINATION 
    ${PROJECT_BINARY_DIR}
    )
endforeach()

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/output)
#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
add_custom_target(PCT DEPENDS examplePCT)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS examplePCT DESTINATION bin)
