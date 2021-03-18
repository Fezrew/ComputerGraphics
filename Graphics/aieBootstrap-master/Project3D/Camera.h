#pragma once
#include <glm/ext.hpp>

using namespace glm;


class Camera
{
public:
	mat4 getViewMatrix();
	mat4 getProjectionMatrix(float w, float h);
	vec3 getCamForward() { return m_forward; }

	void Update(float deltaTime);

private:
	float m_theta = 0;
	float m_phi = 0;
	float m_lastMouseX = 0;
	float m_lastMouseY = 0;
	vec3 m_position = { -10, 1, 0 };
	vec3 m_forward = {0,0,0};
};

