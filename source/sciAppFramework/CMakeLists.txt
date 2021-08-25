
cmake_minimum_required( VERSION 2.6 )


if( POLICY CMP0012 )
  cmake_policy(SET CMP0012 NEW)
endif()

find_package( Qt5 COMPONENTS Widgets Xml QUIET )
if ( NOT ${Qt5_FOUND} )
  find_package( Qt4 COMPONENTS QtCore QtGui QtXml QUIET )
endif()

project( sciAppFramework ) 

set ( sciAppFramework_source_files
                settingsStorage.cpp
                settingsObject.cpp
                inputWidget.cpp 
                labelInputWidget.cpp
                adaptedInputWidget.cpp
                multiInputWidget.cpp    
                multiInputXmlFactory.cpp
                okcancelDialog.cpp      
                stackedSelectorWidget.cpp
                catalogueWidget.cpp
                waitWidget.cpp
                layoutOperations.cpp
                longSpinBox.cpp
                structInputConnection.cpp
  )

set( sciAppFramework_source_moc_files 
                  inputWidget.h                   
                  labelInputWidget.h
                  adaptedInputWidget.h
                  multiInputWidget.h              
                  catalogueWidget.h
                  waitWidget.h
                  longSpinBox.h
    )


if ( ${Qt4_FOUND} )
  message( "Qt4 found - sciAppFramework" )

  include( ${QT_USE_FILE} )

  qt4_wrap_cpp( sciAppFramework_moc_sources ${sciAppFramework_source_moc_files} )
  add_library( sciAppFramework ${LIBRARY_TYPE} ${sciAppFramework_source_files} ${sciAppFramework_moc_sources} )
  target_link_libraries( sciAppFramework ${QT_LIBRARIES} )
elseif ( ${Qt5_FOUND} )
  message( "Qt5 found - sciAppFramework" )

  qt5_wrap_cpp( sciAppFramework_moc_sources ${sciAppFramework_source_moc_files} )
  
  add_library( sciAppFramework ${LIBRARY_TYPE} ${sciAppFramework_source_files} ${sciAppFramework_moc_sources} )
  target_link_libraries( sciAppFramework Qt5::Widgets Qt5::Xml )
  target_include_directories( sciAppFramework PRIVATE ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Xml_INCLUDE_DIRS} )
else()
  message( "No Qt found - sciAppFramework" )
endif()

project( sciMeashurementFramework )

set ( sciMeashurementFramework_source_files
                measurementParameters.cpp
                measurementThread.cpp
                outputWidget.cpp        
                outputSettingsWidget.cpp
                controlWidget.cpp
                measureControlWidget.cpp
                measureMainWidget.cpp
                mainWidget.cpp
  )

set( sciMeashurementFramework_source_moc_files
                measurementThread.h             
                outputWidget.h                  
                outputSettingsWidget.h                
                controlWidget.h                 
                measureControlWidget.h                 
                measureMainWidget.h
  )

if ( ${Qt4_FOUND} )
  message( "Qt4 found - sciMeashurementFramework" )

  include( ${QT_USE_FILE} )

  qt4_wrap_cpp( sciMeashurementFramework_moc_sources ${sciMeashurementFramework_source_moc_files} )
  add_library( sciMeashurementFramework ${LIBRARY_TYPE} ${sciMeashurementFramework_source_files} ${sciMeashurementFramework_moc_sources} )
  if ( NOT ${scigraphics_LIBRARY} MATCHES "" )
    target_link_libraries( sciMeashurementFramework sciAppFramework ${scigraphics_LIBRARY} ${scigraphicsQt_LIBRARY} )
  endif()
  target_include_directories( sciMeashurementFramework PRIVATE ${scigraphics_INCLUDE} )

elseif ( ${Qt5_FOUND} )
  message( "Qt5 found - sciMeashurementFramework" )

  qt5_wrap_cpp( sciMeashurementFramework_moc_sources ${sciMeashurementFramework_source_moc_files} )
  add_library( sciMeashurementFramework ${LIBRARY_TYPE} ${sciMeashurementFramework_source_files} ${sciMeashurementFramework_moc_sources} )
  if ( NOT ${scigraphics_LIBRARY} MATCHES "" )
    target_link_libraries( sciMeashurementFramework sciAppFramework ${scigraphics_LIBRARY} ${scigraphicsQt_LIBRARY} )
  endif()
  target_include_directories( sciMeashurementFramework PRIVATE ${scigraphics_INCLUDE} )
  target_include_directories( sciMeashurementFramework PRIVATE ${Qt5Widgets_INCLUDE_DIRS} )
endif()



