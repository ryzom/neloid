FILE(GLOB SRC *.cpp *.h)

ADD_EXECUTABLE(neloid WIN32 ${SRC})

SOURCE_GROUP(Level FILES level_manager.h level_manager.cpp level.h level.cpp level_editor.h level_editor.cpp)
SOURCE_GROUP(Level\\Tiles FILES	tile.h 
								tile.cpp
								tile_weak.h
								tile_weak.cpp
								tile_start.h
								tile_start.cpp
								tile_portal.h
								tile_portal.cpp
								tile_normal.h
								tile_normal.cpp
								tile_goal.h
								tile_goal.cpp
								)
SOURCE_GROUP(Utilities FILES xml_helpers.cpp xml_helpers.h neloid_exceptions.h)

INCLUDE_DIRECTORIES(${LIBXML2_INCLUDE_DIR} ${NEL_INCLUDE_DIR} ${CEGUI_INCLUDE_DIRS} ${PNG_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(neloid
		${PLATFORM_LINKFLAGS} 
		${LIBXML2_LIBRARIES} 
		${PNG_LIBRARIES}
		${FREETYPE_LIBRARY} 
		${JPEG_LIBRARY}
		${NELMISC_LIBRARY}
		${NELNET_LIBRARY}
		${NELSOUND_LIBRARY}
		${NELSNDDRV_LIBRARY}
		${NEL3D_LIBRARY}
		${CEGUI_LIBRARY})
IF(WIN32)
  SET_TARGET_PROPERTIES(neloid PROPERTIES 
	LINK_FLAGS_DEBUG "/NODEFAULTLIB:libc;libcmt;libcmtd;msvcrt"
	LINK_FLAGS_RELEASE "/NODEFAULTLIB:libc;libcmt;libcmtd;msvcrtd"
	PROJECT_LABEL "Game: Neloid")
ENDIF(WIN32)
ADD_DEFINITIONS(${LIBXML2_DEFINITIONS})

INSTALL(TARGETS neloid RUNTIME DESTINATION bin COMPONENT runtime)
