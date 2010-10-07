#ifndef _GLWIDGET_H_
#define _GLWIDGET_H_

#include <QtOpenGL/qgl.h>
#include <QtDesigner/QDesignerExportWidget>
#include <QKeyEvent>
#include <iostream>
#include <QString>
#include <QFileDialog>
#include <QTimer>
#include <ctime>

#include "Point.h"
#include "Scene.h"

class  QDESIGNER_WIDGET_EXPORT GLWidget : public QGLWidget  // CAG
{
  Q_OBJECT

 public:
  //__declspec(dllexport) GLWidget(QWidget * parent);
  GLWidget(QWidget * parent);
  
 signals:
  void framerate(double);

 public slots:

  void changeRenderMode(int mode);

  // help - Ajuda per la terminal des de la que hem  engegat el programa.
  void help(void);

  // Afegiu aquí la declaració dels slots que necessiteu
  void openModel();

 protected:
  // initializeGL() - Aqui incluim les inicialitzacions del contexte grafic.
  virtual void initializeGL();

  // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
  // Tot el que es dibuixa es dibuixa aqui.
  virtual void paintGL( void );
 
  // resizeGL() - Es cridat quan canvi la mida del widget
  virtual void resizeGL (int width, int height);

  // keyPressEvent() - Cridat quan es prem una tecla
  virtual void keyPressEvent(QKeyEvent *e);

  // mousePressEvent() i mouseReleaseEvent()
  virtual void mousePressEvent( QMouseEvent *e);
  virtual void mouseReleaseEvent( QMouseEvent *);
 
  // mouseMoveEvent() - Cridat quan s'arrosega el ratoli amb algun botó premut.
  virtual void mouseMoveEvent(QMouseEvent *e);
 
  void computeCameraInicial(); 
  void setModelview();
  void setProjection();

  Point getObs();
  
 private:
  // paràmetres de la camera
  float anglecam;
  float ra;
  double anterior, posterior; 
  Point VRP;
  float dist;
  float angleX, angleY, angleZ;
  
  Scene escena;  // Escena a representar en el widget

  // interaccio
  typedef  enum {NONE, ROTATE, ZOOM, PAN} InteractiveAction;
  InteractiveAction DoingInteractive;
 
  int   xClick, yClick;

  QTimer timer;
  
  clock_t oldTime;
  int remainingFrames;
};

#endif
