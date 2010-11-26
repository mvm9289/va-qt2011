#include "GLWidget.h"

#include <stdlib.h>
#define _USE_MATH_DEFINES 1
#include <cmath>
#include <QInputDialog>

//#include "AmbientOcclusion.h"

// Constructora amb format per defecte
GLWidget::GLWidget(QWidget * parent):QGLWidget(parent)
{
	// per rebre events de teclat
	setFocusPolicy(Qt::ClickFocus);

	connect( &timer, SIGNAL(timeout()), this, SLOT(updateGL()) );
	timer.start(0);

	DoingInteractive = NONE;  
	lighting = true;
}

// initializeGL() - Aqui incluim les inicialitzacions del contexte grafic.
void GLWidget::initializeGL()
{
	glClearColor(0.35f, 0.35f, 0.6f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING); 
	glEnable(GL_LIGHT0);

	// dimensions escena i camera inicial
	escena.Init();
	computeCameraInicial();
}

void GLWidget::setModelview()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0,0,-dist);
	glRotatef(-angleZ,0,0,1);
	glRotatef(angleX,1,0,0);
	glRotatef(-angleY,0,1,0);
	glTranslatef(-VRP.x,-VRP.y,-VRP.z);
}

void GLWidget::setProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(anglecam, ra, anterior, posterior);
}

void GLWidget::computeCameraInicial()
{
	double radi;
	Point centreEscena;

	centreEscena = escena.Center();
	radi = escena.RadiEscena();

	// mirem cap al centre del terra de l'escena
	VRP = centreEscena;

	dist = 2*radi;
	anglecam = 60; // 2 * asin (radi/2*radi)
	anterior = 0.01;//radi;
	posterior = 1000.; //3*radi;

	// inicialitzem els angles per a veure-ho des d'un cert punt de vista
	angleX = 0;
	angleY = 0;
	angleZ = 0;

	ra = (float)width()/(float)height();
}

// paintGL() - callback cridat cada cop que cal refrescar la finestra. 
void GLWidget::paintGL( void )
{ 
	// Esborrem els buffers
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	setProjection();
	setModelview();

	// Dibuixar eixos aplicacio
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(20,0,0); // X
	glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,20,0); // Y
	glColor3f(0,0,1); glVertex3f(0,0,0); glVertex3f(0,0,20); // Z
	glEnd();

	if (lighting) 
		glEnable(GL_LIGHTING);
	else 
		glDisable(GL_LIGHTING);
	// Pintem els objectes de l'escena.
	escena.Render();
}

// resizeGL() - Cridat quan es canvia el tamany del viewport.
void GLWidget::resizeGL (int width, int height)
{
	glViewport (0, 0, width, height);
	ra = (float)width/(float)height;
	setProjection();
}

// help() - Surt per la terminal des de la que hem 
// engegat el programa. En la versio amn interficie 
// hauria de crear una nova finestra amb la informacio...
void GLWidget::help( void )
{
	cout<<"Tecles definides: \n";
	cout<<"f         Pinta en filferros\n";
	cout<<"s         Pinta amb omplert de polígons\n";
	cout<<"r         Reinicia els paràmetres de la càmara\n";
	cout<<"h,H,?     Imprimir aquesta ajuda\n";
	// Completar amb altres tecles que definiu...
	//
}

/*--------------------
*
*  teclat()
*
*  callback per quan hom prem una tecla.
*
*/
void GLWidget::keyPressEvent(QKeyEvent *e)
{
	switch( e->key() ) 
	{	    
	case Qt::Key_F: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		updateGL();
		break;

	case Qt::Key_S: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		updateGL();
		break;

	case Qt::Key_R: computeCameraInicial();
		updateGL();
		break;

	case  Qt::Key_H  : case  '?'  :  help();
		break;

	case Qt::Key_L:
		lighting = !lighting;
		break;

	case Qt::Key_A:
		{
			bool ok;
			int nrays = QInputDialog::getInteger(this, tr("QInputDialog::getInteger()"),
										tr("Number of rays:"), 32, 0, 128, 1, &ok);
			if (ok)
			{
				cout << "Calculant ambient occlusion..." << endl;
				// PRAC3
				// completeu...
				cout << "done!" << endl;
			}
		}
		break;


	default: e->ignore(); // el propaguem cap al pare...
	}
}

/*--------------------
* mousePressEvent()
*/
void GLWidget::mousePressEvent( QMouseEvent *e){
	xClick = e->x();
	yClick = e->y();

	if (e->button()&Qt::LeftButton && 
		! (e->modifiers()&(Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
	{
		DoingInteractive = ROTATE;
	}
	else if (e->button()&Qt::LeftButton &&  e->modifiers() &Qt::ShiftModifier)
	{
		DoingInteractive = ZOOM;
	}
	else if (e->button()&&Qt::RightButton)
	{
		DoingInteractive = PAN;
	}
}

/*--------------------
*
* mouseReleaseEvent()
*
*/
void GLWidget::mouseReleaseEvent( QMouseEvent *){
	DoingInteractive = NONE;
}

/*--------------------
*
* mouseMoveEvent()
*
*/
void GLWidget::mouseMoveEvent(QMouseEvent *e)
{ 
	if (DoingInteractive == ROTATE)
	{
		angleY-=e->x()-xClick;
		angleX+=e->y()-yClick;
	}
	else if(DoingInteractive == ZOOM) 
	{
		dist+=(e->y()-yClick)*0.01*escena.RadiEscena();
	}
	else if (DoingInteractive == PAN)
	{
		// Aqui cal el codi que modifica els parametres per a fer el pan
		double x = .1 * (-e->x() + xClick);
		double y = .1 * (e->y() - yClick);
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

		xClick = e->x();
		yClick = e->y();
	}

	// Actualitzar els plans de retallat

	xClick=e->x();
	yClick=e->y();

	updateGL();
}


Point GLWidget::getObs()
{
	double mat[16];
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//glTranslatef(0,0,-dist);
	glRotatef(-angleZ,0,0,1);
	glRotatef(angleX,1,0,0);
	glRotatef(-angleY,0,1,0);
	//glTranslatef(-VRP.x,-VRP.y,-VRP.z);
	glGetDoublev(GL_MODELVIEW_MATRIX,mat);
	glPopMatrix();

	Vector zObs(mat[2],mat[6],mat[10]);
	cout << zObs << endl;
	return VRP+dist*zObs;
}