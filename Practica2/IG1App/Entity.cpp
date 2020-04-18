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

EjesRGB::EjesRGB(GLdouble l): Abs_Entity()
{
  mMesh = Mesh::createRGBAxes(l);
}
//-------------------------------------------------------------------------

EjesRGB::~EjesRGB() 
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
		mMesh->render();
		glLineWidth(1);
	}
}
//-------------------------------------------------------------------------
 
QuadricEntity::QuadricEntity() {
	q = gluNewQuadric();
}

Sphere::Sphere(GLdouble r)
{
	radius = r;
	color = glm::fvec3(0, 255, 0);
}

void Sphere::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);

	// Aquí se puede fijar el color de la esfera así:
	 glEnable(GL_COLOR_MATERIAL);
	 glColor3f(color.r,color.g,color.b);
	 
	// Aquí se puede fijar el modo de dibujar la esfera:
	 gluQuadricDrawStyle(q, GLU_FILL);

	 gluSphere(q, radius, 50, 50);

	// Aquí se debe recuperar el color:
	 glColor3f(1.0, 1.0, 1.0);
}

Cylinder::Cylinder(GLdouble baseR, GLdouble topR, GLdouble h)
{
	baseRadius = baseR;
	topRadius = topR;
	height = h;
	color = glm::fvec3(0, 255, 0);
}
void Cylinder::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);

	// Aquí se puede fijar el color de la esfera así:
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(color.r, color.g, color.b);

   // Aquí se puede fijar el modo de dibujar la esfera:
	gluQuadricDrawStyle(q, GLU_FILL);

	gluCylinder(q, baseRadius,topRadius,height,50,50);

	// Aquí se debe recuperar el color:
	glColor3f(1.0, 1.0, 1.0);
}

Disk::Disk(GLdouble innerR, GLdouble outerR)
{
	innerRadius = innerR;
	outerRadius = outerR;
	color = glm::fvec3(0, 255, 0);
}
void Disk::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);

	// Aquí se puede fijar el color de la esfera así:
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(0,255,0);

   // Aquí se puede fijar el modo de dibujar la esfera:
	gluQuadricDrawStyle(q, GLU_FILL);

	gluDisk(q, innerRadius, outerRadius, 50, 50);

	// Aquí se debe recuperar el color:
	glColor3f(1.0, 1.0, 1.0);
}

PartialDisk::PartialDisk(GLdouble innerR, GLdouble outerR, GLdouble startA, GLdouble sweepA)
{
	innerRadius = innerR;
	outerRadius = outerR;
	startAngle = startA;
	sweepAngle = sweepA;
	color = glm::fvec3(0, 255, 0);
}
void PartialDisk::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);

	// Aquí se puede fijar el color de la esfera así:
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(0,255,0);

   // Aquí se puede fijar el modo de dibujar la esfera:
	gluQuadricDrawStyle(q, GLU_FILL);

	gluPartialDisk(q, innerRadius, outerRadius, 50, 50,startAngle,sweepAngle);

	// Aquí se debe recuperar el color:
	glColor3f(1.0, 1.0, 1.0);
}