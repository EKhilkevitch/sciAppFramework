
cmake_minimum_required( VERSION 2.6 )

project( sciAppFramework ) 

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release )
endif()

set( EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin/" )
set( LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/lib/" )

macro( check_found libname )
  if ( ${libname} MATCHES "${libname}-NOTFOUND" )
  else()
    set( ${libname}_FOUND 1 )
  endif()
endmacro()

if ( NOT LIBRARY_TYPE )
  set( LIBRARY_TYPE STATIC )
endif()

if ( NOT SCIGRAPHICS_DIR )
  set( SCIGRAPHICS_DIR "${PROJECT_SOURCE_DIR}/../scigraphics/" )
endif()

message( "LIBRARY_TYPE = ${LIBRARY_TYPE}" )
message( "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}" )
message( "SCIGRAPHICS_DIR = ${SCIGRAPHICS_DIR}" )

set( scigraphics_INCLUDE "${SCIGRAPHICS_DIR}/source/" )
find_library( scigraphics_LIBRARY   NAMES scigraphics   PATHS "${SCIGRAPHICS_DIR}/lib/" )
find_library( scigraphicsQt_LIBRARY NAMES scigraphicsQt PATHS "${SCIGRAPHICS_DIR}/lib/" )
#check_found( scigraphics_LIBRARY )
#check_found( scigraphicsQt_LIBRARY )

include_directories( "./source/" )

if ( UNIX )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -Wextra ") 
  set( CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fPIC -Wall -Wextra ") 
endif()

add_subdirectory( ./source/sciAppFramework/ )
add_subdirectory( ./source/examples/ )

