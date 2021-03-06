#include "IG1App.h"
#include "CheckML.h"
#include <iostream>

using namespace std;

//-------------------------------------------------------------------------
// static single instance (singleton pattern)

IG1App IG1App::s_ig1app;  // default constructor (constructor with no parameters)

//-------------------------------------------------------------------------

void IG1App::close()
{
	if (!mStop) {  // if main loop has not stopped
		cout << "Closing glut...\n";
		glutLeaveMainLoop();  // stops main loop and destroy the OpenGL context
		mStop = true;   // main loop stopped  
	}
	free();
}
//-------------------------------------------------------------------------

void IG1App::run()   // enters the main event processing loop
{
	if (mWinId == 0) { // if not initialized
		init();       // initialize the application 
		glutMainLoop();      // enters the main event processing loop 
		mStop = true;  // main loop has stopped  
	}
}
//-------------------------------------------------------------------------

void IG1App::init()
{
	// create an OpenGL Context
	iniWinOpenGL();

	// create the scene after creating the context 
	// allocate memory and resources
	mViewPort = new Viewport(mWinW, mWinH); //glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)
	mCamera = new Camera(mViewPort);
	mScene = new Scene;
	mFondo = new Fondo(); //EXTRA1


	mCamera->set2D();
	mScene->init();
}
//-------------------------------------------------------------------------

void IG1App::iniWinOpenGL()
{  // Initialization
	cout << "Starting glut...\n";
	int argc = 0;
	glutInit(&argc, nullptr);

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);  // GLUT_CORE_PROFILE
	glutInitContextFlags(GLUT_DEBUG);		// GLUT_FORWARD_COMPATIBLE

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(mWinW, mWinH);   // window size
	//glutInitWindowPosition (140, 140);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH /*| GLUT_STENCIL*/); // RGBA colors, double buffer, depth buffer and stencil buffer   

	mWinId = glutCreateWindow("IG1App");  // with its associated OpenGL context, return window's identifier 
	if (!gladLoadGL()) { printf("GLAD: Something went wrong!\n"); } //EXTRA 2
	// Callback registration
	glutReshapeFunc(s_resize);
	glutKeyboardFunc(s_key);
	glutSpecialFunc(s_specialKey);
	glutDisplayFunc(s_display);
	glutIdleFunc(s_update);
	glutMouseFunc(s_mouse);
	glutMotionFunc(s_motion);
	glutMouseWheelFunc(s_mouseWheel);


	cout << glGetString(GL_VERSION) << '\n';
	cout << glGetString(GL_VENDOR) << '\n';
}
//-------------------------------------------------------------------------

void IG1App::free()
{  // release memory and resources
	delete mScene; mScene = nullptr;
	delete mCamera; mCamera = nullptr;
	delete mViewPort; mViewPort = nullptr;
	if (auxCamera != nullptr) {
		delete auxCamera;
		auxCamera = nullptr;
	}

	delete mFondo; //EXTRA 1
	mFondo = nullptr;//EXTRA 1

}
//-------------------------------------------------------------------------

void IG1App::display2Vistas()const
{
	Camera auxCam = *mCamera;
	Viewport auxVP = *mViewPort;

	//Usuario
	mViewPort->setSize(mWinW / 2, mWinH);
	auxCam.setSize(mViewPort->width(), mViewPort->height());
	mViewPort->setPos(0, 0);
	mScene->render(auxCam);

	//Cenital
	mViewPort->setPos(mWinW / 2, 0);
	auxCam.setCenital();

	mScene->render(auxCam);

	*mViewPort = auxVP;
}

void IG1App::display() const
{  // double buffering

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clears the back buffer
	//renderizamos el fondo lo primero
	mFondo->render(); //EXTRA 1
	
	if (m2Vistas)display2Vistas();
	else
	{
		mScene->render(*mCamera);  // uploads the viewport and camera to the GPU
	}

	glutSwapBuffers();	// swaps the front and back buffer
}
//-------------------------------------------------------------------------

void IG1App::resize(int newWidth, int newHeight)
{
	mWinW = newWidth; mWinH = newHeight;

	// Resize Viewport to the new window size
	mViewPort->setSize(newWidth, newHeight);

	// Resize Scene Visible Area such that the scale is not modified
	mCamera->setSize(mViewPort->width(), mViewPort->height());

	//Al cambiar el tamaño de la ventana reajustamos el fondo
	mFondo->setSizeVP(newWidth, newHeight);//EXTRA 1
}
//-------------------------------------------------------------------------

