#include "Texture.h"
#include "IG1App.h"


//-------------------------------------------------------------------------

Texture::~Texture()
{
	if (mId != 0)
		glDeleteTextures(1, &mId);
}
//-------------------------------------------------------------------------

void Texture::init()
{
	glGenTextures(1, &mId);
	glBindTexture(GL_TEXTURE_2D, mId);

	//Filters and clamping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);     // GL_CLAMP
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);     // GL_CLAMP

}
//-------------------------------------------------------------------------

void Texture::bind(GLuint mixMode, GLenum Tunit ) // GL_REPLACE, GL_MODULATE, GL_ADD
{
	glEnable(GL_TEXTURE_2D);//EXTRA 2
	glActiveTexture(Tunit);//EXTRA 2
	glBindTexture(GL_TEXTURE_2D, mId);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mixMode);
}

void Texture::unbind(GLenum unit) const
{
	
	glActiveTexture(unit);//EXTRA 2
	glBindTexture(GL_TEXTURE_2D, 0);
}



//-------------------------------------------------------------------------

void Texture::load(const std::string& BMP_Name, GLubyte alpha)
{
	if (mId == 0) init();

	PixMap32RGBA pixMap;

	pixMap.load_bmp24BGR(BMP_Name);

	if (alpha != 255)
		pixMap.set_alpha(alpha);

	mWidth = pixMap.width();
	mHeight = pixMap.height();

	GLint level = 0;   //Base image level
	GLint border = 0;  //No border

	glBindTexture(GL_TEXTURE_2D, mId);
	glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, mWidth, mHeight, border, GL_RGBA, GL_UNSIGNED_BYTE, pixMap.data());

	glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture::load(const std::string& BMP_Name, glm::u8vec3 color, GLubyte alpha) {
	if (mId == 0) init();

	PixMap32RGBA pixMap;

	pixMap.load_bmp24BGR(BMP_Name);

	if (alpha != 255) {
		pixMap.set_colorkey_alpha(color, alpha);
	}

	mWidth = pixMap.width();
	mHeight = pixMap.height();

	GLint level = 0;   //Base image level
	GLint border = 0;  //No border

	glBindTexture(GL_TEXTURE_2D, mId);
	glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, mWidth, mHeight, border, GL_RGBA, GL_UNSIGNED_BYTE, pixMap.data());

	glBindTexture(GL_TEXTURE_2D, 0);
}
//-------------------------------------------------------------------------
void Texture::save(const std::string& NAME) {

	PixMap32RGBA e;
	e.reserve(IG1App::s_ig1app.winWidth(), IG1App::s_ig1app.winHeight());
	glReadBuffer(GL_FRONT);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, e.data());
	glReadPixels(0, 0, IG1App::s_ig1app.winWidth(), IG1App::s_ig1app.winHeight(), GL_RGBA, GL_UNSIGNED_BYTE, e.data());
	e.save_bmp24BGR(NAME);
	e.free();


}

void Texture::setWrap(GLuint wp) // GL_REPEAT, GL_CLAMP
{
	glBindTexture(GL_TEXTURE_2D, mId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wp);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture::loadColorBuffer(int w, int h) {


	init();

	mWidth = w;
	mHeight = h;
	GLint level = 0;   //Base image level
	GLint border = 0;  //No border

	glBindTexture(GL_TEXTURE_2D, mId);
	glReadBuffer(GL_FRONT);
	glCopyTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, 0, 0, w, h, border);
	glReadBuffer(GL_BACK);
	glBindTexture(GL_TEXTURE_2D, 0);

}

//-------------------------------------------------------------------------