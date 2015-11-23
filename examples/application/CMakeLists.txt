
add_executable( ivi-main-loop-application-core
	exampleApplicationCore.cpp
)

TARGET_LINK_LIBRARIES( ivi-main-loop-application-core
	${IVI_MAIN_LOOP_LIBRARIES}
)

install(TARGETS ivi-main-loop-application-core DESTINATION ${CMAKE_INSTALL_BINDIR})
