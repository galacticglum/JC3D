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

# assimp
INCLUDE(${JesusChristIn3D_CMAKE_DIR}/FindASSIMP.cmake)

# Define the include DIRs
include_directories(
	${JesusChristIn3D_SOURCE_DIR}/Engine/Source/Public
	${OPENGL_INCLUDE_DIRS}
	${GLFW_INCLUDE_DIRS}
	${GLAD_INCLUDE_DIRS}
	${SPDLOG_INCLUDE_DIRS}
	${ImGui_INCLUDE_DIRS}
	${ASSIMP_INCLUDE_DIRS}
)

target_link_libraries( Sandbox Engine )

# Set the output directory for sandbox to make managing directories easier
set (SANDBOX_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/bin")
set_target_properties( Sandbox PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${SANDBOX_OUT_DIR})

set_property(TARGET Sandbox PROPERTY CXX_STANDARD 17)
set_property(TARGET Sandbox PROPERTY CXX_STANDARD_REQUIRED ON)

# Copy content to output directory
add_custom_command(TARGET Sandbox PRE_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy_directory
				   ${JesusChristIn3D_SOURCE_DIR}/Sandbox/Content $<TARGET_FILE_DIR:Sandbox>/Content)

# Copy binaries to output directory...
# Find all dependencies that have a bin folder...
include(${JesusChristIn3D_CMAKE_DIR}/utils.cmake)
SUBDIRLIST(DEPENDENCY_DIRS ${JesusChristIn3D_Engine_DEPEND_DIR})
foreach(dependency_name ${DEPENDENCY_DIRS})
	# check if there is a bin directory
	if(EXISTS "${JesusChristIn3D_Engine_DEPEND_DIR}/${dependency_name}/bin")
		# there is a bin directory which means that we should copy this dependency
		add_custom_command(TARGET Sandbox PRE_BUILD
						   COMMAND ${CMAKE_COMMAND} -E copy_directory
						   "${JesusChristIn3D_Engine_DEPEND_DIR}/${dependency_name}/bin/${_config}/$<$<EQUAL:4,${CMAKE_SIZEOF_VOID_P}>:Win32>$<$<EQUAL:8,${CMAKE_SIZEOF_VOID_P}>:x64>" $<TARGET_FILE_DIR:Sandbox>)
	endif()
endforeach()

# Set the working directory
file( WRITE "${CMAKE_CURRENT_BINARY_DIR}/Sandbox.vcxproj.user" 
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>     \
    <Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">
    <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">
        <LocalDebuggerWorkingDirectory>$(OutDir)</LocalDebuggerWorkingDirectory>
        <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
    </PropertyGroup>
    <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">
        <LocalDebuggerWorkingDirectory>$(OutDir)</LocalDebuggerWorkingDirectory>
        <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
    </PropertyGroup>
	</Project>"
)

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