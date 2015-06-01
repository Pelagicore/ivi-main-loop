
add_executable( ivi-main-loop-example-c
	exampleMainLoop.c
)

TARGET_LINK_LIBRARIES( ivi-main-loop-example-c
	${IVI_MAIN_LOOP_LIBRARIES}
)

install(TARGETS ivi-main-loop-example-c DESTINATION ${CMAKE_INSTALL_BINDIR})
