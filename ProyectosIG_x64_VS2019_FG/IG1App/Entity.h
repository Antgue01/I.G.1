//#pragma once
#ifndef _H_Entities_H_
#define _H_Entities_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include "Mesh.h"
using namespace glm;
//-------------------------------------------------------------------------

class Abs_Entity  // abstract class
{
public:
	Abs_Entity() : mModelMat(1.0), mcolor(1) {};  // 4x4 identity matrix
	virtual ~Abs_Entity();

	Abs_Entity(const Abs_Entity& e) = delete;  // no copy constructor
	Abs_Entity& operator=(const Abs_Entity& e) = delete;  // no copy assignment

	virtual void render(glm::dmat4 const& modelViewMat) const = 0;  // abstract method

	// modeling matrix
	glm::dmat4 const& modelMat() const { return mModelMat; };
	void setModelMat(glm::dmat4 const& aMat) { mModelMat = aMat; };
	void Colour(GLuint r, GLuint g, GLuint b, GLuint a);

protected:

	Mesh* mMesh = nullptr;   // the mesh
	glm::dmat4 mModelMat;    // modeling matrix
	dvec4 mcolor;
	// transfers modelViewMat to the GPU
	virtual void upload(glm::dmat4 const& mModelViewMat) const;
};
//-------------------------------------------------------------------------

class EjesRGB : public Abs_Entity
{
public:
	explicit EjesRGB(GLdouble l);
	
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

//-------------------------------------------------------------------------
class Poligono :public Abs_Entity {
public:
	explicit Poligono(GLuint nVert, GLdouble radious);
	virtual void  render(dmat4 const& aMat) const;
};
class Sierpinski :public Abs_Entity {
public:
	explicit Sierpinski(GLuint nvert, GLdouble r);
	virtual void render(dmat4 const& aMat) const;
};
#endif //_H_Entities_H_