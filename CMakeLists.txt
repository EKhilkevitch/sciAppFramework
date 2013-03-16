
cmake_minimum_required( VERSION 2.6 )

project( sciAppFramework ) 

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release )
endif()

set( EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin/" )
set( LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/lib/" )

include_directories( "./source/" )

macro( check_found libname )
  if ( ${libname} MATCHES "${libname}-NOTFOUND" )
  else()
    set( ${libname}_FOUND 1 )
  endif()
endmacro()

if ( NOT LIBRARY_TYPE )
  set( LIBRARY_TYPE STATIC )
endif()

message( "LIBRARY_TYPE = ${LIBRARY_TYPE}" )
message( "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}" )

set( graphics_dir "${PROJECT_SOURCE_DIR}/../scigraphics/" )
include_directories( "${graphics_dir}/source/" )
find_library( scigraphics_LIBRARY   NAMES scigraphics   PATHS "${graphics_dir}/lib/" )
check_found( scigraphics_LIBRARY )
find_library( scigraphicsQt_LIBRARY NAMES scigraphicsQt PATHS "${graphics_dir}/lib/" )
check_found( scigraphicsQt_LIBRARY )

if ( UNIX )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC") 
  set( CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fPIC") 
endif()

if ( UNIX )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall") 
endif()

add_subdirectory( ./source/sciAppFramework/ )
add_subdirectory( ./source/tests/ )

if ( WIN32 )
  add_custom_command( COMMAND "sed -i 's/machine:[a-z0-9]*/machine:x86/g" TARGET Makefile )
endif()


