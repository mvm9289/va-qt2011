
#include <QtPlugin>

#include "QTexturesList.h"
#include "QTexturesListPlugin.h"

QTexturesListPlugin::QTexturesListPlugin(QObject *parent):QObject(parent)
{
    initialized = false;
}

void QTexturesListPlugin::initialize(QDesignerFormEditorInterface *)
{
    if (initialized) return;
    initialized = true;
}

bool QTexturesListPlugin::isInitialized() const
{
    return initialized;
}

QWidget *QTexturesListPlugin::createWidget(QWidget *parent)
{
    return new QTexturesList(parent);
}

QString QTexturesListPlugin::name() const
{
    return "QTexturesList";
}

QString QTexturesListPlugin::group() const
{
    return "Display Widgets [Examples]";
}

QIcon QTexturesListPlugin::icon() const
{
    return QIcon();
}

QString QTexturesListPlugin::toolTip() const
{
    return "";
}

QString QTexturesListPlugin::whatsThis() const
{
    return "";
}

bool QTexturesListPlugin::isContainer() const
{
    return false;
}

QString QTexturesListPlugin::domXml() const
{
    return "<widget class=\"QTexturesList\" name=\"QTexturesListObject\">\n"
        " <property name=\"geometry\">\n"
        "  <rect>\n"
        "   <x>0</x>\n"
        "   <y>0</y>\n"
        "   <width>100</width>\n"
        "   <height>10</height>\n"
        "  </rect>\n"
        " </property>\n"
        " <property name=\"toolTip\" >\n"
        "  <string>Adapted QTexturesList</string>\n"
        " </property>\n"
        "</widget>\n";
}

QString QTexturesListPlugin::includeFile() const
{
    return "QTexturesList.h";
}

Q_EXPORT_PLUGIN2(QTexturesList, QTexturesListPlugin)
