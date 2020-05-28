#include "Entity.h"
#include "IG1App.h"
#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>
#include <iostream>

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
		glEnable(GL_COLOR_MATERIAL);
		glLineWidth(2);
		if (mTexture != nullptr)mTexture->bind(GL_REPLACE);
		mMesh->render();
		mTexture->unbind();
		glLineWidth(1);
		glDisable(GL_COLOR_MATERIAL);

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
		glColor4d(1.0, 1.0, 1.0, 1.0);
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
	mModelMat = translate(dmat4(1), dvec3(-300, 100, -300));
	mModelMat = rotate(mModelMat, radians(yAngle), dvec3(0.0, 1.0, 0.0));
	mModelMat = rotate(mModelMat, radians(zAngle), dvec3(0.0, 0, 1.0));

	zAngle++;
	yAngle++;
}

Caja::Caja(GLdouble ld, Texture* t, Texture* t2) :Abs_Entity()
{
	mMesh = Mesh::generaCajaTexCor(ld);
	meshFondo = Mesh::generaRectanguloTexCor(ld, ld, 1, 1);
	mTexture = t;
	texture2 = t2;
	lado = ld;
}
void Caja::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {

		glEnable(GL_CULL_FACE);

		//fondo caja
		dmat4 fondoMatriz = modelViewMat * mModelMat;
		fondoMatriz = translate(fondoMatriz, dvec3(0, -lado / 2, 0));
		fondoMatriz = rotate(fondoMatriz, radians(180.0), dvec3(0.0, 1.0, 1.0));
		upload(fondoMatriz);

		glCullFace(GL_FRONT);
		if (texture2 != nullptr)texture2->bind(GL_REPLACE);
		meshFondo->render();
		texture2->unbind();

		//lados caja
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);

		glCullFace(GL_FRONT);
		if (mTexture != nullptr)mTexture->bind(GL_REPLACE);
		mMesh->render();
		mTexture->unbind();

		glCullFace(GL_BACK);
		if (texture2 != nullptr)texture2->bind(GL_REPLACE);
		mMesh->render();
		texture2->unbind();

		glDisable(GL_CULL_FACE);
	}
}

Suelo::Suelo(GLdouble w, GLdouble h, GLuint rw, GLuint rh, Texture* t)
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

Foto::Foto(GLdouble w, GLdouble h) {
	mMesh = Mesh::generaRectanguloTexCor(w, h, 1, 1);
	mTexture = new Texture();
	mModelMat = translate(mModelMat, dvec3(0, 1, 0));
	mModelMat = rotate(mModelMat, radians(180.0), dvec3(0.0, 1.0, 1.0));

}
void Foto::render(glm::dmat4 const& modelViewMat) const {
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);

		if (mTexture != nullptr)mTexture->bind(GL_MODULATE);
		mMesh->render();
		mTexture->unbind();
	}
}
void Foto::update() {
	mTexture->loadColorBuffer(IG1App::s_ig1app.winWidth(), IG1App::s_ig1app.winHeight());
}

Plant::Plant(GLdouble w, GLdouble h, Texture* t) {
	mMesh = Mesh::generaRectanguloTexCor(w, h, 1, 1);
	mTexture = t;
}
void Plant::render(glm::dmat4 const& modelViewMat) const {
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		aMat = rotate(aMat, radians(180.0), dvec3(0, 0, 1));
		glAlphaFunc(GL_GREATER, 0);
		upload(aMat);

		if (mTexture != nullptr)mTexture->bind(GL_MODULATE);
		mMesh->render();
		aMat = rotate(aMat, radians(90.0), dvec3(0, 1, 0));
		upload(aMat);
		mMesh->render();
		mTexture->unbind();
	}
}
void Cubo::render(glm::dmat4 const& modelViewMat) const {
	if (mMesh != nullptr) {
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		if (mTexture != nullptr) mTexture->bind(GL_REPLACE);
		mMesh->render();
		mTexture->unbind();
		glBlendFunc(1, 0);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}
}
Cubo::Cubo(GLdouble nl, Texture* t) {
	mMesh = Mesh::generaCajaTexCor(nl);
	mTexture = t;
}

QuadricEntity::QuadricEntity(glm::fvec3 Color) :Abs_Entity()
{
	q = gluNewQuadric();
	color = Color;
}

Sphere::Sphere(GLdouble r, glm::fvec3 Color) :QuadricEntity(Color), radious(r)
{
}

void Sphere::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	// Aquí se puede fijar el color de la esfera así:
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(color.r, color.g, color.b);
	// Aquí se puede fijar el modo de dibujar la esfera:
	gluQuadricDrawStyle(q, GLU_FILL);
	gluSphere(q, radious, 50, 50);
	// Aquí se debe recuperar el color:
	glDisable(GL_COLOR_MATERIAL);
	glColor3f(1.0, 1.0, 1.0);
}

