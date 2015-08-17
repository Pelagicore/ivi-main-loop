
cmake_minimum_required(VERSION 2.6)

PROJECT(ivi-main-loop)

include(GNUInstallDirs)

set(PACKAGE_NAME ${CMAKE_PROJECT_NAME}) 

set(API_MAJOR_VERSION 0)
set(PACKAGE_VERSION ${API_MAJOR_VERSION}.1)

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

configure_file(${PACKAGE_NAME}.pc.in ${PACKAGE_NAME}.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

set(UNINSTALLED_INCLUDE_PATH " -I${CMAKE_CURRENT_SOURCE_DIR}/include -I${CMAKE_CURRENT_BINARY_DIR} #")
set(UNINSTALLED_LIBRARY_PATH " -L${CMAKE_CURRENT_BINARY_DIR}/src/glib #" )
configure_file(${PACKAGE_NAME}.pc.in ${PACKAGE_NAME}-uninstalled.pc @ONLY)
if(INSTALL_PKGCONFIG_UNINSTALLED_FILE)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}-uninstalled.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
endif()


add_subdirectory(include)

set(IVI_MAIN_LOOP_LIBRARIES ivi-main-loop)

set(PATHS_DEFINED 1)
add_subdirectory(examples)

add_subdirectory (src)

add_subdirectory (doc)
