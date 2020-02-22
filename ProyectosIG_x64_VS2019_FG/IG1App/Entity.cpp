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
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

EjesRGB::EjesRGB(GLdouble l) : Abs_Entity()
{
	mMesh = Mesh::createRGBAxes(l);
}
//-------------------------------------------------------------------------

Abs_Entity::~Abs_Entity()
{
	delete mMesh; mMesh = nullptr;
};
//-------------------------------------------------------------------------

void EjesRGB::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glLineWidth(2);
		if (mTexture != nullptr)mTexture->bind(GL_REPLACE);
		mMesh->render();
		mTexture->unbind();
		glLineWidth(1);
	}
}
//-------------------------------------------------------------------------

Poligono::Poligono(GLuint numL, GLdouble rd) : Abs_Entity()
{
	mMesh = Mesh::generaPoligono(numL, rd);
}

void Poligono::render(dmat4 const& modelViewMat)const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glLineWidth(2);
		glColor3d(mColor.r, mColor.g, mColor.b);
		if (mTexture != nullptr)mTexture->bind(GL_REPLACE);
		mMesh->render();
		mTexture->unbind();
		glLineWidth(1);
		glColor3d(1, 1, 1);
	}
}

Sierpinski::Sierpinski(GLuint numP, GLdouble rd) : Abs_Entity()
{
	mMesh = Mesh::generaSierpinski(numP, rd);
}

void Sierpinski::render(dmat4 const& modelViewMat)const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glPointSize(3);
		glColor4dv(value_ptr(mColor));
		if (mTexture != nullptr)mTexture->bind(GL_REPLACE);
		mMesh->render();
		mTexture->unbind();
		glPointSize(1);
		GLdouble db = 1.0;
		glColor4dv(&db);
	}
}

TrianguloRGB::TrianguloRGB(GLdouble rd, GLdouble rotRadius) :Abs_Entity()
{
	rotationRadius = rotRadius;
	mMesh = Mesh::generaTrianguloRGB(rd);
}

void TrianguloRGB::render(glm::dmat4 const& modelViewMat)const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glPolygonMode(GL_BACK, GL_POINT);
		if (mTexture != nullptr)mTexture->bind(GL_REPLACE);
		mMesh->render();
		mTexture->unbind();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void TrianguloRGB::update()
{
	mModelMat = dmat4(1);

	if (radians(internalRotAngle) > 360.0)internalRotAngle = 0.0;
	else internalRotAngle += 15.0;

	if (radians(externalRotAngle) > 360.0)externalRotAngle = 0.0;
	else externalRotAngle += 5.0;

	mModelMat = translate(mModelMat, dvec3(rotationRadius * cos(radians(externalRotAngle)), rotationRadius * sin(radians(externalRotAngle)), 0.0));
	mModelMat = rotate(mModelMat, radians(internalRotAngle), dvec3(0.0, 0.0, 1.0));
}

RectanguloRGB::RectanguloRGB(GLdouble w, GLdouble h) :Abs_Entity()
{
	mMesh = Mesh::generaRectanguloRGB(w, h);
}

void RectanguloRGB::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glPolygonMode(GL_FRONT, GL_POINT);
		if (mTexture != nullptr)mTexture->bind(GL_REPLACE);
		mMesh->render();
		mTexture->unbind();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
Estrella3D::Estrella3D(GLdouble re, GLdouble np, GLdouble h, Texture* t) :Abs_Entity(), h(h)
{
	mMesh = Mesh::generaEstrellaTexCor(re, np, h);
	mTexture = t;
}

void Estrella3D::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glLineWidth(2);
		if (mTexture != nullptr)mTexture->bind(GL_REPLACE);
		mMesh->render();

		aMat = rotate(aMat, radians(180.0), dvec3(1, 0, 0));
		upload(aMat);
		mMesh->render();
		mTexture->unbind();
		glLineWidth(1);
	}
}

void Estrella3D::update()
{
	mModelMat = rotate(dmat4(1), radians(yAngle), dvec3(0.0, 1.0, 0.0));
	mModelMat = rotate(mModelMat, radians(zAngle), dvec3(0.0, 0.0, 1.0));
	zAngle++;
	yAngle++;
}

Caja::Caja(GLdouble ld) :Abs_Entity()
{
	mMesh = Mesh::generaContCubo(ld);
}
void Caja::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if (mTexture != nullptr)mTexture->bind(GL_REPLACE);
		mMesh->render();
		mTexture->unbind();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

Suelo::Suelo(GLdouble w, GLdouble h, GLuint rw, GLuint rh,Texture* t)
{
	mTexture = t;
	mMesh = Mesh::generaRectanguloTexCor(w, h, rw, rh);
    mModelMat = rotate(mModelMat, radians(180.0), dvec3(0.0, 1.0, 1.0));
	
}

void Suelo::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		if (mTexture != nullptr)mTexture->bind(GL_MODULATE);
		mMesh->render();
		mTexture->unbind();
		
	}
}

