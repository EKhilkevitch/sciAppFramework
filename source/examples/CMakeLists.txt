
cmake_minimum_required( VERSION 2.6 )

if ( ENABLE_EXAMPLES )

  find_package(Qt4 COMPONENTS QtCore QtGui QtXml QUIET )
  find_package( Qt5 COMPONENTS Widgets Xml LinguistTools QUIET )

  if(WIN32)
    set( gui_type WIN32 )
  endif(WIN32)

  if ( ${Qt4_FOUND} )
    if ( WIN32 )
      set( QtLibs ${QT_LIBRARIES} ${QT_QTMAIN_LIBRARY} )
    else()
      set( QtLibs ${QT_LIBRARIES} )
    endif()
    include( ${QT_USE_FILE} )
  elseif ( ${Qt5_FOUND} ) 
    if ( WIN32 )
      set( QtLibs Qt5::Widgets Qt5::Xml )
    else()
      set( QtLibs Qt5::Widgets Qt5::Xml )
    endif()
  endif()  

  message( "Examples enabled" )
  
  project( testControlWidget )
  add_executable( testControlWidget testControlWidget.cpp )
  target_link_libraries( testControlWidget sciAppFramework sciMeashurementFramework ${QtLibs} )

  project( testInputWidget )
  add_executable( testInputWidget testInputWidget.cpp )
  target_link_libraries( testInputWidget sciAppFramework ${QtLibs} )

  project( testMainWidget )
  add_executable( testMainWidget testMainWidget.cpp )
  target_link_libraries( testMainWidget sciMeashurementFramework sciAppFramework ${scigraphicsQt_LIBRARY} ${scigraphics_LIBRARY} ${QtLibs} )
  target_include_directories( testMainWidget PRIVATE ${scigraphics_INCLUDE} )

  project( testMultiInputWidget )
  add_executable( testMultiInputWidget testMultiInputWidget.cpp )
  target_link_libraries( testMultiInputWidget sciAppFramework ${QtLibs} )
  
  project( testMultiInputWidgetXml )
  add_executable( testMultiInputWidgetXml testMultiInputWidgetXml.cpp )
  target_link_libraries( testMultiInputWidgetXml sciAppFramework ${QtLibs} )

  project( testOutputWidget )
  add_executable( testOutputWidget testOutputWidget.cpp )
  target_link_libraries( testOutputWidget sciMeashurementFramework sciAppFramework ${scigraphicsQt_LIBRARY} ${scigraphics_LIBRARY} ${QtLibs} )

  project( testCatalogueWidget )
  add_executable( testCatalogueWidget testCatalogueWidget.cpp )
  target_link_libraries( testCatalogueWidget sciAppFramework ${QtLibs} )

  project( testWaitWidget )
  add_executable( testWaitWidget testWaitWidget.cpp )
  target_link_libraries( testWaitWidget sciAppFramework ${QtLibs} )
  
  project( testStructInputConnection )
  add_executable( testStructInputConnection testStructInputConnection.cpp )
  target_link_libraries( testStructInputConnection sciAppFramework ${QtLibs} )

  project( testXmlSettingsStorage )
  add_executable( testXmlSettingsStorage testXmlSettingsStorage.cpp )
  target_link_libraries( testXmlSettingsStorage sciAppFramework ${QtLibs} )

else()
  message( "Examples disabed (use -DENABLE_EXAMPLES=1)" )
endif()

