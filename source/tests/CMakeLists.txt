
cmake_minimum_required( VERSION 2.6 )

find_package(Qt4 COMPONENTS QtCore QtGui QtXml REQUIRED)
include( ${QT_USE_FILE} )

if(WIN32)
  add_definitions( -DQT_DLL )
  link_libraries( ${QT_QTMAIN_LIBRARY} )
  set( gui_type WIN32 )
endif(WIN32)

if ( ENABLE_TESTS )

  project( testControlWidget )
  add_executable( testControlWidget testControlWidget.cpp )
  target_link_libraries( testControlWidget ${QT_LIBRARIES} ${scigraphicsQt_LIBRARY} ${scigraphics_LIBRARY} sciAppFramework )

  project( testInputWidget )
  add_executable( testInputWidget testInputWidget.cpp )
  target_link_libraries( testInputWidget ${QT_LIBRARIES} ${scigraphicsQt_LIBRARY} ${scigraphics_LIBRARY} sciAppFramework )

  project( testMainWidget )
  add_executable( testMainWidget testMainWidget.cpp )
  target_link_libraries( testMainWidget ${QT_LIBRARIES} ${scigraphicsQt_LIBRARY} ${scigraphics_LIBRARY} sciAppFramework )

  project( testMultiInputWidget )
  add_executable( testMultiInputWidget testMultiInputWidget.cpp )
  target_link_libraries( testMultiInputWidget ${QT_LIBRARIES} ${scigraphicsQt_LIBRARY} ${scigraphics_LIBRARY} sciAppFramework )
  
  project( testMultiInputWidgetXml )
  add_executable( testMultiInputWidgetXml testMultiInputWidgetXml.cpp )
  target_link_libraries( testMultiInputWidgetXml ${QT_LIBRARIES} ${scigraphicsQt_LIBRARY} ${scigraphics_LIBRARY} sciAppFramework )

  project( testOutputWidget )
  add_executable( testOutputWidget testOutputWidget.cpp )
  target_link_libraries( testOutputWidget ${QT_LIBRARIES} ${scigraphicsQt_LIBRARY} ${scigraphics_LIBRARY} sciAppFramework )

  project( testCatalogueWidget )
  add_executable( testCatalogueWidget testCatalogueWidget.cpp )
  target_link_libraries( testCatalogueWidget ${QT_LIBRARIES} ${scigraphicsQt_LIBRARY} ${scigraphics_LIBRARY} sciAppFramework )

  project( testWaitWidget )
  add_executable( testWaitWidget testWaitWidget.cpp )
  target_link_libraries( testWaitWidget ${QT_LIBRARIES} ${scigraphicsQt_LIBRARY} ${scigraphics_LIBRARY} sciAppFramework )

endif()

