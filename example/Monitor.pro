
MAKEFILE = Makefile

TARGET = bin/monitor

OBJECTS_DIR     = ./objects/
MOC_DIR         = ./objects/

SCIGRAPHICS     =  ../../scigraphics/
SCIAPPFRAMEWORK =  ../../sciAppFramework/

DESTDIR         = ./bin/
INCLUDEPATH    += ./source/ $${SCIGRAPHICS}/source/ $${SCIAPPFRAMEWORK}/source
VPATH          += ./source/gui/ ./source/monitor/ ./source/
DEPENDPATH     += ./source/gui/ ./source/monitor/ ./source/

TARGET          = monitor

SOURCES        +=                       \
                monitor.cpp             \
                \
                main.cpp                        \
                controlMonitorWidget.cpp        \
                exampleLoadMonitor.cpp          \
                monitorThread.cpp               \
                outputMonitorWidget.cpp         \

HEADERS        +=                       \
                monitor.h               \
                \
                controlMonitorWidget.h          \
                exampleLoadMonitor.h            \
                monitorThread.h                 \
                outputMonitorWidget.h           \

                

unix:LIBS += -L$${SCIGRAPHICS}/lib/ -Wl,-rpath,$${SCIGRAPHICS}/lib/ -lscigraphicsQt -L$${SCIAPPFRAMEWORK}/lib/ -lsciAppFramework

CONFIG   += qt warn_on threads release

