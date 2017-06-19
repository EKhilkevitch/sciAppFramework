
cmake_minimum_required( VERSION 2.6 )

find_package( Qt4 COMPONENTS QtCore QtGui QtXml REQUIRED )
include( ${QT_USE_FILE} )

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

qt4_wrap_cpp( sciAppFramework_moc_sources 
                inputWidget.h                   
                labelInputWidget.h
                adaptedInputWidget.h
                multiInputWidget.h              
                catalogueWidget.h
                waitWidget.h
                longSpinBox.h
  )

add_library( sciAppFramework ${LIBRARY_TYPE} ${sciAppFramework_source_files} ${sciAppFramework_moc_sources} )
target_link_libraries( sciAppFramework ${QT_LIBRARIES} ${scigraphics_LIBRARY} ${scigraphicsQt_LIBRARY} )

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

qt4_wrap_cpp( sciMeashurementFramework_moc_sources 
                measurementThread.h             
                outputWidget.h                  
                outputSettingsWidget.h                
                controlWidget.h                 
                measureControlWidget.h                 
                measureMainWidget.h
  )

add_library( sciMeashurementFramework ${LIBRARY_TYPE} ${sciMeashurementFramework_source_files} ${sciMeashurementFramework_moc_sources} )
target_link_libraries( sciMeashurementFramework sciAppFramework ${QT_LIBRARIES} ${scigraphics_LIBRARY} ${scigraphicsQt_LIBRARY} )

