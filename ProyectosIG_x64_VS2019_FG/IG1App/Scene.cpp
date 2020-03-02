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

	gTextures.push_back(new Texture());
	gTextures.at(0)->load("..\\BMPS\\baldosaF.bmp");

	gTextures.push_back(new Texture()); 
	gTextures.at(1)->load("..\\BMPS\\baldosaC.bmp");

	gTextures.push_back(new Texture());
	gTextures.at(2)->load("..\\BMPS\\container.bmp");

	gTextures.push_back(new Texture());
	gTextures.at(3)->load("..\\BMPS\\papelE.bmp");

	gTextures.push_back(new Texture());
	gTextures.at(4)->load("..\\BMPS\\grass.bmp", u8vec3(0,0,0),0);

	gTextures.push_back(new Texture());
	gTextures.at(5)->load("..\\BMPS\\windowV.bmp",100);

	// Graphics objects (entities) of the scene
	for (int i = 0; i < gObjects.size(); i++)
	{
		delete gObjects.at(i);
		
	}
	gObjects.clear();

	if (miId == 0) {
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
	else if (miId == 1)
	{

		gObjects.push_back(new EjesRGB(400.0));

		Estrella3D* estrella3D = new Estrella3D(25, 4, 25,gTextures.at(0));
		estrella3D->setModelMat(translate(estrella3D->modelMat(), dvec3(-325.0,100.0,-325.0)));
		gObjects.push_back(estrella3D);

		Caja* c = new Caja(50, gTextures.at(2), gTextures.at(3));
		c->setModelMat(translate(c->modelMat(), dvec3(-300.0, 26.0, -300.0)));
		gObjects.push_back(c);

		Suelo* suelo = new Suelo(700, 700, 10, 10,gTextures.at(1));		
		gObjects.push_back(suelo);
		
		Foto* foto = new Foto(300, 300);
		gObjects.push_back(foto);

		Plant* planta = new Plant(200, 200, gTextures.at(4));
		gObjects.push_back(planta);
		planta->setModelMat(translate(planta->modelMat(), dvec3(250.0, 100.0, -250.0)));
		Cubo* cuboConTransparencia = new Cubo(700, gTextures.at(5));
		cuboConTransparencia->setModelMat(translate(cuboConTransparencia->modelMat(), dvec3(0, 350, 0)));
		gObjects.push_back(cuboConTransparencia);
		
	}
}
//-------------------------------------------------------------------------
void Scene::free()
{ // release memory and resources   

	for (Abs_Entity* el : gObjects)
	{
		delete el;  el = nullptr;
	}

	for (Texture* t : gTextures)
	{
		delete t;  t = nullptr;
	}
}
//-------------------------------------------------------------------------
void Scene::setGL()
{
	// OpenGL basic setting
	glClearColor(0.0, 0.0, 0.0, 0.0);  // background color (alpha=1 -> opaque)
	glEnable(GL_DEPTH_TEST);  // enable Depth test 
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_TEXTURE_2D);

}
//-------------------------------------------------------------------------
void Scene::resetGL()
{
	glClearColor(.0, .0, .0, .0);  // background color (alpha=1 -> opaque)
	glDisable(GL_DEPTH_TEST);  // disable Depth test 
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
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
void Scene::setState(int id) {
	miId = id;
	init();
}
//-------------------------------------------------------------------------


