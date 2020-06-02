//#pragma once
#ifndef _H_Mesh_H_
#define _H_Mesh_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include <vector>

//-------------------------------------------------------------------------

class Mesh
{
public:

	static Mesh* createRGBAxes(GLdouble l); // creates a new 3D-RGB axes mesh
	static Mesh* generaPoligono(GLuint numL, GLdouble rd);
	static Mesh* generaSierpinski(GLdouble rd, GLuint numP);
	static Mesh* generaTrianguloRGB(GLdouble rd);
	static Mesh* generaRectangulo(GLdouble w, GLdouble h);
	static Mesh* generaRectanguloRGB(GLdouble w, GLdouble h);
	static Mesh* generaEstrella3D(GLdouble re, GLdouble np, GLdouble h);
	static Mesh* generaContCubo(GLdouble ld);
	static Mesh* generaEstrellaTexCor(GLdouble re, GLuint np, GLdouble h);
	static Mesh* generaRectanguloTexCor(GLdouble w, GLdouble h, GLuint rw, GLuint rh);
	static Mesh* generaCajaTexCor(GLdouble nl);



	Mesh(/*std::vector<unsigned int> Indices = std::vector<unsigned int>()*/)/*:vIndices(Indices)*/ {};
	virtual ~Mesh() {};

	Mesh(const Mesh& m) = delete;  // no copy constructor
	Mesh& operator=(const Mesh& m) = delete;  // no copy assignment

	virtual void render() const;

	GLuint size() const { return mNumVertices; };   // number of elements
	std::vector<glm::dvec3> const& vertices() const { return vVertices; };
	std::vector<glm::dvec4> const& colors() const { return vColors; };
	std::vector<glm::dvec2> const& textCoords() const { return vTexCoords; };

	void SetNormalsVector(std::vector<glm::dvec3> NvNormals) { vNormals = NvNormals; }

protected:

	GLuint mPrimitive = GL_TRIANGLES;   // graphic primitive: GL_POINTS, GL_LINES, GL_TRIANGLES, ...
	GLuint mNumVertices = 0;  // number of elements ( = vVertices.size())
	std::vector<glm::dvec3> vVertices;  // vertex array
	std::vector<glm::dvec4> vColors;    // color array
	std::vector<glm::dvec2> vTexCoords;
	std::vector<glm::dvec3> vNormals;
	//std::vector<unsigned int> vIndices;
	virtual void draw() const;
};
class IndexMesh :public Mesh {
public:
	IndexMesh(std::vector<unsigned int> indices) :Mesh(), vIndices(indices) {}
	virtual void draw() const;
	virtual void render() const;
	static IndexMesh* generaAnilloCuadrado();
	static IndexMesh* generaIndexCuboConTapas(GLdouble l, glm::dvec4 color);
	static IndexMesh* generaGrid(GLdouble lado, GLuint nDiv);
	static IndexMesh* generateGridTex(GLdouble lado, GLuint nDiv);


	//std::vector<unsigned int>
protected:
	std::vector<unsigned int> vIndices;
	void buildNormalVectors();
};
class MbR :public IndexMesh {
public:
	MbR(int m, int n, std::vector<glm::dvec3> perfil) :IndexMesh(std::vector<unsigned int>()),
		m(m), n(n), perfil(perfil) {}
	static MbR* generaIndexMeshByRevolution(int mm, int nn, std::vector<glm::dvec3> perfil, glm::dvec4 color=
		glm::dvec4(0.12, 0.69, 0.6, 1));


private:
	int m;
	int n;
	std::vector<glm::dvec3> perfil;
	glm::dvec4 color;
};
//-------------------------------------------------------------------------

#endif //_H_Scene_H_