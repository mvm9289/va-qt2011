
#ifndef __QTEXTURESLIST_H__
#define __QTEXTURESLIST_H__

#include <QtDesigner/QDesignerExportWidget>
#include <QComboBox>
#include <QString>

class QDESIGNER_WIDGET_EXPORT QTexturesList:public QComboBox
{
    Q_OBJECT
    
    public:
        QTexturesList(QWidget *parent = 0);

    public slots:
        void addTexture(QString texture);
};

#endif
