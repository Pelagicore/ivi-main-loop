
add_executable( ivi-main-loop-glib
	exampleGLibApp.cpp
)

TARGET_LINK_LIBRARIES( ivi-main-loop-glib
	${IVI_MAIN_LOOP_LIBRARIES}
)

install(TARGETS ivi-main-loop-glib DESTINATION ${CMAKE_INSTALL_BINDIR})
