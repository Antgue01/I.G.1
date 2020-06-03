#include "Fondo.h"
#include <gtc\type_ptr.hpp>
#include "IG1App.h"
Fondo::Fondo()
{
	viewport = new Viewport(IG1App::s_ig1app.winWidth(), IG1App::s_ig1app.winHeight());
	cam = new Camera(viewport);
	img = new Texture();
	img->load("..\\BMPS\\noche.bmp");
	secondImg = new Texture();
	secondImg->load("..\\BMPS\\Zelda.bmp",50);
	mesh = Mesh::generaRectanguloTexCor(2, 2, 1, 1);
	cam->setSize(2, 2);
	cam->set2D();
	mesh->setTwoUnits(true);

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
}

void Fondo::setSize(int xw, int yh)
{
	viewport->setSize(xw, yh);
	
}

void Fondo::render() const
{
	if (mesh != nullptr) {
		glm::dmat4 aMat =  cam->viewMat();
		glDisable(GL_DEPTH_TEST);
		cam->upload();
		
 		glPolygonMode(GL_FRONT, GL_FILL);
		if (secondImg != nullptr)
			secondImg->bind(GL_DECAL,GL_TEXTURE1);
		if (img != nullptr)
			img->bind(GL_DECAL);
		mesh->render();
		if (img != nullptr)
			img->unbind();
		if (secondImg != nullptr)
			secondImg->unbind(GL_TEXTURE1);
		glEnable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
