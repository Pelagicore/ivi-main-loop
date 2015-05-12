
add_executable( ivi-main-loop-example-optimized
	exampleMainLoop.cpp
)

TARGET_LINK_LIBRARIES( ivi-main-loop-example-optimized
	${IVI_MAIN_LOOP_LIBRARIES}
)

install(TARGETS ivi-main-loop-example-optimized DESTINATION ${CMAKE_INSTALL_BINDIR})
