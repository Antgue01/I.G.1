//#pragma once
#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include "Camera.h"
#include "Entity.h"

#include <vector>
#include "Light.h"


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
	inline DirLight* getDirectionalLight() { return directionalLight; }
	inline PosLight* getPositionalLight() { return positionalLight; }
	inline SpotLight* getSpotLight() { return spotSceneLight; }
	inline SpotLight* getPlaneLight() { return planeSpotLight; }
	inline SpotLight* getGridLight() { return gridSpotLight; }
	inline SpotLight* getSirenLight() { return SirenSpotLight; }
	inline PosLight* getLuzMinero() { return luzMinero; }
	inline CompoundEntity* getPlane() { return avion; }
	void scenePosLight(Camera const& cam)const;
	void sceneSpotLight(Camera const& cam)const;
	


protected:
	void free();
	void setGL();
	void resetGL();
	int miId = 5;
	bool LightsActivated = true;
	bool PositionalLightActivated = false;
	bool SpotLightActivated = false;
	bool lightsOFF = false;
	DirLight* directionalLight = nullptr;
	PosLight* positionalLight = nullptr;
	SpotLight* spotSceneLight = nullptr;
	SpotLight* planeSpotLight = nullptr;
	SpotLight* gridSpotLight = nullptr;
	SpotLight* SirenSpotLight = nullptr;
	PosLight* luzMinero = nullptr;
	bool lightsAreInitialized = false;

	void setLights();
	Plane* avion = nullptr;


	std::vector<Abs_Entity*> gObjects;  // Entities (graphic objects) of the scene
	std::vector<Texture*>gTextures;

};
//-------------------------------------------------------------------------

#endif //_H_Scene_H_

