cmake_minimum_required(VERSION 3.10)
project(NeuroMapCHOP)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set macOS deployment target
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13)

# TouchDesigner Plugin specific settings
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../CustomOperatorSamples/CHOP/BasicFilterCHOP  # For TouchDesigner headers
    ${CMAKE_CURRENT_SOURCE_DIR}/../MiniDNN/include  # MiniDNN headers
    ${CMAKE_CURRENT_SOURCE_DIR}/../MiniDNN/include/external/sparsepp  # Eigen is included in MiniDNN
)

# Find Eigen (header-only)
find_package(Eigen3 QUIET)
if(NOT Eigen3_FOUND)
    # If system Eigen not found, we'll use the one that comes with MiniDNN
    message(STATUS "Using Eigen from system or MiniDNN dependencies")
endif()

# Source files
set(SOURCES
    NeuroMapCHOP.cpp
    Parameters.cpp
    DataManager.cpp
)

set(HEADERS
    NeuroMapCHOP.h
    Parameters.h
    DataManager.h
    CPlusPlus_Common.h
    CHOP_CPlusPlusBase.h
)

# Create the plugin library
add_library(${PROJECT_NAME} MODULE ${SOURCES} ${HEADERS})

# macOS specific bundle settings
set_target_properties(${PROJECT_NAME} PROPERTIES
    BUNDLE TRUE
    BUNDLE_EXTENSION "plugin"
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in
    OUTPUT_NAME "NeuroMapCHOP"
)

# Compiler definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
    NOMINMAX
    WIN32_LEAN_AND_MEAN
)

# Linker flags for macOS
if(APPLE)
    target_link_libraries(${PROJECT_NAME} 
        "-framework Foundation"
        "-framework CoreGraphics" 
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS "-Wl,-exported_symbol,_FillCHOPPluginInfo -Wl,-exported_symbol,_CreateCHOPInstance -Wl,-exported_symbol,_DestroyCHOPInstance"
    )
endif()

# Copy TouchDesigner headers to our project
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/../CustomOperatorSamples/CHOP/BasicFilterCHOP/CHOP_CPlusPlusBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CHOP_CPlusPlusBase.h
    COPYONLY
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/../CustomOperatorSamples/CHOP/BasicFilterCHOP/CPlusPlus_Common.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CPlusPlus_Common.h
    COPYONLY
)