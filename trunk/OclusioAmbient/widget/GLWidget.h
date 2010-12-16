
#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QFileDialog>
#include <QTimer>
#include <ctime>

#include "Point.h"
#include "Scene.h"

class GLWidget:public QGLWidget
{
    Q_OBJECT
    
    public:
        double anglecam;
        double ra;
        double anterior, posterior, anteriorAux, posteriorAux, anteriorIni, posteriorIni;
        Point VRP;
        double dist;
        double angleX, angleY, angleZ;
    
        Scene scene;

        typedef  enum {NONE, ROTATE, ZOOM, PAN, SELECT} InteractiveAction;
        InteractiveAction DoingInteractive;

        int   xClick, yClick;
        
        QTimer timer;
        
        clock_t oldTime;
        int remainingFrames;
        bool movement;

        bool selection;
        int selectedObjectID;
        
        bool projector;
        int projectorTexture;
        QString oldObjectTexture;
        QString oldProjectorTexture;
    
        bool shadows;

        int nRaysObs;
        int nRaysOcc;
        int dmax;
        bool constantImpl;

    public:
        GLWidget(QWidget *parent = 0);

    protected:
        void initializeGL ();
        void paintGL ();
        void resizeGL (int width, int height);

        virtual void mousePressEvent (QMouseEvent *e);
        virtual void mouseReleaseEvent (QMouseEvent *e);
        virtual void mouseMoveEvent (QMouseEvent *e);
        virtual void keyPressEvent (QKeyEvent *event);
    
        void computeInitialCamera();
        void setModelview();
        void setProjection();
        void computeClippingPlanes();
    
        void showNumTrianglesQuads();
    
        void selectObj();
    
        void setTextureMatrix();

    signals:
        void framerate(double);
        void numTriangles(double);
        void numQuads(double);

        void enableDeleteObjectButton(bool);

        void enableOpenTextureButton(bool);
        void enableTextureSpins(bool);
        void newTexture(QString);
        void enableTextureBox(bool);
        void newProjectorTexture(QString);
        void enableProjectorTextureBox(bool);
        void setProjectorChecked(bool);
        void enableResetProjector(bool);

        void setShadowsChecked(bool);
        void setLightSettingsEnabled(bool);
        void setLatLonDefaultValue(int);
        void setRadiusDefaultValue(int);
        void selectionModeChecked(bool);
        void setLCDRadius(double);
        void setLCDLatitude(double);
        void setLCDLongitude(double);

        void setAOccCheckBoxChecked(bool);
        void setAOccSettingsEnabled(bool);
        void setObsCheckBoxChecked(bool);
        void setObsSettingsEnabled(bool);

        void setRenderBoxesSettinsEnabled(bool);
        void setOGLIllumEnabled(bool);

    
    public slots:
        void help();
        void openModel();
        void deleteModel();
        void openTexture();
        void changeRenderMode(int mode);
        void startStop();
        void resetCamera();
        void selectionMode();
        void setTexture(QString name);
        void repeatWrapS(int sWrap);
        void repeatWrapT(int tWrap);
        void projectiveTextureMapping();
        void resetProjector();
        void setShadows();
        void setLightRadius(int r);
        void setLightLongitude(int lon);
        void setLightLatitude(int lat);

        void setAmbientOcclusion(bool checked);
        void setObscurance(bool checked);

        void computeAmbientOcclusion();
        void computeObscurance();

        void setnRaysOcc(int nrays);
        void setnRaysObs(int nrays);
        void setDmax(int d);
        void setRoFuncImpl(int selected);

        void setRenderBoxes(bool rend);
        void setRenderBoxesLvl(int lvl);

        void setOpenGLIllum(bool b);


};

#endif
