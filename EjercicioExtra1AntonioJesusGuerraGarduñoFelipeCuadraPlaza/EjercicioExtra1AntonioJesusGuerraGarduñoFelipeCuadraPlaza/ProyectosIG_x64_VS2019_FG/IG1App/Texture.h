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

	//void bind(GLuint mixMode);   // GL_REPLACE, GL_MODULATE, GL_ADD, ...

	//Hemos cambiado la signatura con respecto al enunciado para poder poner por defecto la unidad de textura 0 y así evitarnos pasarla en todas
	//las llamadas del proyecto
	void bind(GLuint mixMode, GLenum unit = GL_TEXTURE0);   //EXTRA 2 

	//void unbind() const { glBindTexture(GL_TEXTURE_2D, 0); };
	void unbind(GLenum unit = GL_TEXTURE0) const; //EXTRA 2

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

