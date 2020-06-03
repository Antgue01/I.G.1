#include "Fondo.h"
#include <gtc\type_ptr.hpp>
#include "IG1App.h"
Fondo::Fondo()
{
	//ponemos el viewport para que ocupe toda la pantalla
	viewport = new Viewport(IG1App::s_ig1app.winWidth(), IG1App::s_ig1app.winHeight());
	cam = new Camera(viewport);
	img = new Texture();
	img->load("..\\BMPS\\noche.bmp");
	secondImg = new Texture(); //EXTRA 2
	secondImg->load("..\\BMPS\\Zelda.bmp", 50); //EXTRA 2
	//malla de 2x2
	mesh = Mesh::generaRectanguloTexCor(2, 2, 1, 1);
	//ponemos la cámara al mismo tamaño que la malla para que se vea entera
	cam->setSize(2, 2);
	cam->set2D();
	mesh->setTwoUnits(true);//EXTRA 2

}

Fondo::~Fondo()
{
	delete cam;
	cam = nullptr;
	delete mesh;
	mesh = nullptr;
	delete img;
	img = nullptr;
	delete viewport;
	viewport = nullptr;
	delete secondImg; //EXTRA 2
	secondImg = nullptr; //EXTRA 2
}

void Fondo::setSize(int xw, int yh)
{
	//Ajustamos el tamaño del viewport
	viewport->setSize(xw, yh);

}

void Fondo::render() const
{
	if (mesh != nullptr) {
		//desactivamos el test de profundidad para que el fondo no tape lo demás
		glDisable(GL_DEPTH_TEST);
		cam->upload();

		glPolygonMode(GL_FRONT, GL_FILL);
		if (secondImg != nullptr) //EXTRA 2
			secondImg->bind(GL_DECAL, GL_TEXTURE1); //EXTRA 2
		if (img != nullptr)
			img->bind(GL_DECAL);
		mesh->render();
		if (img != nullptr)
			img->unbind();
		if (secondImg != nullptr) //EXTRA 2
			secondImg->unbind(GL_TEXTURE1); //EXTRA 2
		glEnable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
