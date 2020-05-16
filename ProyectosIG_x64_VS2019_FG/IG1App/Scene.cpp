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
	gObjects.push_back(new EjesRGB(400));
	glClearColor(0.7, 0.8, 0.9, 1);

	if (miId == 0) {
		Sphere* cabeza = new Sphere(175, fvec3(0.82, 0.41, 0.11));
		Cylinder* ojo1 = new Cylinder(20, 0, 50, fvec3(0.13, 0.7, 0.67));
		ojo1->setModelMat(translate(ojo1->modelMat(), dvec3(-60, 50, 160)));
		Cylinder* ojo2 = new Cylinder(20, 0, 50, fvec3(0, 0, .5));
		ojo2->setModelMat(translate(ojo2->modelMat(), dvec3(60, 50, 160)));
		Disk* sombrero = new Disk(140, 190, dvec3(1, 0, 0));
		sombrero->setModelMat(translate(sombrero->modelMat(), dvec3(0, 100, 0)));
		sombrero->setModelMat(rotate(sombrero->modelMat(), radians(90.0), dvec3(1, 0, 0)));
		PartialDisk* barba = new PartialDisk(150, 160, 90, 180, fvec3(0, 1, 0));
		barba->setModelMat(translate(barba->modelMat(), dvec3(0, 0, 87.5)));

		gObjects.push_back(cabeza);
		gObjects.push_back(ojo1);
		gObjects.push_back(ojo2);
		gObjects.push_back(sombrero);
		gObjects.push_back(barba);

		//----------------------------------------------------------------------------------------
		//Escena Ana

		/*gObjects.push_back(new EjesRGB(400.0));
		
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
		rgbr->setModelMat(scale(rgbr->modelMat(), s));		*/
	}
	else if (miId == 1)
	{
		AnilloCuadrado* ac = new AnilloCuadrado();
		gObjects.push_back(ac);
		
		//------------------------------------------------------------------------------------------
		//Escena Ana 

		/*gObjects.push_back(new EjesRGB(400.0));

		Estrella3D* estrella3D = new Estrella3D(25, 4, 25,gTextures.at(0));
		estrella3D->setModelMat(translate(estrella3D->modelMat(), dvec3(-300.0,100.0,-300.0)));
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
		*/
	}
	else if (miId == 2) 
	{
		gObjects.push_back(new Cube(100.0));
	}
	else if (miId == 3)
	{		

		CompoundEntity* avion = new CompoundEntity();
		gObjects.push_back(avion);

		Sphere* s = new Sphere(100.0, fvec3(1, 0, 0));
		avion->addEntity(s);

		
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
	sceneDirLight(cam);
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
void Scene::sceneDirLight(Camera const& cam) const {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glm::fvec4 posDir = { 1, 1, 1, 0 };
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(cam.viewMat()));
	glLightfv(GL_LIGHT0, GL_POSITION, value_ptr(posDir));
	glm::fvec4 ambient = { 0, 0, 0, 1 };
	glm::fvec4 diffuse = { 1, 1, 1, 1 };
	glm::fvec4 specular = { 0.5, 0.5, 0.5, 1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, value_ptr(ambient));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, value_ptr(diffuse));
	glLightfv(GL_LIGHT0, GL_SPECULAR, value_ptr(specular));
}

//-------------------------------------------------------------------------


