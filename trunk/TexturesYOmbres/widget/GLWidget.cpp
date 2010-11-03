
#include <cmath>
#include <stdlib.h>
#include <sys/timeb.h>

#include "GLWidget.h"

#define FRAMERATE_RANGE 100

GLWidget::GLWidget(QWidget *parent):QGLWidget(parent)
{
    setFocusPolicy(Qt::ClickFocus);
    
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer.start(0);
    
    DoingInteractive = NONE;
}

void GLWidget::initializeGL()
{
    glClearColor(0.35f, 0.35f, 0.6f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    
    scene.Init();
    computeInitialCamera();
    showNumTrianglesQuads();
    
    movement = false;
    remainingFrames = FRAMERATE_RANGE;
    timeb t;
    ftime(&t);
    oldTime = t.time*1000 + t.millitm;
}

void GLWidget::setModelview()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -dist);
    glRotatef(-angleZ, 0, 0, 1);
    glRotatef(angleX, 1, 0, 0);
    glRotatef(-angleY, 0, 1, 0);
    glTranslatef(-VRP.x, -VRP.y, -VRP.z);
}

void GLWidget::setProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(anglecam, ra, anterior, posterior);
}

void GLWidget::computeInitialCamera()
{
    Point center = scene.center();
    double radius = scene.radius();

    VRP = center;

    dist = 2*radius;
    anglecam = 60;
    anteriorIni = anteriorAux = anterior = radius;
    posteriorIni = posteriorAux = posterior = 3*radius;

    angleX = 0;
    angleY = 0;
    angleZ = 0;

    ra = (float)width()/(float)height();
}

void GLWidget::resizeGL(int width, int height)
{
    glViewport (0, 0, width, height);
    ra = (float)width/(float)height;
    setProjection();
}

void GLWidget::paintGL( void )
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setModelview();
    setProjection();

    /*glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(20,0,0);
    glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,20,0);
    glColor3f(0,0,1); glVertex3f(0,0,0); glVertex3f(0,0,20);
    glEnd();*/

    scene.Render();

    if(movement) if (++angleY >= 360.0) angleY -= 360.0;
    if (--remainingFrames == 0)
    {
        glFinish();
        timeb t;
        ftime(&t);
        clock_t currentTime = t.time*1000 + t.millitm;;
        double rate = (FRAMERATE_RANGE*1000.0)/(double)(currentTime - oldTime);
        emit framerate(rate);		
        oldTime = currentTime;
        remainingFrames = FRAMERATE_RANGE;
    }
}

void GLWidget::help( void )
{
    cout<<"Defined keys: \n";
    cout<<"f         Render in GL_LINES mode\n";
    cout<<"s         Render in GL_FILL mode\n";
    cout<<"r         Reset camera parameters\n";
    cout<<"h,H,?     Print help\n";
}

void GLWidget::mousePressEvent(QMouseEvent *e)
{
    xClick = e->x();
    yClick = e->y();

    if (e->button()&Qt::LeftButton && !(e->modifiers()&(Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
        DoingInteractive = ROTATE;
    else if (e->button()&Qt::LeftButton &&  e->modifiers()&Qt::ShiftModifier)
        DoingInteractive = ZOOM;
    else if (e->button()&Qt::RightButton)
        DoingInteractive = PAN;
}

void GLWidget::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
        case Qt::Key_F:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            updateGL();
            break;
        case Qt::Key_S:
            glPolygonMode(GL_FRONT, GL_FILL);
            updateGL();
            break;
        case Qt::Key_R:
            computeInitialCamera();
            updateGL();
            break;
        case 'h': case 'H': case '?':
            help();
            break;
        default:
            e->ignore();
            break;
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *)
{
    DoingInteractive = NONE;
}

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (DoingInteractive == ROTATE)
    {
        angleY-=e->x()-xClick;
        angleX+=e->y()-yClick;
    }
    else if(DoingInteractive == ZOOM) 
    {
        double inc = (e->y()-yClick)*0.01*scene.radius();
        
        dist += inc;
        anteriorAux += inc;
        posteriorAux += inc;
        computeClippingPlanes();
        anteriorIni += inc;
        posteriorIni += inc;
    }
    else if (DoingInteractive == PAN)
    {
        double x = .1*(-e->x() + xClick);
        double y = .1*(e->y() - yClick);
        float m[4][4];
        glGetFloatv (GL_MODELVIEW_MATRIX, &m[0][0]);
        Point s, u;
        s.x = m[0][0];
        s.y = m[1][0];
        s.z = m[2][0];
        u.x = m[0][1];
        u.y = m[1][1];
        u.z = m[2][1];
        VRP = VRP + x * s + y * u;

        Point center = scene.center();
        double distance = (VRP - center).length();
        anteriorAux = anteriorIni - distance;
        posteriorAux = posteriorIni + distance;
        computeClippingPlanes();
    }

    xClick=e->x();
    yClick=e->y();

    updateGL();
}

void GLWidget::computeClippingPlanes()
{
    if (anteriorAux <= 0.0)
    {
        anterior = 0.001;
        posterior = (posteriorAux - anteriorAux) + posterior;
    }
    else
    {
        anterior = anteriorAux;
        posterior = posteriorAux;
    }
}

void GLWidget::openModel()
{
    QString filename = QFileDialog::getOpenFileName(this, "Select a model...", "../models", "object (*.obj)");
    if (filename != "") 
    {
        scene.OpenModel(filename.toLatin1().data());
        computeInitialCamera();
    }
}

void GLWidget::openTexture()
{
    QString filename = QFileDialog::getOpenFileName(this, "Select a texture...", "../textures", "Image (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)");
    if (filename != "") 
    {
        QImage texture, buf;
        buf.load(filename);
        texture = QGLWidget::convertToGLFormat(buf);
        scene.setTexture((void *)texture.bits(), texture.width(), texture.height());
    }
}

void GLWidget::changeRenderMode(int mode)
{
    scene.ChangeRenderMode(mode);
}

void GLWidget::startStop()
{
    movement = !movement;
}

void GLWidget::showNumTrianglesQuads()
{
    int tri, quads;
    tri = scene.numTrianglesQuads_Model()[0];
    quads = scene.numTrianglesQuads_Model()[1];
    emit numQuads((double)(quads)/1000.0);
    emit numTriangles((double)(tri)/1000.0);
}

void GLWidget::resetCamera()
{
    computeInitialCamera();
}

