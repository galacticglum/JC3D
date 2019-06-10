include(${JesusChristIn3D_CMAKE_DIR}/utils.cmake)

set(ImGui_INCLUDE_DIRS "${JesusChristIn3D_Engine_DEPEND_DIR}/imgui/include")
set(ImGui_SOURCE_DIRS "${JesusChristIn3D_Engine_DEPEND_DIR}/imgui/src")

file(GLOB_RECURSE ImGui_HDRS
	${ImGui_INCLUDE_DIRS}/*.h
	${ImGui_INCLUDE_DIRS}/*.hpp
)

file(GLOB_RECURSE ImGui_SRCS
	${ImGui_INCLUDE_DIRS}/*.cpp
    ${ImGui_INCLUDE_DIRS}/*.c
    ${ImGui_SOURCE_DIRS}/*.cpp
	${ImGui_SOURCE_DIRS}/*.c
)

# Check if we found it!
IF ( ImGui_INCLUDE_DIRS AND ImGui_SOURCE_DIRS )
	SET( ImGui_FOUND TRUE )

	# Remove any example files from the srcs
	filter_items(ImGui_SRCS ".*examples.*")

	MESSAGE( STATUS "Looking for ImGui - found" )
ELSE ( ImGui_INCLUDE_DIRS AND ImGui_SOURCE_DIRS )
	SET( ImGui_FOUND FAlSE )
	MESSAGE( STATUS "Looking for ImGui - not found" )
ENDIF ( ImGui_INCLUDE_DIRS AND ImGui_SOURCE_DIRS )