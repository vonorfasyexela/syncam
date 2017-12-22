cmake_minimum_required( VERSION 2.6 )
##############################################################################
project( syncam )
set( SOURCE_FILES syncam.c )

# if you need libraries that pkg-config can find set ON instead OFF
option( USE_GSTREAMER         "Want GStreamer?"         ON )

add_compile_options( -g )
add_compile_options( -Wall )
add_compile_options( -std=c11 )
##############################################################################

add_executable( ${PROJECT_NAME} ${SOURCE_FILES} )

if ( USE_GSTREAMER )
    find_package( PkgConfig REQUIRED )
    pkg_search_module( GSTREAMER REQUIRED gstreamer-1.0 )
    include_directories( ${GSTREAMER_INCLUDE_DIRS} )
    link_directories( ${GSTREAMER_LIBRARY_DIRS} )
    target_compile_options( ${PROJECT_NAME} PRIVATE ${GSTREAMER_CFLAGS} )
    target_link_libraries( ${PROJECT_NAME} ${GSTREAMER_LIBRARIES} )
endif ( USE_GSTREAMER )