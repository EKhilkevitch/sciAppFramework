
cmake_minimum_required( VERSION 2.6 )
project( sciAppFramework ) 

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

set ( sciAppFramework_source_files
                settingsObject.cpp
                inputWidget.cpp         
                multiInputWidget.cpp    
                measurementThread.cpp   
                parametersWidget.cpp    
                controlWidget.cpp       
                outputWidget.cpp        
                outputSettingsWidget.cpp
                mainWidget.cpp          
                okcancelDialog.cpp      
                catalogueWidget.cpp
                waitWidget.cpp
                utility.cpp             
  )

qt4_wrap_cpp( sciAppFramework_moc_sources 
                inputWidget.h                   
                multiInputWidget.h              
                measurementThread.h             
                parametersWidget.h              
                controlWidget.h                 
                outputWidget.h                  
                outputSettingsWidget.h                
                mainWidget.h                    
                okcancelDialog.h                
                catalogueWidget.h
                waitWidget.h
  )

add_library( sciAppFramework ${LIBRARY_TYPE} ${sciAppFramework_source_files} ${sciAppFramework_moc_sources} )
target_link_libraries( sciAppFramework ${QT_LIBRARIES} ${scigraphics_LIBRARY} ${scigraphicsQt_LIBRARY} )

