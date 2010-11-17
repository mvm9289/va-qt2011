
#include <cmath>
#include <stdlib.h>
#include <sys/timeb.h>

#include "GLWidget.h"
#include "Texture.h"

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
    
    movement = false;
    
    selection = false;
    selectedObjectID = NONE_OBJECT;
    
    projector = false;
    projectorTexture = -1;
    
    shadows = false;
    
    scene.Init();
    computeInitialCamera();
    showNumTrianglesQuads();
    
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
    if (projector || shadows) gluPerspective(anglecam, ra, 0.001, posterior*3);
    else gluPerspective(anglecam, ra, anterior, posterior);
}

void GLWidget::computeInitialCamera()
{
    Point center = scene.center();
    double radius = scene.radius();
    VRP = center;

    if (shadows)
    {
        dist = 5*radius;
        anglecam = 60;
        anteriorIni = anteriorAux = anterior = radius;
        posteriorIni = posteriorAux = posterior = 4*radius;
    }
    else
    {
        dist = 2*radius;
        anglecam = 60;
        anteriorIni = anteriorAux = anterior = radius;
        posteriorIni = posteriorAux = posterior = 3*radius;
    }

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

    if (!projector) scene.Render(false);
    else
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, projectorTexture);
        scene.Render(true);
        glDisable(GL_TEXTURE_2D);
    }

    if(movement && !selection) if (++angleY >= 360.0) angleY -= 360.0;
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

    if(!selection)
    {
      if (e->button()&Qt::LeftButton && !(e->modifiers()&(Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
          DoingInteractive = ROTATE;
      else if (e->button()&Qt::LeftButton &&  e->modifiers()&Qt::ShiftModifier)
          DoingInteractive = ZOOM;
      else if (e->button()&Qt::RightButton)
          DoingInteractive = PAN;
    }
    else
    {
        if (e->button()&Qt::LeftButton)
        {
            if (selectedObjectID != NONE_OBJECT)
            {
                scene.setSelected(selectedObjectID, false);
                emit enableOpenTextureButton(projector);
                emit enableTextureSpins(false);
                emit enableTextureBox(false);
                emit enableDeleteObjectButton(false);
                selectedObjectID = NONE_OBJECT;
            }
            selectObj();
            if (selectedObjectID != NONE_OBJECT)
            {
                scene.setSelected(selectedObjectID, true);
                emit enableOpenTextureButton(!projector);
                emit enableTextureSpins(!projector);
                emit enableTextureBox(!projector);
                emit enableDeleteObjectButton(true);
            }
        }
        else if (e->button()&Qt::RightButton && selectedObjectID != NONE_OBJECT)
        {
            scene.setSelected(selectedObjectID, false);
            emit enableOpenTextureButton(projector);
            emit enableTextureSpins(false);
            emit enableTextureBox(false);
            emit enableDeleteObjectButton(false);
            selectedObjectID = NONE_OBJECT;
        }
        DoingInteractive = SELECT;
    }
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
        case Qt::Key_A:
            if(selection && e->modifiers()&Qt::ControlModifier) 
            {
                scene.setSelected(ALL_OBJECTS, true);
                selectedObjectID = ALL_OBJECTS;
                emit enableOpenTextureButton(!projector);
                emit enableTextureSpins(!projector);
                emit enableTextureBox(!projector);
                emit enableDeleteObjectButton(true);
            }
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

    if(DoingInteractive == SELECT)
    {
        if(selectedObjectID != NONE_OBJECT)
        {
            double x = -0.5*(-e->x() + xClick);
            double y = -0.5*(e->y() - yClick);
            float m[4][4];
            glGetFloatv (GL_MODELVIEW_MATRIX, &m[0][0]);
            Point s, u;
            s.x = m[0][0];
            s.y = m[1][0];
            s.z = m[2][0];
            u.x = m[0][1];
            u.y = m[1][1];
            u.z = m[2][1];
            scene.redistributeSelectedObject(x*s + y*u);
        scene.updateBoundingBox();
        }
    }
    else if (DoingInteractive == ROTATE)
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
    timer.stop();
    QString filename = QFileDialog::getOpenFileName(this, "Select a model...", "../models", "object (*.obj)");
    timer.start(0);

    if (filename != "") 
    {
        scene.OpenModel(filename.toLatin1().data());
        scene.updateBoundingBox();
        computeInitialCamera();
        showNumTrianglesQuads();
    }
}

void GLWidget::deleteModel()
{
    scene.deleteSelectedModel();
    scene.updateBoundingBox();
    emit enableOpenTextureButton(projector);
    emit enableTextureSpins(false);
    emit enableTextureBox(false);
    emit enableDeleteObjectButton(false);

    showNumTrianglesQuads();
    selectionMode();
    emit selectionModeChecked(false);
}

void GLWidget::openTexture()
{
    timer.stop();
    QString filename = QFileDialog::getOpenFileName(this, "Select a texture...", "../textures", "Image (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)");
    timer.start(0);

    if (filename != "") 
    {
        Texture texture;
        if (texture.loadTexture(filename) == string(filename.toLatin1().data()))
        {
            texture.setMinMagFilter(GL_LINEAR, GL_LINEAR);
            if (projector)
            {
                texture.setWrapMode(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
                texture.sendToGL(true);
                projectorTexture = texture.getTextureID();
                emit newProjectorTexture(filename);
                oldProjectorTexture = filename;
            }
            else
            {
                texture.setWrapMode(GL_REPEAT, GL_REPEAT);
                texture.sendToGL();
                scene.setTexture(texture.getTextureID());
                emit newTexture(filename);
                oldObjectTexture = filename;
                selectionMode();
                emit selectionModeChecked(false);
            }
        }
        else cout << "Error: Can not open the texture" << endl;
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
    vector<int> info = scene.numTrianglesQuads();
    int tri = info[0];
    int quads = info[1];
    emit numQuads((double)(quads)/1000.0);
    emit numTriangles((double)(tri)/1000.0);
}

void GLWidget::resetCamera()
{
    computeInitialCamera();
}

void GLWidget::selectionMode()
{
    if (selection && selectedObjectID != NONE_OBJECT)
    {
        scene.setSelected(selectedObjectID, false);
        emit enableOpenTextureButton(projector);
        emit enableTextureSpins(false);
        emit enableTextureBox(false);
        emit enableDeleteObjectButton(false);
        selectedObjectID = NONE_OBJECT;
    }
    selection = !selection;
}

void GLWidget::selectObj()
{
    GLuint hbuff [50*4];
    glSelectBuffer(50*4,hbuff);
    glDisable(GL_LIGHTING);

    glRenderMode(GL_SELECT);
    glInitNames();

    glMatrixMode(GL_PROJECTION);
    GLfloat proj[16];
    glGetFloatv(GL_PROJECTION_MATRIX, proj);
    glPushMatrix();
    glLoadIdentity();
    int vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    gluPickMatrix(xClick, vp[3] - yClick, 2, 2, vp);
    glMultMatrixf(proj);

    scene.Render();

    int nhits = glRenderMode(GL_RENDER);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    if(nhits == 0) selectedObjectID = NONE_OBJECT;
    else 
    {
        int minZnear = hbuff[1];
        selectedObjectID = hbuff[3];
        nhits--;
        for (int i = 5; nhits > 0; i+=4, nhits--)
            if (minZnear > (int)hbuff[i]) 
            {
                minZnear = hbuff[i];
                selectedObjectID = hbuff[i+2];
            }
    }
    glEnable(GL_LIGHTING);
}


void GLWidget::setTexture(QString name)
{
    Texture texture;
    int textureID = texture.getTextureID(string(name.toLatin1().data()), projector);
    if (textureID != -1)
    {
        texture.setMinMagFilter(GL_LINEAR, GL_LINEAR);
        if (!projector)
        {
            texture.setWrapMode(GL_REPEAT, GL_REPEAT);
            texture.resendToGL();
            scene.setTexture(textureID);
            oldObjectTexture = name;
        }
        else
        {
            texture.setWrapMode(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
            texture.resendToGL(true);
            projectorTexture = textureID;
            oldProjectorTexture = name;
        }
    }
}

void GLWidget::repeatWrapS(int sWrap)
{
    scene.repeatWrapS(sWrap);
}

void GLWidget::repeatWrapT(int tWrap)
{
    scene.repeatWrapT(tWrap);
}

void GLWidget::setTextureMatrix()
{
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glTranslated(0.5, 0.5,0.5);
    glScaled(0.5, 0.5, 0.5);
    gluPerspective(anglecam, ra, anterior, posterior);
    glTranslatef(0, 0, -dist);
    glRotatef(-angleZ, 0, 0, 1);
    glRotatef(angleX, 1, 0, 0);
    glRotatef(-angleY, 0, 1, 0);
    glTranslatef(-VRP.x, -VRP.y, -VRP.z);
    glMatrixMode(GL_MODELVIEW);
}

void GLWidget::projectiveTextureMapping()
{
    if (projectorTexture == -1)
    {
        if (shadows)
        {
            setShadows();
            emit setShadowsChecked(false);
        }
            
        projector = true;
        openTexture();
        if (projectorTexture != -1)
        {
            emit enableOpenTextureButton(true);
            emit enableProjectorTextureBox(true);
            emit enableTextureBox(false);
            emit enableTextureSpins(false);
            emit enableResetProjector(true);
            setTextureMatrix();
        }
        else
        {
            projector = false;
            emit setProjectorChecked(false);
        }
    }
    else if (projector)
    {
        projector = false;
        
        emit enableOpenTextureButton(selectedObjectID != NONE_OBJECT);
        emit enableProjectorTextureBox(false);
        emit enableTextureBox(selectedObjectID != NONE_OBJECT);
        emit enableTextureSpins(selectedObjectID != NONE_OBJECT);
        emit enableResetProjector(false);
        
        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
        
        setTexture(oldObjectTexture);
    }
    else
    {
        if (shadows)
        {
            setShadows();
            emit setShadowsChecked(false);
        }
        
        projector = true;
        
        emit enableOpenTextureButton(true);
        emit enableProjectorTextureBox(true);
        emit enableTextureBox(false);
        emit enableTextureSpins(false);
        emit enableResetProjector(true);
        
        setTextureMatrix();
        
        setTexture(oldProjectorTexture);
    }
}

void GLWidget::resetProjector()
{
    setTextureMatrix();
}

void GLWidget::setShadows()
{
    if (!shadows)
    {
        if (projector)
        {
            projectiveTextureMapping();
            emit setProjectorChecked(false);
        }
        
        shadows = true;
        computeInitialCamera();
        emit setLatLonDefaultValue(90);
        emit setRadiusDefaultValue(100);
        emit setLightSettingsEnabled(true);
    }
    else
    {
        shadows = false;
        computeInitialCamera();
        emit setLightSettingsEnabled(false);
    }
    scene.setShadows();
}

void GLWidget::setLightRadius(int rad)
{
    scene.setLightRadius(((float)rad)/100);
    emit setLCDRadius(((float)rad)/100);
}

void GLWidget::setLightLongitude(int lon)
{
    scene.setLightLongitude((float)lon);
    setLCDLongitude(lon);
}

void GLWidget::setLightLatitude(int lat)
{
    scene.setLightLatitude((float)lat);
    setLCDLatitude(lat);
}
