
add_executable( ivi-main-loop-example
	exampleMainLoop.cpp
)

TARGET_LINK_LIBRARIES( ivi-main-loop-example
	${IVI_MAIN_LOOP_LIBRARIES}
)

install(TARGETS ivi-main-loop-example DESTINATION ${CMAKE_INSTALL_BINDIR})
