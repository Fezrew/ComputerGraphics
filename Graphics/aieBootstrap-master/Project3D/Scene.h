#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <list>
#include "Instance.h"
#include <vector>

using namespace std;
using namespace glm;
using namespace aie;

const int MAX_LIGHTS = 4;

struct Light
{
	vec3 direction;
	vec3 colour;
	Light() {}

	Light(glm::vec3 pos, glm::vec3 col, float intensity)
	{
		direction = pos;
		colour = col * intensity;
	}
};

class Scene
{
public:
	Scene(Camera* camera, vec2 windowSize, Light& light, vec3 ambientLight);
	~Scene();

	void addInstance(Instance* instance);
	void draw();

	Camera* getCamera() { return m_camera; }
	vec2 getWindowSize() { return m_windowSize; }
	vec3 getAmbientLight() { return m_ambientLight; }

	Light m_sunLight;

	int getNumLights() { return (int)m_pointLights.size(); }
	vec3* getPointlightPositions() { return &m_pointLightPositions[0]; }
	vec3* getPointlightColours() { return &m_pointLightColours[0]; }

	vector<Light>& getPointLights() { return m_pointLights; }

protected:
	Camera* m_camera;
	vec2 m_windowSize;
	//Light m_sunLight;
	vector<Light> m_pointLights;
	vec3 m_ambientLight;
	list<Instance*> m_instances;

	vec3 m_pointLightPositions[MAX_LIGHTS];
	vec3 m_pointLightColours[MAX_LIGHTS];
};

