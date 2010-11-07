
#include "QTexturesList.h"

QTexturesList::QTexturesList(QWidget* parent):QComboBox(parent) {}

void QTexturesList::addTexture(QString texture)
{
    addItem(texture);
    setEnabled(true);
}
