//#pragma once
#ifndef _H_Texture_H_
#define _H_Texture_H_

//#include <GL/freeglut.h>
//#include <glm.hpp>
#include "GLInclude.h"
#include "Pixmap32RGBA.h"
#include <string>
//-------------------------------------------------------------------------
class IG1App;
class Texture
{
public:
	Texture() {};
	~Texture();

	Texture(const Texture& tex) = delete;  // no copy constructor
	Texture& operator=(const Texture& tex) = delete;  // no copy assignment

	void load(const std::string& BMP_Name, GLubyte alpha = 255); // load from file and upload to GPU
	static void save(const std::string& BMP_Name);
	void load(const std::string& BMP_Name, glm::u8vec3 color, GLubyte alpha = 0);
	void loadColorBuffer(int w, int h);
	//EXTRA 2
	void bind(GLuint mixMode, GLenum unit = GL_TEXTURE0);   // GL_REPLACE, GL_MODULATE, GL_ADD, ...
	//EXTRA 2
	void unbind(GLenum unit = GL_TEXTURE0) const;
	GLuint width() const { return mWidth; };
	GLuint height() const { return mHeight; };

	void setWrap(GLuint wp);  // GL_REPEAT, GL_CLAMP 

protected:

	void init();

	GLuint mWidth = 0;
	GLuint mHeight = 0;
	GLuint mId = 0;

};

//-------------------------------------------------------------------------

#endif //_H_Texture_H_

