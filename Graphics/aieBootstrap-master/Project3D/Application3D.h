#pragma once
#include "Application.h"
#include "Mesh.h"
#include "Shader.h"
#include "OBJMesh.h"
#include "Scene.h"

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

	mat4 m_viewMatrix;
	mat4 m_projectionMatrix;

	Texture m_gridTexture;

	ShaderProgram m_shader;
	ShaderProgram m_texturedShader;
	ShaderProgram m_normalMapShader;

	OBJMesh m_snakeMesh;
	OBJMesh m_spearMesh;
	Mesh m_quadMesh;
	mat4 m_quadTransform;

	Camera m_camera;

	Scene* m_scene;
};