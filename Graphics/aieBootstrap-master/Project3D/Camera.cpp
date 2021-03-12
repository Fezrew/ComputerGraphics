#include "Camera.h"
#include <Input.h>

using namespace aie;

mat4 Camera::getViewMatrix()
{
	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);
	vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));

	return lookAt(m_position, m_position + forward, glm::vec3(0, 1, 0));
}

mat4 Camera::getProjectionMatrix(float w, float h)
{
	return glm::perspective(glm::pi<float>() * 0.25f,
		w / h,
		0.1f, 1000.f);
}

void Camera::Update(float deltaTime)
{
	Input* input = Input::getInstance();
	float thetaR = radians(m_theta);
	float phiR = radians(m_phi);

	//calculate the forwards and right axes and up axis for the camera
	vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
	vec3 right(-sin(thetaR), 0, cos(thetaR));
	vec3 up(0, 1, 0);

	// use WASD, QE keys to move camera around
	if (input->isKeyDown(INPUT_KEY_E))
	{
		m_position += up * deltaTime;
	}
	if (input->isKeyDown(INPUT_KEY_Q))
	{
		m_position -= up * deltaTime;
	}

	if (input->isKeyDown(INPUT_KEY_W))
	{
		m_position += forward * deltaTime;
	}
	if (input->isKeyDown(INPUT_KEY_S))
	{
		m_position -= forward * deltaTime;
	}
	if (input->isKeyDown(INPUT_KEY_A))
	{
		m_position -= right * deltaTime;
	}
	if (input->isKeyDown(INPUT_KEY_D))
	{
		m_position += right * deltaTime;
	}

	// get the current mouse coordinates
	float mx = input->getMouseX();
	float my = input->getMouseY();
	const float turnSpeed = 0.1f;

	// if the right button is down, increment theta and phi
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		m_theta += turnSpeed * (mx - m_lastMouseX);
		m_phi += turnSpeed * (my - m_lastMouseY);
	}

	// store this frames values for next frame
	m_lastMouseX = mx;
	m_lastMouseY = my;

}