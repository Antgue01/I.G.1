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

	//EXTRA 1
	mFondo = new Fondo();
	mFondo->setSize(mWinW, mWinH);
	//FIN EXTRA 1


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

	//EXTRA 1
	delete mFondo; mFondo = nullptr;
	//FIN EXTRA 1

	if (auxCamera != nullptr) {
		delete auxCamera;
		auxCamera = nullptr;
	}

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

	//EXTRA 1
	//mFondo->getCamera()->foo();
	mFondo->render();

	if (m2Vistas)display2Vistas();
	else
	{
		//FIN EXTRA 1
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

	//EXTRA 1
	//Resize background image
	mFondo->setSize(newWidth, newHeight);
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
	case '0':
		if (mScene->GetID() != 0)
			mScene->setState(0);
		break;
	case '1':
		if (mScene->GetID() != 1)
			mScene->setState(1);
		break;
	case '2':
		if (mScene->GetID() != 2)
			mScene->setState(2);
		break;
	case '3':
		if (mScene->GetID() != 3)
			mScene->setState(3);
		break;
	case '4':
		if (mScene->GetID() != 4)
			mScene->setState(4);
		break;
	case 'F':
		Texture::save("foto.bmp ");
		break;
	case 'D':
		mCamera->moveLR(1);
		break;
	case 'A':
		mCamera->moveLR(-1);
		break;
	case 'W':
		mCamera->moveUD(1);
		break;
	case 'S':
		mCamera->moveUD(-1);
		break;
	case 'Q':
		mCamera->moveFB(1);
		break;
	case 'E':
		mCamera->moveFB(-1);
		break;
	case 'p':
		mCamera->changePrj();
		break;
	case 'k':
		m2Vistas = !m2Vistas;
		break;
	case 'q':
	{//if (!mScene->getLightsActivated())
		//	mScene->setLightsActivated(true);
		DirLight* DL = mScene->getDirectionalLight();
		if (DL != nullptr)
			DL->enable();
		break;
	}
	case 'w':
	{	//if (mScene->getLightsActivated())
		//	mScene->setLightsActivated(false);
		DirLight* DL = mScene->getDirectionalLight();
		if (DL != nullptr)
			DL->disable();
		break;
	}
	case 'a':
	{
		//if (!mScene->getPositionalLightActivated())
		//	mScene->setPositionalLightActivated(true);
		PosLight* PL = mScene->getPositionalLight();
		if (PL != nullptr)
			PL->enable();

		break;
	}
	case 's':
	{	//if (mScene->getPositionalLightActivated())
		//	mScene->setPositionalLightActivated(false);
		PosLight* PL = mScene->getPositionalLight();
		if (PL != nullptr)
			PL->disable();
		break;
	}
	case 'z':
	{	//if (!mScene->getSpotLightActivated())
		//	mScene->setSpotLightActivated(true);
		SpotLight* SPL = mScene->getSpotLight();
		if (SPL != nullptr)
			SPL->enable();
		break;
	}
	case 'x':
	{	//if (mScene->getSpotLightActivated())
		//	mScene->setSpotLightActivated(false);
		SpotLight* SPL = mScene->getSpotLight();
		if (SPL != nullptr)
			SPL->disable();
		break;

	}
	case 't':
	{
		if (mScene->getPlane() != nullptr)
		{
			SpotLight* SPL = mScene->getPlaneLight();
			if (SPL != nullptr)
				SPL->enable();
		}
		break;
	}
	case 'g':
	{
		if (mScene->getPlane() != nullptr)
		{
			SpotLight* SPL = mScene->getPlaneLight();
			if (SPL != nullptr)
				SPL->disable();
		}
		break;
	}
	case 'd':
	{
		if (mScene->getPlane() != nullptr)
		{
			PosLight* SPL = mScene->getLuzMinero();
			if (SPL != nullptr)
				SPL->enable();
		}
		break;
	}
	case 'f':
	{
		if (mScene->getPlane() != nullptr)
		{
			PosLight* SPL = mScene->getLuzMinero();
			if (SPL != nullptr)
				SPL->disable();
		}
		break;
	}
	case 'e':
		GLfloat * amb;
		mScene->switchLightsOnOff();
		if (mScene->getLightsOff())
		{
			//apagamos las luces y ponemos la luz ambiente global a 0
			amb = new GLfloat[4]{ 0, 0, 0, 1.0 };
			if (mScene->getPlaneLight() != nullptr)mScene->getPlaneLight()->disable();
			if (mScene->getDirectionalLight() != nullptr)mScene->getDirectionalLight()->disable();
			if (mScene->getPositionalLight() != nullptr)mScene->getPositionalLight()->disable();
			if (mScene->getSpotLight() != nullptr)mScene->getSpotLight()->disable();
			if (mScene->getLuzMinero() != nullptr)mScene->getLuzMinero()->disable();

		}
		//reestablecemos la luz ambiente global
		else amb = new GLfloat[4]{ 0.2, 0.2, 0.2, 1.0 };
		//volcamos la variable en la luz ambiente global
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
		break;
	case 'y':
		animActivated = !animActivated;
		break;

	default:
		need_redisplay = false;
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

		mFondo->getCamera()->foo();
	}
	else if (mMouseButt == GLUT_LEFT_BUTTON)
	{
		mCamera->orbit(mp.x * 0.05, -mp.y);
		mFondo->getCamera()->foo();

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

