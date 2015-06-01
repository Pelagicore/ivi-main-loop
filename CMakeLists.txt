
cmake_minimum_required(VERSION 2.6)

set(PACKAGE ivi-main-loop)
PROJECT(${PACKAGE})

include(GNUInstallDirs)

SET( ${PROJECT_NAME}_MAJOR_VERSION 1 )
SET( ${PROJECT_NAME}_MINOR_VERSION 0 )
SET( ${PROJECT_NAME}_PATCH_LEVEL 0 )

set(VERSION ${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_LEVEL})
set(PACKAGE_VERSION ${VERSION})

# Dependencies
FIND_PACKAGE(PkgConfig REQUIRED)

OPTION( USE_IVI_LOGGING "Use ivi-logging" OFF )
if (USE_IVI_LOGGING)
    FIND_PACKAGE(IVILogging REQUIRED)
    set(LOGGING_LIBRARIES ${IVILOGGING_LIBRARIES})
    add_definitions(-DUSE_IVI_LOGGING)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

include_directories(
	include
	src
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)

OPTION( ENABLE_GLIB_BACKEND "Set GLIB as default backend" ON )

if (ENABLE_GLIB_BACKEND)
    pkg_check_modules(GLIB REQUIRED glib-2.0)
    add_definitions(${GLIB_CFLAGS_OTHER})
    link_directories(${GLIB_LIBRARY_DIRS} )
    include_directories(${GLIB_INCLUDE_DIRS} )
    set(DEFAULT_BACKEND glib)
    set(DEFAULT_BACKEND_CLASS GLibEventDispatcherTypes)
    set(ADDITIONAL_PKGCONFIG_DEPENDENCIES glib-2.0)
else()
endif()

configure_file(ivi-main-loop-config.h.in ivi-main-loop-config.h @ONLY)

configure_file(ivi-main-loop.pc.in ivi-main-loop.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ivi-main-loop.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

set(DEVELOPMENT_INCLUDE_PATH " -I${CMAKE_CURRENT_SOURCE_DIR}/include -I${CMAKE_CURRENT_BINARY_DIR} #")
set(DEVELOPMENT_LIBRARY_PATH " -L${CMAKE_CURRENT_BINARY_DIR}/src/glib #" )
configure_file(ivi-main-loop.pc.in ivi-main-loop-uninstalled.pc @ONLY)
if(INSTALL_PKGCONFIG_UNINSTALLED_FILE)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ivi-main-loop-uninstalled.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
endif()


add_subdirectory(include)

set(IVI_MAIN_LOOP_LIBRARIES ivi-main-loop)

set(PATHS_DEFINED 1)
add_subdirectory(examples)

add_subdirectory (src)

add_subdirectory (doc)
