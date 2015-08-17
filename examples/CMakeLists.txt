
cmake_minimum_required(VERSION 2.8)

PROJECT(MainLoopExamples)

include(GNUInstallDirs)

if(NOT PATHS_DEFINED)

    # Dependencies
    FIND_PACKAGE(PkgConfig REQUIRED)
    
    pkg_check_modules(IVI_MAIN_LOOP REQUIRED ivi-main-loop)
    add_definitions(${IVI_MAIN_LOOP_CFLAGS_OTHER})
    include_directories(${IVI_MAIN_LOOP_INCLUDE_DIRS})
    link_directories(${IVI_MAIN_LOOP_LIBRARY_DIRS} )

else()
    
    include_directories(${CMAKE_SOURCE_DIR}/include)
endif()

add_subdirectory(cpp-application)
add_subdirectory(cpp-application-unix-signal)
add_subdirectory(glib)
add_subdirectory(c-application)
#add_subdirectory(basic_optimized)
