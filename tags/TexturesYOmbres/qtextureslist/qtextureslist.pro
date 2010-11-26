CONFIG+=designer plugin release
TEMPLATE=lib
TARGET=
DEPENDPATH+=.
INCLUDEPATH+=.

HEADERS+=QTexturesList.h QTexturesListPlugin.h
SOURCES+=QTexturesList.cpp QTexturesListPlugin.cpp

target.path=~/.designer/plugins
INSTALLS+=target