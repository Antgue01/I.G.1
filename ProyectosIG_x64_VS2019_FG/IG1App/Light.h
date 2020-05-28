#pragma once
#include <glm.hpp>
#include <GL/freeglut.h>
#include <gtc/type_ptr.hpp>

class Light
{
public:
	Light();
	virtual ~Light() { disable(); }
	void uploadL() const;
	// Método abstracto
	virtual void upload(glm::dmat4 const& modelViewMat) const = 0;
	inline void disable() { if (id < GL_LIGHT0 + GL_MAX_LIGHTS) glDisable(id); };
	inline void enable() { if (id < GL_LIGHT0 + GL_MAX_LIGHTS) glEnable(id); };
	inline void setAmb(glm::fvec4 amb) { ambient = amb; uploadL(); }
	inline void setSpec(glm::fvec4 spec) { specular = spec; uploadL(); }
	inline void setDiff(glm::fvec4 diff) { diffuse = diff; uploadL(); }
protected:

	static GLuint cont; // Atributo para poder generar un nuevo id cada vez
	GLuint id = GL_LIGHT0 + GL_MAX_LIGHTS; // Primer id no válido
	// Atributos lumínicos y geométrico de la fuente de luz
	glm::fvec4 ambient = { 0.1, 0.1, 0.1, 1 };
	glm::fvec4 diffuse = { 0.5, 0.5, 0.5, 1 };
	glm::fvec4 specular = { 0.5, 0.5, 0.5, 1 };
	glm::fvec4 posDir = { 0, 0, 1, 0 };
	// Añade setter’s para cambiar el valor de los atributos lumínicos

};
class DirLight :public Light {
public:
	DirLight(glm::fvec3 dir = glm::fvec3(0, 0, 1)) :Light() {
		setPosDir(dir);
	}
	virtual void upload(glm::dmat4 const& modelViewMat) const;
	inline void setPosDir(glm::fvec3 dir) {
		posDir = glm::fvec4(dir, 0.0);
	}
};
class PosLight : public Light {
protected:
	// Factores de atenuación
	GLfloat kc = 1, kl = 0, kq = 0;
public:
	PosLight(glm::fvec3 pos = { 0, 0, 0 }) :Light() {
		setPosDir(pos);
	}
	virtual void upload(glm::dmat4 const& modelViewMat) const;
	inline void setPosDir(glm::fvec3 dir) { posDir = glm::fvec4(dir, 1); }
	void setAtte(GLfloat Kc, GLfloat Kl, GLfloat Kq);
};
class SpotLight : public PosLight {
protected:
	// Atributos del foco
	glm::fvec4 direction = { 0, 0, -1, 0 };
	GLfloat cutoff = 180;
	GLfloat exp = 0;
public:
	SpotLight(glm::fvec3 pos = { 0, 0, 0 })
		: PosLight(pos) {
	};
	virtual void upload(glm::dmat4 const& modelViewMat) const;
	void setSpot(glm::fvec3 dir, GLfloat cf, GLfloat e);
};

