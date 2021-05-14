#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <imgui.h>


using namespace aie;
using namespace glm;

Application3D::Application3D() 
{

}

Application3D::~Application3D() {

}

bool Application3D::startup() {

	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

#pragma region Shader/Texture Loading
	m_shader.loadShader(eShaderStage::VERTEX, "./shaders/simple.vert");
	m_shader.loadShader(eShaderStage::FRAGMENT, "./shaders/simple.frag");
	m_shader.loadShader(eShaderStage::VERTEX, "./shaders/phong.vert");
	m_shader.loadShader(eShaderStage::FRAGMENT, "./shaders/phong.frag");
	if (m_shader.link() == false)
	{
		printf("Shader Error: %s\n", m_shader.getLastError());
		return false;
	}

	m_texturedShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/textured.vert");
	m_texturedShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/textured.frag");
	if (m_texturedShader.link() == false) 
	{
		printf("Shader Error: %s\n", m_texturedShader.getLastError());
		return false;
	}

	m_normalMapShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/normalmap.vert");
	m_normalMapShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/normalmap.frag");
	if (m_normalMapShader.link() == false)
	{
		printf("Shader Error: %s\n", m_normalMapShader.getLastError());
		return false;
	}

	if (m_gridTexture.load("./textures/numbered_grid.tga") == false) {
		printf("Failed to load texture!\n");
		return false;
	}
#pragma endregion

	Light light;
	light.colour = { 1, 1, 1 };
	light.direction = vec3(1, -1, 1);

	m_scene = new Scene(&m_camera, glm::vec2(getWindowWidth(), getWindowHeight()), light, glm::vec3(0.25f, 0.25f, 0.25f));

	// red light on the left
	m_scene->getPointLights().push_back(Light(vec3(5, 3, 0), vec3(1, 0, 0), 50));
	// green light on the right
	m_scene->getPointLights().push_back(Light(vec3(-5, 3, 0), vec3(0, 1, 0), 50));

#pragma region Creating models
	if (m_snakeMesh.load("./stanford/Snake/Snake.obj", true, true) == false)
	{
		printf("Snake Mesh Error!\n");
		return false;
	}

	mat4 m_snakeTransform =
	{
		0.15f,0,0,0,
		0,0.15f,0,0,
		0,0,0.15f,0,
		0,0,0,1
	};

	m_scene->addInstance(new Instance(&m_snakeMesh, m_snakeTransform, &m_texturedShader));

	if (m_spearMesh.load("./stanford/soulspear/soulspear.obj", true, true) == false)
	{
		printf("Spear Mesh Error!\n");
		return false;
	}

	mat4 spearTransform =
	{
		0.5,0,0,0,
		0,0.5f,0,0,
		0,0,0.5f,0,
		1,0.5f,3,1
	};

	m_scene->addInstance(new Instance(&m_spearMesh, spearTransform, &m_normalMapShader));

	// create a simple quad
	m_quadMesh.initialiseQuad();

	// define a scale matrix for the quad
	m_quadTransform = 
	{
	10,0,0,0,
	0,10,0,0,
	0,0,10,0,
	0,0,0,1
	};
#pragma endregion


	return true;
}

void Application3D::shutdown() 
{
	Gizmos::destroy();
	delete m_scene;
}

void Application3D::update(float deltaTime) 
{
	// query time since application started
	float time = getTime();

	ImGui::Begin("Light Settings");

	// Change the lights direction and colour manually
	ImGui::DragFloat3("Sunlight Direction", &m_scene->m_sunLight.direction[0], 0.1f, -1.0f, 1.0f);
	ImGui::DragFloat3("Sunlight Colour", &m_scene->m_sunLight.colour[0], 0.1f, 0.0f, 2.0f);
	ImGui::End();

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	//Update the camera's transform
	m_camera.Update(deltaTime);

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application3D::draw() 
{
	// wipe the screen to the background colour
	clearScreen();

	// bind transform
	mat4 projectionMatrix = m_camera.getProjectionMatrix(getWindowWidth(), (float)getWindowHeight());
	mat4 viewMatrix = m_camera.getViewMatrix();

	m_scene->draw();

	// draw 3D gizmos
	Gizmos::draw(projectionMatrix * viewMatrix);

	// draw 2D gizmos using an orthogonal projection matrix (or screen dimensions)
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());
}