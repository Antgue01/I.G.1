//#pragma once
#ifndef _H_Entities_H_
#define _H_Entities_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include "Mesh.h"
#include "Texture.h"

//-------------------------------------------------------------------------

class Abs_Entity  // abstract class
{
public:
	Abs_Entity(): mModelMat(1.0), mColor(1.0) {};  // 4x4 identity matrix
	virtual ~Abs_Entity();

	Abs_Entity(const Abs_Entity& e) = delete;  // no copy constructor
	Abs_Entity& operator=(const Abs_Entity& e) = delete;  // no copy assignment

	virtual void render(glm::dmat4 const& modelViewMat) const = 0;  // abstract method
	virtual void update() { }
	Texture* GetTexture() { return mTexture; }
	// modeling matrix
	glm::dmat4 const& modelMat() const { return mModelMat; };
	void setModelMat(glm::dmat4 const& aMat) { mModelMat = aMat; };
	void setColor(GLuint r, GLuint g, GLuint b, GLuint a) { mColor = { r,g,b,a }; };
	void setTexture(Texture* tex) { mTexture = tex; }
	
protected:

	Texture* mTexture = nullptr;
	Mesh* mMesh = nullptr;   // the mesh
	glm::dmat4 mModelMat;    // modeling matrix
	glm::dvec4 mColor;
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

class Poligono : public Abs_Entity
{
public:
	explicit Poligono(GLuint numL, GLdouble rd);
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

class Sierpinski : public Abs_Entity
{
public:
	explicit Sierpinski(GLuint numP, GLdouble rd);
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

class TrianguloRGB :public Abs_Entity
{
public:
	explicit TrianguloRGB(GLdouble rd, GLdouble rotRadius = 300);
	virtual void render(glm::dmat4 const& modelViewMat) const;
	virtual void update();
private:
	GLdouble internalRotAngle = 0.0;
	GLdouble externalRotAngle = 0.0;
	GLdouble rotationRadius = 0.0;
};

class RectanguloRGB :public Abs_Entity
{
public:
	explicit RectanguloRGB(GLdouble w, GLdouble h);
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

class Estrella3D : public Abs_Entity
{
public:
	explicit Estrella3D(GLdouble re, GLdouble np, GLdouble h,Texture*t);
	virtual void render(glm::dmat4 const& modelViewMat) const;
	virtual void update();
private:
	GLdouble h;
	GLdouble zAngle = 1.0;
	GLdouble yAngle = 1.0;
};

class Caja :public Abs_Entity {
public:
	explicit Caja(GLdouble ld,Texture* t,Texture* t2);
	virtual void render(glm::dmat4 const& modelViewMat) const;
private:
	Texture* texture2 = nullptr;
	Mesh* meshFondo = nullptr;
	GLdouble lado;
};

class Suelo :public Abs_Entity {
public:
	explicit Suelo(GLdouble w, GLdouble h, GLuint rw, GLuint rh,Texture* t);
	virtual void render(glm::dmat4 const& modelViewMat) const;

};
class Foto :public Abs_Entity {
public:
	explicit Foto(GLdouble w, GLdouble h);
	virtual void render(glm::dmat4 const& modelViewMat) const;
	virtual void update();

};
class Plant :public Abs_Entity {
public:
	explicit Plant(GLdouble w, GLdouble h, Texture* t);
	virtual void render(glm::dmat4 const& modelViewMat) const;

};
class Cubo:public Abs_Entity {
public:
	explicit Cubo(GLdouble nl, Texture* t);
	virtual void render(glm::dmat4 const& modelViewMat) const;

};
class QuadricEntity : public Abs_Entity {
public:
	explicit  QuadricEntity(glm::fvec3 color = glm::fvec3(-1, -1, -1));
protected:
	GLUquadricObj* q;
	glm::fvec3 color;
};
class Sphere :public QuadricEntity {
public: 
	explicit Sphere(GLdouble r, glm::fvec3 color = glm::fvec3(-1, -1, -1));
	virtual void render(glm::dmat4 const& modelViewMat) const;
private:
	GLdouble radious;
};
class Cylinder :public QuadricEntity {
public:
	explicit Cylinder(GLdouble baseRadious, GLdouble topRadious, GLdouble height, glm::fvec3 color = glm::fvec3(-1, -1, -1));
	virtual void render(glm::dmat4 const& modelViewMat) const;
private:
	GLdouble baseRadious;
	GLdouble topRadious;
	GLdouble height;
};
class Disk :public QuadricEntity {
public:
	explicit Disk(GLdouble innerRadious, GLdouble outerRadious, glm::fvec3 color = glm::fvec3(-1, -1, -1));
	virtual void render(glm::dmat4 const& modelViewMat) const;

private:
	GLdouble innerRadious;
	GLdouble outerRadious;
};
class PartialDisk :public QuadricEntity {
public:
	explicit PartialDisk(GLdouble innerRadious, GLdouble outerRadious, GLdouble startAngle, GLdouble sweepAngle, glm::fvec3 color = glm::fvec3(-1, -1, -1));
	virtual void render(glm::dmat4 const& modelViewMat) const;

private:
	GLdouble innerRadious;
	GLdouble outerRadious;
	GLdouble startAngle;
	GLdouble sweepAngle;
};
#endif //_H_Entities_H_