Cylinder::Cylinder(GLdouble baseRadious, GLdouble topRadious, GLdouble height, glm::fvec3 color) :QuadricEntity(color),
baseRadious(baseRadious), topRadious(topRadious), height(height)
{
}

void Cylinder::render(glm::dmat4 const& modelViewMat)const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	// Aquí se puede fijar el color de la esfera así:
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(color.r, color.g, color.b);
	// Aquí se puede fijar el modo de dibujar la esfera:
	gluQuadricDrawStyle(q, GLU_FILL);
	gluCylinder(q, baseRadious, topRadious, height, 50, 50);
	// Aquí se debe recuperar el color:
	glDisable(GL_COLOR_MATERIAL);
	glColor3f(1.0, 1.0, 1.0);
}

Disk::Disk(GLdouble innerRadious, GLdouble outerRadious, glm::fvec3 color)
	:QuadricEntity(color), innerRadious(innerRadious), outerRadious(outerRadious)
{
}

void Disk::render(glm::dmat4 const& modelViewMat)const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	// Aquí se puede fijar el color de la esfera así:
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(color.r, color.g, color.b);
	// Aquí se puede fijar el modo de dibujar la esfera:
	gluQuadricDrawStyle(q, GLU_FILL);
	gluDisk(q, innerRadious, outerRadious, 50, 50);
	// Aquí se debe recuperar el color:
	glDisable(GL_COLOR_MATERIAL);
	glColor3f(1.0, 1.0, 1.0);
}

PartialDisk::PartialDisk(GLdouble innerRadious, GLdouble outerRadious, GLdouble startAngle, GLdouble sweepAngle, glm::fvec3 color) :
	QuadricEntity(color), innerRadious(innerRadious), outerRadious(outerRadious), startAngle(startAngle),
	sweepAngle(sweepAngle)
{
}

void PartialDisk::render(glm::dmat4 const& modelViewMat)const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	// Aquí se puede fijar el color de la esfera así:
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(color.r, color.g, color.b);
	// Aquí se puede fijar el modo de dibujar la esfera:
	gluQuadricDrawStyle(q, GLU_FILL);
	gluPartialDisk(q, innerRadious, outerRadious, 50, 50, startAngle, sweepAngle);
	// Aquí se debe recuperar el color:
	glColor3f(1.0, 1.0, 1.0);
	glDisable(GL_COLOR_MATERIAL);
}

AnilloCuadrado::AnilloCuadrado()
{
	mMesh = IndexMesh::generaAnilloCuadrado();
}

void AnilloCuadrado::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glEnable(GL_COLOR_MATERIAL);

		glPolygonMode(GL_BACK, GL_POINT);
		if (mTexture != nullptr)mTexture->bind(GL_REPLACE);
		mMesh->render();
		mTexture->unbind();
		glDisable(GL_COLOR_MATERIAL);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

Cube::Cube(double l, glm::dvec4 color) :EntityWithMaterial(), color(color)
{
	mMesh = IndexMesh::generaIndexCuboConTapas(l, color);
}

void Cube::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glPolygonMode(GL_BACK, GL_POINT);
		if (mTexture != nullptr)mTexture->bind(GL_REPLACE);
		if (material != nullptr) {
			material->upload();
		}
		else
			glEnable(GL_COLOR_MATERIAL);
		mMesh->render();
		mTexture->unbind();
		glDisable(GL_COLOR_MATERIAL);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void Cube::setCopper()
{
	if (material != nullptr)
		material->setCopper();
}

CompoundEntity :: ~CompoundEntity()
{
	for (Abs_Entity* el : gObjects)
	{
		delete el;  el = nullptr;
	}
}

Abs_Entity* CompoundEntity::getEntity(int id)
{
	if (id < gObjects.size() && id >= 0)
		return gObjects.at(id);
	else return nullptr;
}

void CompoundEntity::render(glm::dmat4 const& modelViewMat)const
{
	dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
	upload(aMat);

	for (Abs_Entity* e : gObjects)
	{

		e->render(aMat);
	}
}

Cono::Cono(int h, int r, int n) :h(h), r(r), n(n)
{
	std::vector<glm::dvec3> perfil;
	perfil.push_back(glm::dvec3(.5, 0, 0));
	perfil.push_back(glm::dvec3(r, 0, 0));
	perfil.push_back(glm::dvec3(.5, h, 0));
	mMesh = MbR::generaIndexMeshByRevolution(perfil.size(), n, perfil);
}

void Cono::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glPolygonMode(GL_BACK, GL_LINE);
		glEnable(GL_COLOR_MATERIAL);
		if (mTexture != nullptr)mTexture->bind(GL_REPLACE);
		mMesh->render();
		mTexture->unbind();
		glDisable(GL_COLOR_MATERIAL);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

