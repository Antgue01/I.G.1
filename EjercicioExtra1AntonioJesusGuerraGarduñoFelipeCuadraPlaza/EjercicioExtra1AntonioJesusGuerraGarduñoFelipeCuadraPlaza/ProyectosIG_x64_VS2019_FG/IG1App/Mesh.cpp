#include "Mesh.h"
#include "CheckML.h"
#include <fstream>
using namespace std;
using namespace glm;

//-------------------------------------------------------------------------

void Mesh::draw() const
{
	glDrawArrays(mPrimitive, 0, size());   // primitive graphic, first index and number of elements to be rendered
}
//-------------------------------------------------------------------------

void Mesh::render() const
{
	if (vVertices.size() > 0) {  // transfer data
	  // transfer the coordinates of the vertices
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_DOUBLE, 0, vVertices.data());  // number of coordinates per vertex, type of each coordinate, stride, pointer 
		if (vColors.size() > 0) { // transfer colors
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_DOUBLE, 0, vColors.data());  // components number (rgba=4), type of each component, stride, pointer  
		}

		if (vTexCoords.size() > 0)
		{
			//activamos y subimos la unidad de textura 0
			glClientActiveTexture(GL_TEXTURE0); //EXTRA 2
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_DOUBLE, 0, vTexCoords.data());
			//solo activamos y subimos la textura 1 si hay que renderizar las 2
			if (TwoUnits) //EXTRA 2
			{
				glClientActiveTexture(GL_TEXTURE1); //EXTRA 2
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_DOUBLE, 0, vTexCoords.data());
			}
		}



		draw();

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_DOUBLE);
	}
}
//-------------------------------------------------------------------------

Mesh* Mesh::createRGBAxes(GLdouble l)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_LINES;

	mesh->mNumVertices = 6;
	mesh->vVertices.reserve(mesh->mNumVertices);

	// X axis vertices
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
	mesh->vVertices.emplace_back(l, 0.0, 0.0);
	// Y axis vertices
	mesh->vVertices.emplace_back(0, 0.0, 0.0);
	mesh->vVertices.emplace_back(0.0, l, 0.0);
	// Z axis vertices
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
	mesh->vVertices.emplace_back(0.0, 0.0, l);

	mesh->vColors.reserve(mesh->mNumVertices);
	// X axis color: red  (Alpha = 1 : fully opaque)
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	// Y axis color: green
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	// Z axis color: blue
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

	return mesh;
}
//-------------------------------------------------------------------------

Mesh* Mesh::generaPoligono(GLuint numL, GLdouble rd)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_LINE_LOOP;
	mesh->mNumVertices = numL;
	mesh->vVertices.reserve(mesh->mNumVertices);

	GLdouble angle = radians(90.0);

	for (GLuint i = 0; i < numL; i++)
	{
		GLdouble x = rd * cos(angle);
		GLdouble y = rd * sin(angle);

		mesh->vVertices.emplace_back(x, y, 0.0);
		angle += radians(360.0 / numL);
	}

	return mesh;
}

Mesh* Mesh::generaSierpinski(GLdouble rd, GLuint numP)
{
	Mesh* triangulo = generaPoligono(3, rd);
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_POINTS;
	mesh->vVertices = triangulo->vertices();
	mesh->mNumVertices = 3 * (numP + 1);
	mesh->vVertices.reserve(mesh->mNumVertices);

	vec3 verticeRandom = mesh->vVertices.at(rand() % 3);

	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		int access = rand() % 3;
		verticeRandom = { (verticeRandom.x + triangulo->vVertices[access].x) / 2,(verticeRandom.y + triangulo->vVertices[access].y) / 2,(verticeRandom.z + triangulo->vVertices[access].z) / 2 };
		mesh->vVertices.emplace_back(verticeRandom);
	}
	delete triangulo;
	triangulo = nullptr;
	return mesh;
}

Mesh* Mesh::generaTrianguloRGB(GLdouble rd)
{
	Mesh* mesh = generaPoligono(3, rd);

	mesh->mPrimitive = GL_TRIANGLES;

	mesh->vColors.reserve(3);
	mesh->vColors.emplace_back(255, 0, 0, 255);
	mesh->vColors.emplace_back(0, 255, 0, 255);
	mesh->vColors.emplace_back(0, 0, 255, 255);

	return mesh;
}

Mesh* Mesh::generaRectangulo(GLdouble w, GLdouble h)
{
	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 4;
	mesh->vVertices.reserve(mesh->mNumVertices);

	vec3 punt = { -w / 2,-h / 2,0 };
	mesh->vVertices.emplace_back(punt);

	punt = { punt.x,punt.y + h,0 };
	mesh->vVertices.emplace_back(punt);

	punt = { punt.x + w,punt.y - h,0 };
	mesh->vVertices.emplace_back(punt);

	punt = { punt.x,punt.y + h,0 };
	mesh->vVertices.emplace_back(punt);

	return mesh;
}

