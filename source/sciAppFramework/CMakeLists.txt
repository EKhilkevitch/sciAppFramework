
cmake_minimum_required( VERSION 2.6 )
project( sciAppFramework ) 

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

set ( sciAppFramework_source_files
                inputWidget.cpp         
                multiInputWidget.cpp    
                measurementThread.cpp   
                parametersWidget.cpp    
                controlWidget.cpp       
                outputWidget.cpp        
                settingsWidget.cpp      
                mainWidget.cpp          
                okcancelDialog.cpp      
                catalogueWidget.cpp
                utility.cpp             
  )

qt4_wrap_cpp( sciAppFramework_moc_sources 
                inputWidget.h                   
                multiInputWidget.h              
                measurementThread.h             
                parametersWidget.h              
                controlWidget.h                 
                outputWidget.h                  
                settingsWidget.h                
                mainWidget.h                    
                okcancelDialog.h                
                catalogueWidget.h
  )

add_library( sciAppFramework ${library_type} ${sciAppFramework_source_files} ${sciAppFramework_moc_sources} )

