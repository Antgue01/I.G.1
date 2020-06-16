#pragma once
//toda la clase es el EXTRA 1
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"
class IG1App;
class Fondo
{
public:
	Fondo();
	~Fondo();
	void setSizeVP(int xw, int yh);
	void render() const;
	inline Camera* getCamera() { return cam; }
private:
	Camera* cam;
	Mesh* mesh;
	Texture* img;
	Texture* secondImg; //EXTRA 2
	Viewport* viewport;
};

