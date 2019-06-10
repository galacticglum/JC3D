set(GLFW_INCLUDE_DIRS "${JesusChristIn3D_Engine_DEPEND_DIR}/glfw/include")

# set(GLFW_LIBRARIES "${JesusChristIn3D_DEPEND_DIR}/glfw/lib/${_config}/x64/$<$<CONFIG:Debug>:glfw3-debug.lib>$<$<CONFIG:Release>:glfw3.lib>")
set(GLFW_LIBRARIES "${JesusChristIn3D_Engine_DEPEND_DIR}/glfw/lib/${_config}/x64/glfw3.lib")

# Check if we found it!
IF ( GLFW_INCLUDE_DIRS AND GLFW_LIBRARIES )
	SET( GLFW_FOUND TRUE )
	MESSAGE( STATUS "Looking for GLFW - found" )
ELSE ( GLFW_INCLUDE_DIRS AND GLFW_LIBRARIES )
	SET( GLFW_FOUND FALSE )
	MESSAGE( STATUS "Looking for GLFW - not found" )
ENDIF ( GLFW_INCLUDE_DIRS AND GLFW_LIBRARIES )