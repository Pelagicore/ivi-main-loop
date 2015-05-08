
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
  
endif()

add_subdirectory(basic)
add_subdirectory(basic_optimized)
