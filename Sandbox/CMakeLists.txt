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

file(GLOB_RECURSE SANDBOX_HDRS
	${JesusChristIn3D_SOURCE_DIR}/Sandbox/Source/*.h
)

file(GLOB_RECURSE SANDBOX_SRCS
	${JesusChristIn3D_SOURCE_DIR}/Sandbox/Source/*.cpp
	${JesusChristIn3D_SOURCE_DIR}/Sandbox/Source/*.c
)

include_directories(${JesusChristIn3D_SOURCE_DIR}/Sandbox/Source/Public)

# Define the executable
add_executable(Sandbox ${SANDBOX_HDRS} ${SANDBOX_SRCS})
target_compile_definitions(Sandbox PUBLIC 
	"$<$<CONFIG:Release>:O2>" 
	"$<$<CONFIG:Release>:NDEBUG>"
)

target_compile_options(Sandbox PUBLIC
	"$<$<CONFIG:Release>:-w>"
	"$<$<CONFIG:Debug>:-W4>"
)

# GLAD
INCLUDE(${JesusChristIn3D_CMAKE_DIR}/FindGLAD.cmake)

# GLFW
INCLUDE(${JesusChristIn3D_CMAKE_DIR}/FindGLFW.cmake)

# spdlog
INCLUDE(${JesusChristIn3D_CMAKE_DIR}/FindSPDLOG.cmake)

# imgui
INCLUDE(${JesusChristIn3D_CMAKE_DIR}/FindImGui.cmake)

# Define the include DIRs
include_directories(
	${JesusChristIn3D_SOURCE_DIR}/Engine/Source/Public
	${OPENGL_INCLUDE_DIRS}
	${GLFW_INCLUDE_DIRS}
	${GLAD_INCLUDE_DIRS}
	${SPDLOG_INCLUDE_DIRS}
	${ImGui_INCLUDE_DIRS}
)

target_link_libraries( Sandbox Engine )
set_target_properties( Sandbox PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Content")

if(WIN32)
	string(REPLACE "/" "\\" source_path_windows "${JesusChristIn3D_SOURCE_DIR}/Sandbox/Content")
	string(REPLACE "/" "\\" build_path_windows "${JesusChristIn3D_BINARY_DIR}/Sandbox/Content")
	execute_process(COMMAND cmd.exe /c mklink /J "${build_path_windows}" "${source_path_windows}" RESULT_VARIABLE exitcode)
else()
	execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${JesusChristIn3D_SOURCE_DIR}/Sandbox/Content ${JesusChristIn3D_BINARY_DIR}/Sandbox/Content RESULT_VARIABLE exitcode)
endif()

if(NOT ${exitcode} EQUAL 0)
	MESSAGE("SYMLINKING FAILED: ${exitcode}")
	MESSAGE("FALLING BACK TO COPYING")
endif()

# Copy file if the build resource folder contains an older version. 
# This is ignored when the symlink was succesful because the files are already the newest version.
file(GLOB_RECURSE RES RELATIVE ${JesusChristIn3D_SOURCE_DIR}/Sandbox/Content/ ${JesusChristIn3D_SOURCE_DIR}/Sandbox/Content/*.*)
foreach(file IN LISTS RES)
	configure_file(${JesusChristIn3D_SOURCE_DIR}/Sandbox/Content/${file} ${JesusChristIn3D_BINARY_DIR}/Sandbox/Content/${file} COPYONLY)
endforeach()

# Create virtual folders to make it look nicer in VS
if(MSVC_IDE)
	foreach(source IN LISTS SANDBOX_SRCS SANDBOX_HDRS)
		get_filename_component(source_path "${source}" PATH)
		string(REPLACE "${JesusChristIn3D_SOURCE_DIR}" "" relative_source_path "${source_path}")
		string(REPLACE "/" "\\" source_path_msvc "${relative_source_path}")
		string(REPLACE "Sandbox\\Source\\" "" actual_source_path_msvc "${source_path_msvc}")
		source_group("${actual_source_path_msvc}" FILES "${source}")
	endforeach()
endif()