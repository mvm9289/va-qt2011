CONFIG		    += designer plugin debug_and_release qt warn_on release
TARGET 		    = pluginGLWidget
QT			        += opengl
OBJECTS_DIR	= .obj
MOC_DIR		    = .moc
TEMPLATE		    = lib app
target.path	    = ~/.designer/plugins
INSTALLS		    += target
HEADERS		    = GLWidgetPlugin.h GLWidget.h
SOURCES		    = GLWidgetPlugin.cpp GLWidget.cpp
LANGUAGE	    = C++

HEADERS		    += 	Object.h \
                                Face.h \
                                Vertex.h \
                                Color.h \
                                Box.h \
                                Material.h \
                                MaterialLib.h \
                                Point.h \
                                Scene.h \
				Texture.h
       
SOURCES		    += 	Object.cpp \
                                Face.cpp \
                                Vertex.cpp \
                                Color.cpp \
                                Box.cpp \
                                Material.cpp \
                                MaterialLib.cpp \
                                Point.cpp \
                                Scene.cpp \
				Texture.cpp
                                