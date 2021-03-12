#pragma once
#include "Application.h"
#include <glm/mat4x4.hpp>
#include "Mesh.h"
#include "Shader.h"
#include "OBJMesh.h"
#include "Camera.h"

using namespace aie;

class Application3D : public aie::Application {
public:

	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	//mat4	m_viewMatrix;
	//mat4	m_projectionMatrix;

	ShaderProgram m_shader;
	OBJMesh m_snakeMesh;
	mat4 m_snakeTransform;

	struct Light
	{
		vec3 direction;
		vec3 colour;
	};

	Light m_light;
	vec3 m_ambientLight;
	Camera m_camera;
};