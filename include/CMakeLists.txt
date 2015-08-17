
set(INCLUDE_FILES
	ivi-main-loop.h
	ivi-main-loop-c-types.h
	ivi-main-loop-c.h
	ivi-main-loop-common.h
	ivi-main-loop-log.h
	${CMAKE_CURRENT_BINARY_DIR}/../ivi-main-loop-config.h
)

install(FILES ${INCLUDE_FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PACKAGE_NAME})

