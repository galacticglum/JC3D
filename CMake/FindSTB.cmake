set(STB_INCLUDE_DIRS "${JesusChristIn3D_Engine_DEPEND_DIR}/stb/include")

# Check if we found it!
IF ( STB_INCLUDE_DIRS )
	SET( STB_FOUND TRUE )
	MESSAGE( STATUS "Looking for stb - found" )
ELSE ( STB_INCLUDE_DIRS )
	SET( STB_FOUND FALSE )
	MESSAGE( STATUS "Looking for stb - not found" )
ENDIF ( STB_INCLUDE_DIRS )