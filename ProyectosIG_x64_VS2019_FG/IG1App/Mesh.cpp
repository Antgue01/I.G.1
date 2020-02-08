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

		draw();

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}
Mesh* Mesh::generaPoligono(GLuint numl, GLdouble rd) {
	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_LINE_LOOP;
	mesh->mNumVertices = numl;
	mesh->vVertices.reserve(mesh->mNumVertices);
	mesh->vVertices.emplace_back(0.0, rd, 0.0);
	GLdouble offset = radians(90.0);
	for (GLuint i = 0; i < numl - 1; i++)
	{

		//mesh->vVertices.emplace_back(rd * cos(radians(360.0 / numl)),rd*sin(radians(360.0/numl)),0);
		offset += radians(360.0 / numl);
		mesh->vVertices.emplace_back(rd * cos(offset), rd * sin(offset), 0);

	}
	return mesh;
}
Mesh* Mesh::generaSierpinski(GLuint numP, GLdouble rd) {
	Mesh* toraianguru = generaPoligono(3, rd);
	Mesh* mesh = new Mesh();

	//Escribe tu código aquí...
	mesh->mPrimitive = GL_POINTS;
	mesh->mNumVertices = 3 * (numP + 1);
	mesh->vVertices.reserve(mesh->mNumVertices);
	vec3 verticeRandom = toraianguru->vVertices.at(rand() % 3);
	for (GLuint i = 0; i < mesh->mNumVertices - 1; i++)
	{
		int access = rand() % 3;
		vec3 pr2 = { toraianguru->vVertices[access].x,toraianguru->vVertices[access].y,toraianguru->vVertices[access].z };
		vec3 media = { (verticeRandom.x + pr2.x) / 2,(verticeRandom.y + pr2.y) / 2,(verticeRandom.z + pr2.z) / 2 };
		//verticeRandom = { (verticeRandom.x + toraianguru->vVertices[(access)].x) / 2,(verticeRandom.y + toraianguru->vVertices[access].y) / 2 ,
		//(verticeRandom.z + toraianguru->vVertices[access].z) / 2 };
		mesh->vVertices.emplace_back(media);
		verticeRandom = media;
	}
	delete toraianguru;
	toraianguru = nullptr;
	return mesh;

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

