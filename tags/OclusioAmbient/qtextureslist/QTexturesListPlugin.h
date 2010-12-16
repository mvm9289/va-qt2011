
#ifndef __QTEXTURESLISTPLUGIN_H__
#define __QTEXTURESLISTPLUGIN_H__

#include <QDesignerCustomWidgetInterface>

class QTexturesListPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

    private:
        bool initialized;

    public:
        QTexturesListPlugin(QObject *parent = 0);
        bool isContainer() const;
        bool isInitialized() const;
        QIcon icon() const;
        QString domXml() const;
        QString group() const;
        QString includeFile() const;
        QString name() const;
        QString toolTip() const;
        QString whatsThis() const;
        QWidget *createWidget(QWidget *parent);
        void initialize(QDesignerFormEditorInterface *core);
};

#endif
