set(FMT_INCLUDE_DIRS "${JesusChristIn3D_Engine_DEPEND_DIR}/fmt/include")

# Check if we found it!
IF ( FMT_INCLUDE_DIRS )
	SET( FMT_FOUND TRUE )
	MESSAGE( STATUS "Looking for FMT - found" )
ELSE ( FMT_INCLUDE_DIRS )
	SET( FMT_FOUND FALSE )
	MESSAGE( STATUS "Looking for FMT - not found" )
ENDIF ( FMT_INCLUDE_DIRS )