void IG1App::key(unsigned char key, int x, int y)
{
	bool need_redisplay = true;

	switch (key) {
	case 27:  // Escape key 
		glutLeaveMainLoop();  // stops main loop and destroy the OpenGL context
	case '+':
		mCamera->setScale(+0.01);  // zoom in  (increases the scale)
		break;
	case '-':
		mCamera->setScale(-0.01);  // zoom out (decreases the scale)
		break;
	case 'l':
		mCamera->set3D();
		break;
	case 'o':
		mCamera->set2D();
		break;
	case 'u':
		mScene->update();
		break;
	case 'U':
		animActivated = !animActivated;
		break;
	default:
		need_redisplay = false;
		break;
	case '0':
		if (mScene->GetID() != 0)
			mScene->setState(0);
		break;
	case '1':
		if (mScene->GetID() != 1)
			mScene->setState(1);
		break;
	case 'F':
		Texture::save("foto.bmp ");
		break;
	case '2':
		mCamera->moveLR(1);
		break;
	case '3':
		mCamera->moveLR(-1);
		break;
	case '4':
		mCamera->moveUD(1);
		break;
	case '5':
		mCamera->moveUD(-1);
		break;
	case '6':
		mCamera->moveFB(1);
		break;
	case '7':
		mCamera->moveFB(-1);
		break;
	case 'p':
		mCamera->changePrj();
		break;
	case 'k':
		m2Vistas = !m2Vistas;
		break;
	}
	//switch

	if (need_redisplay)
		glutPostRedisplay(); // marks the window as needing to be redisplayed -> calls to display()
}
//-------------------------------------------------------------------------

void IG1App::specialKey(int key, int x, int y)
{
	bool need_redisplay = true;
	int mdf = glutGetModifiers(); // returns the modifiers (Shift, Ctrl, Alt)

	switch (key) {
	case GLUT_KEY_RIGHT:
		//if (mdf == GLUT_ACTIVE_CTRL)
		//	mCamera->pitch(-1);   // rotates -1 on the X axis
		//else
		//	mCamera->pitch(1);    // rotates 1 on the X axis
		break;
	case GLUT_KEY_LEFT:
		//if (mdf == GLUT_ACTIVE_CTRL)
		////	mCamera->yaw(1);      // rotates 1 on the Y axis 
		//else
		////	mCamera->yaw(-1);     // rotate -1 on the Y axis 
		//break;
	case GLUT_KEY_UP:
		//mCamera->roll(1);    // rotates 1 on the Z axis
		break;
	case GLUT_KEY_DOWN:
		//mCamera->roll(-1);   // rotates -1 on the Z axis
		break;
	default:
		need_redisplay = false;
		break;
	}//switch

	if (need_redisplay)
		glutPostRedisplay(); // marks the window as needing to be redisplayed -> calls to display()
}
void IG1App::mouse(int button, int state, int x, int y) {
	mMouseCoord = glm::dvec2(x, mWinH - y);
	mMouseButt = button;
	mState = state;
}
void IG1App::motion(int x, int y)
{
	// guardamos la anterior posici�n en var. temp.
	glm::dvec2 mp = mMouseCoord;

	// Guardamos la posici�n actual
	mMouseCoord = glm::dvec2(x, mWinH - y);

	// calculamos el desplazamiento realizado
	mp = mMouseCoord - mp;

	if (mMouseButt == GLUT_RIGHT_BUTTON)
	{
		mCamera->moveLR(mp.x);
		mCamera->moveUD(mp.y);
	}
	else if (mMouseButt == GLUT_LEFT_BUTTON)
	{
		mCamera->orbit(mp.x * 0.05, -mp.y);
	}

	glutPostRedisplay();
}

void IG1App::mouseWheel(int n, int d, int x, int y)
{
	int m = glutGetModifiers();

	if (m == 0) // ninguna est� presionada
	{
		if (d == 1)mCamera->moveFB(.1);
		else mCamera->moveFB(-.1);

		glutPostRedisplay();
	}
	else if (m == GLUT_ACTIVE_CTRL)
	{
		if (d == 1)mCamera->setScale(+0.01);
		else mCamera->setScale(-0.01);
	}

	glutPostRedisplay();
}
void IG1App::Update() {
	int time = glutGet(GLUT_ELAPSED_TIME);

	if (animActivated && time - mLastUpdateTime > 1000 / 60)
	{
		mScene->update();
		display();
		mLastUpdateTime = time;

		//redisplay
	}
}

//-------------------------------------------------------------------------

