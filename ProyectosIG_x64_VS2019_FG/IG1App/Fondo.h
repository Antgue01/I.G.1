#pragma once
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"
class Fondo
{
public:
	Fondo();
	~Fondo();
	void setSize(int xw, int yh);
	void render() const;
	inline Camera* getCamera() { return cam; }
private:
	Camera* cam;
	Mesh* mesh;
	Texture* img;
	Viewport* viewport;
};

