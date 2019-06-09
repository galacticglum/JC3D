set(SPDLOG_INCLUDE_DIRS "${JesusChristIn3D_Engine_DEPEND_DIR}/spdlog/include")

# Check if we found it!
IF ( SPDLOG_INCLUDE_DIRS )
	SET( SPDLOG_FOUND TRUE )
	MESSAGE( STATUS "Looking for spdlog - found" )
ELSE ( SPDLOG_INCLUDE_DIRS )
	SET( SPDLOG_FOUND FALSE )
	MESSAGE( STATUS "Looking for spdlog - not found" )
ENDIF ( SPDLOG_INCLUDE_DIRS )