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

     //Graphics objects (entities) of the scene
	gObjects.push_back(new EjesRGB(400.0));
	//Poligono* pol = new Poligono(10,300);
	Poligono* circ = new Poligono(100, 300);
	circ->Colour(75, 0, 130, 255);
	//pol->Colour(255,255,0,255);
	gObjects.push_back(circ);
	//gObjects.push_back(pol);
	//Sierpinski* s = new Sierpinski(4000, 200);
	//s->Colour(255, 0, 0,255);
	//gObjects.push_back(s);
	//TrianguloRGB* t = new TrianguloRGB(300);
	//gObjects.push_back(t);
	//RectanguloRGB* re = new RectanguloRGB(200, 200);
	//gObjects.push_back(re);
	//vec3 trans = { -100,0,0 };
	//translate(dest, trans);
	//Estrella3D* estrellita = new Estrella3D(300, 5, 300);
	//gObjects.push_back(estrellita);
	Caja* cajitamagica = new Caja(300);
	gObjects.push_back(cajitamagica);
	SetBackground({ 0.0,0.0,0.0,0.0 });
	
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
	glClearColor(1.0, 1.0, 1.0, 1.0);  // background color (alpha=1 -> opaque)
	glEnable(GL_DEPTH_TEST);  // enable Depth test 

}
void Scene::SetBackground(vec4 col) {
	glClearColor(col.r, col.g, col.b, col.a);
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
//-------------------------------------------------------------------------