Mesh* Mesh::generaRectanguloRGB(GLdouble w, GLdouble h)
{
	Mesh* mesh = generaRectangulo(w, h);

	mesh->vColors.reserve(mesh->mNumVertices);
	mesh->vColors.emplace_back(255, 0, 0, 255);
	mesh->vColors.emplace_back(0, 0, 255, 255);
	mesh->vColors.emplace_back(0, 0, 255, 255);
	mesh->vColors.emplace_back(0, 255, 0, 255);

	return mesh;
}
Mesh* Mesh::generaEstrella3D(GLdouble re, GLdouble np, GLdouble h)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_TRIANGLE_FAN;
	mesh->mNumVertices = 2 * np + 2;
	mesh->vVertices.reserve(mesh->mNumVertices);

	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);

	GLdouble angle = radians(90.0);

	for (GLuint i = 0; i < mesh->mNumVertices - 1; i++)
	{
		if (i % 2 == 0)mesh->vVertices.emplace_back(re * cos(angle), re * sin(angle), h);
		else mesh->vVertices.emplace_back((re / 2) * cos(angle), (re / 2) * sin(angle), h);

		angle += radians(180.0 / np);
	}

	return mesh;
}
Mesh* Mesh::generaContCubo(GLdouble ld) {
	Mesh* cubo = new Mesh();
	cubo->mPrimitive = GL_TRIANGLE_STRIP;
	cubo->mNumVertices = 10;
	cubo->vVertices.reserve(cubo->mNumVertices);

	int n = ld / 2;
	//frontal
	cubo->vVertices.emplace_back(-n, -n, n);
	cubo->vVertices.emplace_back(-n, n, n);
	cubo->vVertices.emplace_back(n, -n, n);
	cubo->vVertices.emplace_back(n, n, n);

	//derecha
	cubo->vVertices.emplace_back(n, -n, n - ld);
	cubo->vVertices.emplace_back(n, n, n - ld);

	//trasera
	cubo->vVertices.emplace_back(n - ld, -n, -n);
	cubo->vVertices.emplace_back(n - ld, n, -n);

	//izquierda
	cubo->vVertices.emplace_back(-n, -n, n);
	cubo->vVertices.emplace_back(-n, n, n);


	return cubo;
}

Mesh* Mesh::generaEstrellaTexCor(GLdouble re, GLuint np, GLdouble h)
{
	Mesh* mesh = generaEstrella3D(re, np, h);

	mesh->vTexCoords.reserve(mesh->mNumVertices);
	mesh->vTexCoords.emplace_back(0.5, 0.5);
	GLdouble angle = radians(90.0);

	for (GLuint i = 0; i < mesh->mNumVertices - 1; i++)
	{
		if (i % 2 == 0)mesh->vTexCoords.emplace_back(0.5 + 0.5 * cos(angle), 0.5 + 0.5 * sin(angle));
		else mesh->vTexCoords.emplace_back(0.5 + 0.25 * cos(angle), 0.5 + 0.25 * sin(angle));

		angle += radians(180.0 / np);
	}

	return mesh;
}

Mesh* Mesh::generaRectanguloTexCor(GLdouble w, GLdouble h, GLuint rw, GLuint rh)
{
	Mesh* mesh = generaRectangulo(w, h);
	mesh->vTexCoords.reserve(mesh->mNumVertices);

	mesh->vTexCoords.emplace_back(0.0, 0.0);
	mesh->vTexCoords.emplace_back(0.0, rh);
	mesh->vTexCoords.emplace_back(rw, 0.0);
	mesh->vTexCoords.emplace_back(rw, rh);


	return mesh;
}
Mesh* Mesh::generaCajaTexCor(GLdouble nl) {
	Mesh* m = m->generaContCubo(nl);

	m->vTexCoords.reserve(16); //4 caras 

	m->vTexCoords.emplace_back(0, 1);
	m->vTexCoords.emplace_back(0, 0);
	m->vTexCoords.emplace_back(1, 1);
	m->vTexCoords.emplace_back(1, 0);

	m->vTexCoords.emplace_back(0, 1);
	m->vTexCoords.emplace_back(0, 0);
	m->vTexCoords.emplace_back(1, 1);
	m->vTexCoords.emplace_back(1, 0);

	m->vTexCoords.emplace_back(0, 1);
	m->vTexCoords.emplace_back(0, 0);
	m->vTexCoords.emplace_back(1, 1);
	m->vTexCoords.emplace_back(1, 0);

	m->vTexCoords.emplace_back(0, 1);
	m->vTexCoords.emplace_back(0, 0);
	m->vTexCoords.emplace_back(1, 1);
	m->vTexCoords.emplace_back(1, 0);

	return m;
}

