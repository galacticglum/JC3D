macro(ternary var boolean value1 value2)
    set(${var} $<${${boolean}}:${value1}>$<$<NOT:${${boolean}}>:${value2}>)
endmacro(ternary)

macro(CONFIG_NAME var)
    set(${var} $<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>)
endmacro(CONFIG_NAME)

CONFIG_NAME(_config)
cmake_minimum_required(VERSION 3.6)

# Setup the base project
project(JesusChristIn3D)

option(BUILD_ENGINE "Generate engine project" ON)
option(BUILD_SANDBOX "Generate sandbox project" ON)

# We need a CMAKE_DIR with some code to find external dependencies
SET(JesusChristIn3D_CMAKE_DIR "${JesusChristIn3D_SOURCE_DIR}/CMake")
SET(JesusChristIn3D_Engine_DEPEND_DIR "${JesusChristIn3D_SOURCE_DIR}/Engine/Dependencies")

if(BUILD_ENGINE)
    add_subdirectory(${CMAKE_SOURCE_DIR}/Engine)
endif()

if(BUILD_SANDBOX)
    add_subdirectory(${CMAKE_SOURCE_DIR}/Sandbox)
endif()

if (MSVC)
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT Sandbox)
endif (MSVC)