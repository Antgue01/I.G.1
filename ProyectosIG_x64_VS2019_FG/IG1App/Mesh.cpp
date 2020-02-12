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
Mesh* Mesh::generaPoligono(GLuint numl, GLdouble rd, bool fill) {
	Mesh* mesh = new Mesh();
	if (!fill)
		mesh->mPrimitive = GL_LINE_LOOP;
	else mesh->mPrimitive = GL_TRIANGLES;
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
	Mesh* toraianguru = generaPoligono(3, rd, false);
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
Mesh* Mesh::generaTrianguloRGB(GLdouble rd) {
	Mesh* triang = generaPoligono(3, rd, true);
	vec4 r = { 255,0,0,255 };
	vec4 g = { 0,255,0,255 };
	vec4 b = { 0,0,255,255 };
	triang->vColors.emplace_back(r);
	triang->vColors.emplace_back(g);
	triang->vColors.emplace_back(b);

	return triang;
}
Mesh* Mesh::generaRectangulo(GLdouble w, GLdouble h) {
	Mesh* rectangle = new Mesh();
	rectangle->mPrimitive = GL_TRIANGLE_STRIP;
	rectangle->mNumVertices = 4;
	rectangle->vVertices.reserve(rectangle->mNumVertices);
	vec3 punt = { -w / 2,-h / 2,0 };
	rectangle->vVertices.emplace_back(punt);
	punt = { punt.x,punt.y + h,0 };
	rectangle->vVertices.emplace_back(punt);
	punt = { punt.x + w,punt.y - h,0 };
	rectangle->vVertices.emplace_back(punt);
	punt = { punt.x,punt.y + h,0 };

	rectangle->vVertices.emplace_back(punt);
	return rectangle;
}
Mesh* Mesh::generaRectanguloRGB(GLdouble w, GLdouble h)
{
	Mesh* rect = generaRectangulo(w, h);
	vec4 r = { 255,0,0,255 };
	vec4 g = { 0,255,0,255 };
	vec4 b = { 0,0,255,255 };
	vec4 wh = { 255,255,255,255 };

	rect->vColors.push_back(r);
	rect->vColors.push_back(g);
	rect->vColors.push_back(b);
	rect->vColors.push_back(wh);
	return rect;
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
Mesh* Mesh::generaEstrella3D(GLdouble re, GLdouble np, GLdouble h) {

	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_TRIANGLE_FAN;
	mesh->mNumVertices = 2 * np + 2;
	mesh->vVertices.reserve(mesh->mNumVertices);
	mesh->vVertices.emplace_back(0, 0, h);
	mesh->vVertices.emplace_back(0, re, 0);
	GLdouble offset = radians(90.0);
	for (int i = 0; i < mesh->mNumVertices - 2; i++)
	{
		offset += radians(180.0 / np);

		if (i % 2 == 0)
		{//interior;

			mesh->vVertices.emplace_back((re / 2) * cos(offset), (re / 2) * sin(offset), 0);
		}
		else {
			//exterior;
			mesh->vVertices.emplace_back(re * cos(offset), re * sin(offset), 0);

		}
	}
	return mesh;

}
Mesh* Mesh::generaContCubo(GLdouble ld) {

	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 10;
	mesh->vVertices.reserve(mesh->mNumVertices);
	mesh->vVertices.emplace_back(0, 0, 0);
	for (int i = 0; i < mesh->mNumVertices - 1; i++)
	{
		if (i % 2 == 0)
			mesh->vVertices.emplace_back(mesh->vVertices.at(i - 1).x + ld, mesh->vVertices.at(i - 2).y - ld, 0);
		else
			mesh->vVertices.emplace_back(mesh->vVertices.at(i - 2).x + ld, mesh->vVertices.at(i - 2).y, 0);

	}
	return mesh;
}
//-------------------------------------------------------------------------

