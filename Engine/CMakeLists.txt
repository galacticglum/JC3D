# add the -c and -Wall flags
if(MSVC)
	add_definitions(
		-c
	)
else()
	add_definitions(
		-c
		-Wall
	)
endif()

add_definitions( -D_CRT_SECURE_NO_WARNINGS )
add_definitions( -DFMT_HEADER_ONLY )
add_definitions( -DGLFW_INCLUDE_NONE )

# Lets load up our headers for the engine project
file(GLOB_RECURSE ENGINE_HDRS
	${JesusChristIn3D_SOURCE_DIR}/Engine/Source/*.h
)

# Lets load up our sources for the engine project
file(GLOB_RECURSE ENGINE_SRCS
	${JesusChristIn3D_SOURCE_DIR}/Engine/Source/*.cpp
	${JesusChristIn3D_SOURCE_DIR}/Engine/Source/*.c
)

include_directories(${JesusChristIn3D_SOURCE_DIR}/Engine/Source/Public)

# Define the executable
add_library(Engine STATIC ${ENGINE_HDRS} ${ENGINE_SRCS})
set_target_properties(Engine PROPERTIES LINKER_LANGUAGE CXX)

target_compile_definitions(Engine PUBLIC 
	"$<$<CONFIG:Release>:O2>" 
	"$<$<CONFIG:Release>:NDEBUG>"
)

target_compile_options(Engine PUBLIC
	"$<$<CONFIG:Release>:-w>"
	"$<$<CONFIG:Debug>:-W4>"
)

#######################################
# look for the packages that we need! #
#######################################

# OpenGL
find_package(OpenGL REQUIRED)

# GLAD
INCLUDE(${JesusChristIn3D_CMAKE_DIR}/FindGLAD.cmake)

# GLFW
INCLUDE(${JesusChristIn3D_CMAKE_DIR}/FindGLFW.cmake)

# spdlog
INCLUDE(${JesusChristIn3D_CMAKE_DIR}/FindSPDLOG.cmake)

# Define the include DIRs
include_directories(
	${JesusChristIn3D_SOURCE_DIR}/Engine/Source
	${OPENGL_INCLUDE_DIRS}
    ${GLAD_INCLUDE_DIRS}
	${GLFW_INCLUDE_DIRS}
	${SPDLOG_INCLUDE_DIRS}
)

# Define the link libraries
target_link_libraries( Engine
	${OPENGL_LIBRARIES}
    ${GLFW_LIBRARIES}
)

if(WIN32)
	string(REPLACE "/" "\\" source_path_windows "${JesusChristIn3D_SOURCE_DIR}/Engine/Content")
	string(REPLACE "/" "\\" build_path_windows "${JesusChristIn3D_BINARY_DIR}/Engine/Content")
	execute_process(COMMAND cmd.exe /c mklink /J "${build_path_windows}" "${source_path_windows}" RESULT_VARIABLE exitcode)
else()
	execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${JesusChristIn3D_SOURCE_DIR}/Engine/Content ${JesusChristIn3D_BINARY_DIR}/Engine/Content RESULT_VARIABLE exitcode)
endif()

if(NOT ${exitcode} EQUAL 0)
	MESSAGE("SYMLINKING FAILED: ${exitcode}")
	MESSAGE("FALLING BACK TO COPYING")
endif()

# Copy file if the build resource folder contains an older version. This is ignored when the symlink was succesful because the files are already the newest version.
file(GLOB_RECURSE RES RELATIVE ${JesusChristIn3D_SOURCE_DIR}/Engine/Content/ ${JesusChristIn3D_SOURCE_DIR}/Engine/Content/*.*)
foreach(file IN LISTS RES)
	configure_file(${JesusChristIn3D_SOURCE_DIR}/Engine/Content/${file} ${JesusChristIn3D_BINARY_DIR}/Engine/Content/${file} COPYONLY)
endforeach()

# Create virtual folders to make it look nicer in VS
if(MSVC_IDE)
	foreach(source IN LISTS ENGINE_SRCS ENGINE_HDRS)
		get_filename_component(source_path "${source}" PATH)
		string(REPLACE "${JesusChristIn3D_SOURCE_DIR}" "" relative_source_path "${source_path}")
		string(REPLACE "/" "\\" source_path_msvc "${relative_source_path}")
		string(REPLACE "Engine\\Source\\" "" actual_source_path_msvc "${source_path_msvc}")
		source_group("${actual_source_path_msvc}" FILES "${source}")
	endforeach()
endif()

# Make GLAD project
add_library(Glad STATIC ${GLAD_HDRS} ${GLAD_SRCS})
set_target_properties(Glad PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(Glad PUBLIC 
	"$<$<CONFIG:Release>:O2>" 
	"$<$<CONFIG:Release>:NDEBUG>"
)

target_compile_options(Glad PUBLIC
	"$<$<CONFIG:Release>:-w>"
	"$<$<CONFIG:Debug>:-W4>"
)

target_link_libraries( Engine Glad )