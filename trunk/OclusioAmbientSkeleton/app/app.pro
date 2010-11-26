TEMPLATE	= app
LANGUAGE	= C++

CONFIG += debug_and_release
CONFIG += qt warn_on release console

CONFIG(debug, debug|release){
  DESTDIR = ../bin
  MOC_DIR = build/debug
  OBJECTS_DIR = build/debug
  RCC_DIR = build/debug
}CONFIG(release, debug|release){
  DESTDIR = ../bin
  MOC_DIR = build/release
  OBJECTS_DIR = build/release
  RCC_DIR = build/release
}

QT += opengl
INCLUDEPATH += ../widget

#LIBS += ../widget/libpluginGLWidget.so
#LIBS += ../widget/release/pluginGLWidget.lib

LIBS += -L$$DESTDIR -lpluginGLWidget

HEADERS	+= Principal.h \

SOURCES	+= Principal.cpp \
	main.cpp  

TARGET = PracticaVA

FORMS	= Principal.ui




