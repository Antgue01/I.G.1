#include "Fondo.h"
#include <gtc\type_ptr.hpp>

Fondo::Fondo()
{
	viewport = new Viewport(2, 2);
	cam = new Camera(viewport);
	img = new Texture();
	img->load("..\\BMPS\\noche.bmp");
	mesh = Mesh::generaRectanguloTexCor(2, 2, 1, 1);
	cam->foo();
	
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
	cam->setSize(xw, yh);
	
}

void Fondo::render() const
{
	if (mesh != nullptr) {
		glm::dmat4 aMat =  cam->viewMat();
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixd(value_ptr(aMat));
		glPolygonMode(GL_BACK, GL_POINT);
		if (img != nullptr)
			img->bind(GL_REPLACE);
		mesh->render();
		if (img != nullptr)
			img->unbind();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
