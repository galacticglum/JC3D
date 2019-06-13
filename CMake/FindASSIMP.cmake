set(ASSIMP_INCLUDE_DIRS "${JesusChristIn3D_Engine_DEPEND_DIR}/assimp/include")
set(ASSIMP_LIBRARIES "${JesusChristIn3D_Engine_DEPEND_DIR}/assimp/lib/${_config}/x64/$<$<CONFIG:Debug>:assimp-vc140-mtd.lib>$<$<CONFIG:Release>:assimp-vc140-mt.lib>")

# Check if we found it!
IF ( ASSIMP_INCLUDE_DIRS AND ASSIMP_LIBRARIES )
	SET( ASSIMP_FOUND TRUE )
	MESSAGE( STATUS "Looking for ASSIMP - found" )
ELSE ( ASSIMP_INCLUDE_DIRS AND ASSIMP_LIBRARIES )
	SET( ASSIMP_FOUND FALSE )
	MESSAGE( STATUS "Looking for ASSIMP - not found" )
ENDIF ( ASSIMP_INCLUDE_DIRS AND ASSIMP_LIBRARIES )