Esfera::Esfera(int r, int p, int m, glm::dvec4 color) :EntityWithMaterial(), r(r), p(p), m(m) {

	std::vector<glm::dvec3> perfil;
	GLdouble angle = radians(-90.0);
	GLdouble sum = radians(180.0 / p);

	perfil.push_back(glm::dvec3(0.0, r * sin(angle), 0.0));
	angle -= sum;
	for (int i = 0; i < p; i++)
	{
		perfil.push_back(glm::dvec3(r * cos(angle), r * sin(angle), 0.0));
		angle -= sum;
	}

	mMesh = MbR::generaIndexMeshByRevolution(p + 1, m, perfil, color);
}

void Esfera::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glPolygonMode(GL_FRONT, GL_FILL);
		if (material != nullptr)
		{
			material->upload();
		}
		else
		{
			glEnable(GL_COLOR_MATERIAL);
		}
		if (mTexture != nullptr)mTexture->bind(GL_REPLACE);
		mMesh->render();
		mTexture->unbind();
		glDisable(GL_COLOR_MATERIAL);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void Esfera::setGold()
{
	if (material != nullptr)
		material->setGold();
}

void EntityWithMaterial::quitMaterial()
{
	if (material != nullptr)
	{
		delete material;
		material = nullptr;
	}
}
Plane::Plane(SpotLight* sp, GLdouble WorldRadious) :CompoundEntity(), spotLight(sp), rotationRadius(WorldRadious), HelixRotAngle(3) {
	helices = new CompoundEntity();
	helices->addEntity(new Cylinder(30, 10, 100, glm::fvec3(0, 0, 1)));
	helices->addEntity(new Cylinder(30, 10, 100, glm::fvec3(0, 0, 1)));
	for (Abs_Entity* c : helices->getObjects())
	{
		c->setModelMat(translate(c->modelMat(), glm::dvec3(0, 0, 100)));
		c->setModelMat(rotate(c->modelMat(), radians(90.0), glm::dvec3(0, 1, 0)));
	}
	helices->getEntity(1)->setModelMat(rotate(helices->getEntity(1)->modelMat(), radians(180.0), glm::dvec3(0, 1, 0)));
	Sphere* s = new Sphere(100.0, glm::fvec3(1, 0, 0));
	CompoundEntity* chasis = new CompoundEntity();
	chasis->addEntity(helices);
	chasis->addEntity(s);
	Cube* cube = new Cube(100, glm::dvec4(0, 1, 0, 1));
	cube->setModelMat(scale(cube->modelMat(), glm::dvec3(4, .3, 1)));
	addEntity(chasis);
	addEntity(cube);
	cube->setMaterial(new Material());
	cube->setCopper();
}

void Plane::move()
{
	//for (Abs_Entity* ent : gObjects)
	//	ent->setModelMat(dmat4(1));

	if (PlaneRotAngle > 360.0)
		PlaneRotAngle = 0.0;
	else PlaneRotAngle += 1.0;

	//for (Abs_Entity* ent : gObjects)
	//{
	//	//ent->setModelMat(rotate(ent->modelMat(), radians(1.0), dvec3(0, -1, 0)));
	//	//ent->setModelMat(rotate(ent->modelMat(), radians(1.0), dvec3(0, 0, 1)));
	//}
	////movemos y rotamos el avión
	//for (Abs_Entity* ent : gObjects)
	//{
	//	std::cout << mModelMat[0].x << std::endl;
	//	std::cout << mModelMat[1].y << std::endl;
	//	std::cout << mModelMat[2].z<<std::endl;
	//	ent->setModelMat(translate(modelMat(), dvec3(0, -1+ 1 * sin(radians(1.0)), +1 * cos(radians(1.0)))));
	//	ent->setModelMat(rotate(modelMat(),radians(2.0), dvec3(0, 1, 0)));
	//	//ent->setModelMat(rotate(ent->modelMat(), radians(1.0), dvec3(0, -1, 0)));
	//	//ent->setModelMat(rotate(ent->modelMat(), radians(1.0), dvec3(0, 0, 1)));
	//}
	//gObjects.at(1)->setModelMat(scale(gObjects.at(1)->modelMat(), glm::dvec3(4, .3, 1)));
	setModelMat(translate(modelMat(), dvec3(0, + 20* sin(radians(1.0)), 0 +  20* cos(radians(1.0)))));
	setModelMat(rotate(modelMat(),radians(1.0), dvec3(1,0,0)));


	//rotamos las hélices
	helices->getObjects().at(0)->setModelMat(rotate(helices->getObjects().at(0)->modelMat(), radians(-HelixRotAngle), dvec3(1, 0, 0)));
	helices->getObjects().at(1)->setModelMat(rotate(helices->getObjects().at(1)->modelMat(), radians(HelixRotAngle), dvec3(1, 0, 0)));

}
