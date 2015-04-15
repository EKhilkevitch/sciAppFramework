
cmake_minimum_required( VERSION 2.6 )

find_package(Qt4 COMPONENTS QtCore QtGui QtXml REQUIRED)
include( ${QT_USE_FILE} )

if(WIN32)
  link_libraries( ${QT_QTMAIN_LIBRARY} )
  set( gui_type WIN32 )
endif(WIN32)

if ( ENABLE_EXAMPLES )
  
  message( "Examples enabled" )
  
  project( testControlWidget )
  add_executable( testControlWidget testControlWidget.cpp )
  target_link_libraries( testControlWidget sciAppFramework ${scigraphicsQt_LIBRARY} ${scigraphics_LIBRARY} ${QT_LIBRARIES} )

  project( testInputWidget )
  add_executable( testInputWidget testInputWidget.cpp )
  target_link_libraries( testInputWidget sciAppFramework ${scigraphicsQt_LIBRARY} ${scigraphics_LIBRARY} ${QT_LIBRARIES} )

  project( testMainWidget )
  add_executable( testMainWidget testMainWidget.cpp )
  target_link_libraries( testMainWidget sciAppFramework ${scigraphicsQt_LIBRARY} ${scigraphics_LIBRARY} ${QT_LIBRARIES} )

  project( testMultiInputWidget )
  add_executable( testMultiInputWidget testMultiInputWidget.cpp )
  target_link_libraries( testMultiInputWidget sciAppFramework ${scigraphicsQt_LIBRARY} ${scigraphics_LIBRARY} ${QT_LIBRARIES} )
  
  project( testMultiInputWidgetXml )
  add_executable( testMultiInputWidgetXml testMultiInputWidgetXml.cpp )
  target_link_libraries( testMultiInputWidgetXml sciAppFramework ${scigraphicsQt_LIBRARY} ${scigraphics_LIBRARY} ${QT_LIBRARIES} )

  project( testOutputWidget )
  add_executable( testOutputWidget testOutputWidget.cpp )
  target_link_libraries( testOutputWidget sciAppFramework ${scigraphicsQt_LIBRARY} ${scigraphics_LIBRARY} ${QT_LIBRARIES} )

  project( testCatalogueWidget )
  add_executable( testCatalogueWidget testCatalogueWidget.cpp )
  target_link_libraries( testCatalogueWidget sciAppFramework ${scigraphicsQt_LIBRARY} ${scigraphics_LIBRARY} ${QT_LIBRARIES} )

  project( testWaitWidget )
  add_executable( testWaitWidget testWaitWidget.cpp )
  target_link_libraries( testWaitWidget sciAppFramework ${scigraphicsQt_LIBRARY} ${scigraphics_LIBRARY} ${QT_LIBRARIES} )
  
  project( testStructInputConnection )
  add_executable( testStructInputConnection testStructInputConnection.cpp )
  target_link_libraries( testStructInputConnection sciAppFramework ${scigraphicsQt_LIBRARY} ${scigraphics_LIBRARY} ${QT_LIBRARIES} )

else()
  message( "Examples disabed (use -DENABLE_EXAMPLES=1)" )
endif()

