INCLUDEPATH +=$$PWD/include
Debug: { 
LIBS += -l$$PWD/x64/vc15/lib/opencv_world343d
} 
Release: { 
LIBS += -l$$PWD/x64/vc15/lib/opencv_world343
} 

