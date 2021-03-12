#pragma once
#include <glm/ext.hpp>

using namespace glm;


class Camera
{
public:
	mat4 getViewMatrix();
	mat4 getProjectionMatrix(float w, float h);

	void Update(float deltaTime);

private:
	float m_theta = 0;
	float m_phi = 0;
	float m_lastMouseX = 0;
	float m_lastMouseY = 0;
	vec3 m_position = { -10, 1, 0 };

};

