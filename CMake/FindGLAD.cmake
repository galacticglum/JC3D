set(GLAD_INCLUDE_DIRS "${JesusChristIn3D_Engine_DEPEND_DIR}/glad/include")
set(GLAD_SOURCE_DIRS "${JesusChristIn3D_Engine_DEPEND_DIR}/glad/src")

file(GLOB_RECURSE GLAD_HDRS
	${GLAD_INCLUDE_DIRS}/*.h
	${GLAD_INCLUDE_DIRS}/*.hpp
)

file(GLOB_RECURSE GLAD_SRCS
	${GLAD_INCLUDE_DIRS}/*.c
	${GLAD_INCLUDE_DIRS}/*.cpp
	${GLAD_SOURCE_DIRS}/*.c
	${GLAD_SOURCE_DIRS}/*.cpp
)

# Check if we found it!
IF ( GLAD_INCLUDE_DIRS AND GLAD_SOURCE_DIRS )
	SET( GLAD_FOUND TRUE )
	MESSAGE( STATUS "Looking for GLAD - found" )
ELSE ( GLAD_INCLUDE_DIRS AND GLAD_SOURCE_DIRS )
	SET( GLAD_FOUND FAlSE )
	MESSAGE( STATUS "Looking for GLAD - not found" )
ENDIF ( GLAD_INCLUDE_DIRS AND GLAD_SOURCE_DIRS )