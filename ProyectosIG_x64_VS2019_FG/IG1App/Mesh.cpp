#include "Mesh.h"
#include "CheckML.h"
#include <fstream>
#include <iostream>
using namespace std;
using namespace glm;

//-------------------------------------------------------------------------

void Mesh::draw() const
{
	//if(vIndices.size()<=0)									   
	glDrawArrays(mPrimitive, 0, size());   // primitive graphic, first index and number of elements to be rendered
	//else
	//glDrawElements(mPrimitive, vIndices.size(), GL_UNSIGNED_INT, vIndices.data());

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
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_DOUBLE, 0, vTexCoords.data());
		}

		if (vNormals.size() > 0)
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_DOUBLE, 0, vNormals.data());
		}

		draw();

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
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

	mesh->vTexCoords.emplace_back(0.0, rh);
	mesh->vTexCoords.emplace_back(0.0, 0.0);
	mesh->vTexCoords.emplace_back(rw, rh);
	mesh->vTexCoords.emplace_back(rw, 0.0);


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



void IndexMesh::draw() const
{
	glDrawElements(mPrimitive, vIndices.size(), GL_UNSIGNED_INT, vIndices.data());
}

void IndexMesh::render() const
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
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_DOUBLE, 0, vTexCoords.data());
		}
		if (vIndices.size() > 0) {
			glEnableClientState(GL_INDEX_ARRAY);
			glIndexPointer(GL_UNSIGNED_INT, 0, vIndices.data());
		}
		if (vNormals.size() > 0)
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_DOUBLE, 0, vNormals.data());
		}


		draw();

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_DOUBLE);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_INDEX_ARRAY);
	}
}

IndexMesh* IndexMesh::generaAnilloCuadrado()
{
	vector<unsigned int> aux({ 0, 1, 2, 3, 4, 5, 6, 7, 0, 1 });
	IndexMesh* AnilloCuadrado = new IndexMesh(aux);
	AnilloCuadrado->mPrimitive = GL_TRIANGLE_STRIP;
	AnilloCuadrado->mNumVertices = 8;/*10*/
	AnilloCuadrado->vVertices.reserve(AnilloCuadrado->mNumVertices);
	AnilloCuadrado->vVertices.emplace_back(30.0, 30.0, 0.0);
	AnilloCuadrado->vVertices.emplace_back(10.0, 10.0, 0.0);
	AnilloCuadrado->vVertices.emplace_back(70.0, 30.0, 0.0);
	AnilloCuadrado->vVertices.emplace_back(90.0, 10.0, 0.0);
	AnilloCuadrado->vVertices.emplace_back(70.0, 70.0, 0.0);
	AnilloCuadrado->vVertices.emplace_back(90.0, 90.0, 0.0);
	AnilloCuadrado->vVertices.emplace_back(30.0, 70.0, 0.0);
	AnilloCuadrado->vVertices.emplace_back(10.0, 90.0, 0.0);
	//AnilloCuadrado->vVertices.emplace_back(30.0, 30.0, 0.0);
	//AnilloCuadrado->vVertices.emplace_back(10.0, 10.0, 0.0);

	AnilloCuadrado->vColors.reserve(8/*10*/);

	AnilloCuadrado->vColors.emplace_back(0.0, 0.0, 0.0, 1.0);
	AnilloCuadrado->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	AnilloCuadrado->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	AnilloCuadrado->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	AnilloCuadrado->vColors.emplace_back(1.0, 1.0, 0.0, 1.0);
	AnilloCuadrado->vColors.emplace_back(1.0, 0.0, 1.0, 1.0);
	AnilloCuadrado->vColors.emplace_back(0.0, 1.0, 1.0, 1.0);
	AnilloCuadrado->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	//AnilloCuadrado->vColors.emplace_back(0.0, 0.0, 0.0, 1.0);
	//AnilloCuadrado->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);

	AnilloCuadrado->SetNormalsVector({
		{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1} });

	return AnilloCuadrado;
}

