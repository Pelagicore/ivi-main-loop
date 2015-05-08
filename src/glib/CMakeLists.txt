
add_library( ivi-main-loop SHARED
	ivi-main-loop-glib.cpp
)

set(INCLUDE_FOLDER ../../include)
include_directories(${INCLUDE_FOLDER})

set_target_properties(ivi-main-loop PROPERTIES VERSION ${VERSION} SOVERSION ${${PROJECT_NAME}_MAJOR_VERSION})

TARGET_LINK_LIBRARIES( ivi-main-loop
   ${LOGGING_LIBRARIES}
   ${GLIB_LIBRARIES}
)

INSTALL(TARGETS ivi-main-loop
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

set(INCLUDE_FILES ${INCLUDE_FOLDER}/glib/ivi-main-loop-glib.h)

INSTALL(FILES ${INCLUDE_FILES}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PACKAGE}/glib
)
