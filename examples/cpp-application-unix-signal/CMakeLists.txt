
add_executable( ivi-main-loop-example-unix-signal
	exampleMainLoopUnixSignal.cpp
)

TARGET_LINK_LIBRARIES( ivi-main-loop-example-unix-signal
	${IVI_MAIN_LOOP_LIBRARIES}
)

install(TARGETS ivi-main-loop-example-unix-signal DESTINATION ${CMAKE_INSTALL_BINDIR})
