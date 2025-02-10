cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(Tutorial)

# Find the Geant4 package
find_package(Geant4 REQUIRED ui_all vis_all)

# Include Geant4-specific setup file
include(${Geant4_USE_FILE})

# Include the project's include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Find all source files in the src/ directory
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

file(GLOB MACRO_FILES "macros/*.mac")
file(COPY ${MACRO_FILES} DESTINATION ${CMAKE_BINARY_DIR}/)

# Define the executable
add_executable(sim sim.cc ${sources})

# Link Geant4 libraries to the executable
target_link_libraries(sim ${Geant4_LIBRARIES})

# Define the custom target 'Tutorial' which depends on 'sim'
add_custom_target(Tutorial DEPENDS sim)

# Set RPATH to use when installing
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "$ORIGIN")