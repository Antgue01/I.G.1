#include "Scene.h"
#include "CheckML.h"
#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>

using namespace glm;
//-------------------------------------------------------------------------

void Scene::init()
{ 
	setGL();  // OpenGL settings

	// allocate memory and load resources
    // Lights
    // Textures

    // Graphics objects (entities) of the scene
	gObjects.push_back(new EjesRGB(400.0));

	Poligono* triangulo = new Poligono(3, 300);
	triangulo->setColor(255, 255, 0, 255);
	gObjects.push_back(triangulo);

	Poligono* circulo = new Poligono(100, 300);
	circulo->setColor(255, 0, 255, 255);
	gObjects.push_back(circulo);

	Sierpinski* sierpinski = new Sierpinski(300, 5000);
	sierpinski->setColor(255, 255, 0, 255);
	gObjects.push_back(sierpinski);

	TrianguloRGB* rgbt = new TrianguloRGB(50);
	gObjects.push_back(rgbt);

	RectanguloRGB* rgbr = new RectanguloRGB(100, 50);
	gObjects.push_back(rgbr);
	dvec3 v = { 0,0,-100 };
	rgbr->setModelMat(translate(rgbr->modelMat(), v));
	dvec3 s = { 8,12,0 };
	rgbr->setModelMat(scale(rgbr->modelMat(), s));
	
}
//-------------------------------------------------------------------------
void Scene::free() 
{ // release memory and resources   

	for (Abs_Entity* el : gObjects)
	{
		delete el;  el = nullptr;
	}
}
//-------------------------------------------------------------------------
void Scene::setGL() 
{
	// OpenGL basic setting
	glClearColor(0.0, 0.0, 0.0, 0.0);  // background color (alpha=1 -> opaque)
	glEnable(GL_DEPTH_TEST);  // enable Depth test 

}
//-------------------------------------------------------------------------
void Scene::resetGL() 
{
	glClearColor(.0, .0, .0, .0);  // background color (alpha=1 -> opaque)
	glDisable(GL_DEPTH_TEST);  // disable Depth test 	
}
//-------------------------------------------------------------------------

void Scene::render(Camera const& cam) const 
{
	cam.upload();

	for (Abs_Entity* el : gObjects)
	{
	  el->render(cam.viewMat());
	}
}

void Scene::update()
{
	for (Abs_Entity* el : gObjects)
	{
		el->update();
	}
}
//-------------------------------------------------------------------------


