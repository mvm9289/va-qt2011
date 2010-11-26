#qmake -r
#make -j5 release

TEMPLATE = subdirs
SUBDIRS += widget app
CONFIG += ordered debug_and_release