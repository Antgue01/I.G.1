#include "Entity.h"

#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>

using namespace glm;

//-------------------------------------------------------------------------

void Abs_Entity::upload(dmat4 const& modelViewMat) const
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(modelViewMat));  // transfers modelView matrix to the GPU
}
Abs_Entity::~Abs_Entity() {
	delete mMesh; mMesh = nullptr;
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void Abs_Entity::Colour(GLuint r, GLuint g, GLuint b, GLuint a) {
	mcolor = { r,g,b,a };
}
EjesRGB::EjesRGB(GLdouble l) : Abs_Entity()
{
	mMesh = Mesh::createRGBAxes(l);
}
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------

void EjesRGB::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glLineWidth(2);
		mMesh->render();
		glLineWidth(1);
	}
}
Poligono::Poligono(GLuint nVert, GLdouble radious) {
	mMesh = mMesh->generaPoligono(nVert, radious);
}
void Poligono::render(dmat4 const& aMat)const {
	dmat4 nMat = aMat * mModelMat;
	upload(nMat);
	glLineWidth(2);
	glColor3d(mcolor.r, mcolor.g, mcolor.b);
	mMesh->render();
	glLineWidth(1);
	glColor3d(1, 1, 1);
}
Sierpinski::Sierpinski(GLuint nVert, GLdouble r) {
	mMesh = mMesh->generaSierpinski(nVert, r);
}
void Sierpinski::render(dmat4 const& M)const {
	dmat4 Mat = M * mModelMat;
	upload(Mat);
	glPointSize(2);
	glColor4dv(value_ptr(mcolor));
	mMesh->render();
	glPointSize(1);
	GLdouble* df = new GLdouble(1.0);
	glColor4dv(df);
	delete df;
}
//-------------------------------------------------------------------------
