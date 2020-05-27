//#pragma once
#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include "Camera.h"
#include "Entity.h"

#include <vector>

//-------------------------------------------------------------------------

class Scene
{
public:
	Scene() {};
	~Scene() { free(); resetGL(); };

	Scene(const Scene& s) = delete;  // no copy constructor
	Scene& operator=(const Scene& s) = delete;  // no copy assignment

	void init();

	void render(Camera const& cam) const;
	void update();
	int GetID() { return miId; }
	void setState(int id);

	void sceneDirLight(Camera const& cam) const;
	inline void setLightsActivated(bool value) { LightsActivated = value; }
	inline bool getLightsActivated() { return LightsActivated; }
	inline void setPositionalLightActivated(bool value) { PositionalLightActivated = value; }
	inline bool getPositionalLightActivated() { return PositionalLightActivated; }
	inline void setSpotLightActivated(bool value) { SpotLightActivated = value; }
	inline bool getSpotLightActivated() { return SpotLightActivated; }
	inline void switchLightsOnOff() { lightsOFF = !lightsOFF; }
	inline bool getLightsOff() { return lightsOFF; }
	void scenePosLight(Camera const& cam)const;
	void sceneSpotLight(Camera const& cam)const;

protected:
	void free();
	void setGL();
	void resetGL();
	int miId = 3;
	bool LightsActivated = true;
	bool PositionalLightActivated = false;
	bool SpotLightActivated = false;
	bool lightsOFF = false;

	std::vector<Abs_Entity*> gObjects;  // Entities (graphic objects) of the scene
	std::vector<Texture*>gTextures;

};
//-------------------------------------------------------------------------

#endif //_H_Scene_H_

