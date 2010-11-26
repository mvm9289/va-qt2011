TEMPLATE    = lib

CONFIG     += designer plugin debug_and_release
TARGET      = pluginGLWidget
QT         += opengl

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

#target.path = $$[QT_INSTALL_PLUGINS]/designer
target.path = ~/.designer/plugins

INSTALLS   += target

HEADERS     =  GLWidgetPlugin.h GLWidget.h
SOURCES     =  GLWidgetPlugin.cpp GLWidget.cpp

HEADERS	+= 	Object.h \
	Face.h \
	Vertex.h \
	Color.h \
	Box.h \
	Material.h \
	MaterialLib.h \
	Point.h \
        Scene.h 
       
SOURCES	+= Object.cpp \
	Face.cpp \
	Vertex.cpp \
	Color.cpp \
	Box.cpp \
	Material.cpp \
	MaterialLib.cpp \
	Point.cpp \
        Scene.cpp 

HEADERS += 	Ray.h \
		Surface.h \
		Directions.h 

SOURCES += 	Ray.cpp \
		Surface.cpp \
		Directions.cpp 
 