IndexMesh* IndexMesh::generaIndexCuboConTapas(GLdouble l, glm::dvec4 color)
{
	GLdouble lm = l / 2;
	vector<unsigned int> indexes({
		0, 1, 2,
		2, 3, 0,

		1, 5, 6,
		6, 2, 1,

		7, 6, 5,
		5, 4, 7,

		4, 0, 3,
		3, 7, 4,

		4, 5, 1,
		1, 0, 4,

		3, 2, 6,
		6, 7, 3 });

	IndexMesh* m = new IndexMesh(indexes);
	m->mPrimitive = GL_TRIANGLES;
	m->mNumVertices = 8;
	m->vVertices.reserve(8);
	m->vVertices.emplace_back(-lm, -lm, lm);
	m->vVertices.emplace_back(lm, -lm, lm);
	m->vVertices.emplace_back(lm, lm, lm);
	m->vVertices.emplace_back(-lm, lm, lm);

	m->vVertices.emplace_back(-lm, -lm, -lm);
	m->vVertices.emplace_back(lm, -lm, -lm);
	m->vVertices.emplace_back(lm, lm, -lm);
	m->vVertices.emplace_back(-lm, lm, -lm);

	m->vColors.reserve(8);
	for (size_t i = 0; i < m->vColors.capacity(); i++)
	{
		m->vColors.emplace_back(color);
	}
	m->buildNormalVectors();
	return m;
}

void IndexMesh::buildNormalVectors()
{
	vNormals.reserve(mNumVertices);
	for (int i = 0; i < vNormals.capacity(); i++)
	{
		vNormals.emplace_back(0, 0, 0);
	}
	for (int i = 0; i < vIndices.size() / 3; i++)
	{
		glm::dvec3 a = vVertices[vIndices[3 * i]];
		glm::dvec3 b = vVertices[vIndices[(3 * i) + 1]];		
		glm::dvec3 c = vVertices[vIndices[(3 * i) + 2]];
		glm::dvec3 n = normalize(cross(b - a, c - a));
		vNormals[vIndices[3 * i]] += n;
		vNormals[vIndices[(3 * i) + 1]] += n;
		vNormals[vIndices[(3 * i) + 2]] += n;
	}
	for (glm::dvec3 m : vNormals) {
		normalize(m);
	}
}

MbR* MbR::generaIndexMeshByRevolution(int mm, int nn, std::vector<glm::dvec3> perfil)
{
	MbR* m = new MbR(mm, nn, perfil);
	m->mPrimitive = GL_TRIANGLES;
	m->mNumVertices = mm * nn;
	std::vector<dvec3>vertices;
	vertices.reserve(m->mNumVertices);
	for (int i = 0; i < nn; i++) {
		GLdouble theta = i * 360 / nn;
		GLdouble c = cos(radians(theta));
		GLdouble s = sin(radians(theta));
		for (int j = 0; j < mm; j++) {
			int indice = i * mm + j;
			GLdouble x = c * perfil[j].x + s * perfil[j].z;
			GLdouble z = -s * perfil[j].x + c * perfil[j].z;
			vertices.push_back(dvec3(x, perfil[j].y, z));
		}
	}
	for (glm::dvec3 vet : vertices) {
		m->vVertices.emplace_back(vet);
	}
	for (int i = 0; i < nn; i++) {
		for (int j = 0; j < mm - 1; j++)
		{
			int indice = i * mm + j;
			m->vIndices.push_back(indice);
			int indice2 = (indice + mm) % (nn * mm);
			m->vIndices.push_back(indice2);
			int indice3 = (indice + mm + 1) % (nn * mm);
			m->vIndices.push_back(indice3);
			int indice4 = indice + 1;
			m->vIndices.push_back(indice);
			m->vIndices.push_back(indice3);
			m->vIndices.push_back(indice4);


		}
	}
	m->buildNormalVectors();
	return m;
}
