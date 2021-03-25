#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <list>
#include "Instance.h"

using namespace std;
using namespace glm;
using namespace aie;

struct Light
{
	vec3 direction;
	vec3 colour;
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

	Light m_light;

protected:
	Camera* m_camera;
	vec2 m_windowSize;

	vec3 m_ambientLight;
	list<Instance*> m_instances;
};

