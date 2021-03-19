#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <imgui.h>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;
using namespace aie;
using namespace glm;

Application3D::Application3D() {

}

Application3D::~Application3D() {

}

bool Application3D::startup() {

	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

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
	m_texturedShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/normalmap.vert");
	m_texturedShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/normalmap.frag");
	if (m_texturedShader.link() == false) 
	{
		printf("Shader Error: %s\n", m_texturedShader.getLastError());
		return false;
	}
	if (m_gridTexture.load("./textures/numbered_grid.tga") == false) {
		printf("Failed to load texture!\n");
		return false;
	}

	if (m_snakeMesh.load("./stanford/Snake/Snake.obj", true, true) == false)
	{
		printf("Snake Mesh Error!\n");
		return false;
	}

	m_snakeTransform =
	{
		0.15f,0,0,0,
		0,0.15f,0,0,
		0,0,0.15f,0,
		0,0,0,1
	};

	if (m_spearMesh.load("./stanford/soulspear/soulspear.obj", true, true) == false)
	{
		printf("Spear Mesh Error!\n");
		return false;
	}

	m_spearTransform =
	{
		0.5,0,0,0,
		0,0.5f,0,0,
		0,0,0.5f,0,
		1,0.5f,3,1
	};

	m_light.colour = { 1, 1, 1 };
	m_ambientLight = { 0.25f, 0.25f, 0.25f };

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


	return true;
}

void Application3D::shutdown() 
{

	Gizmos::destroy();
}

void Application3D::update(float deltaTime) 
{
	// query time since application started
	float time = getTime();

	// rotate camera & light
	//m_viewMatrix = glm::lookAt(vec3(glm::sin(time) * 10, 10, glm::cos(time) * 10), vec3(0), vec3(0, 1, 0));
	//m_light.direction = normalize(vec3(cos(time * 2), sin(time * 2), 0));

	// move light to camera
	//m_light.direction = m_camera.getCamForward();

	ImGui::Begin("Light Settings");

	// Change the lights direction and colour manually
	ImGui::DragFloat3("Sunlight Direction", &m_light.direction[0], 0.1f, -1.0f, 1.0f);
	ImGui::DragFloat3("Sunlight Colour", &m_light.colour[0], 0.1f, 0.0f, 2.0f);
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

	// bind shader
	//m_shader.bind();

	// bind shader
	m_texturedShader.bind();

	// bind light
	m_texturedShader.bindUniform("AmbientColour", m_ambientLight);
	m_texturedShader.bindUniform("LightColour", m_light.colour);
	m_texturedShader.bindUniform("LightDirection", m_light.direction);
	m_texturedShader.bindUniform("CameraPosition", viewMatrix);

	// bind transform
	auto pvm = projectionMatrix * viewMatrix * m_snakeTransform;
	m_texturedShader.bindUniform("ProjectionViewModel", pvm);
	m_texturedShader.bindUniform("ModelMatrix", m_snakeTransform);

	// bind texture location
	//m_texturedShader.bindUniform("diffuseTexture", 0);

	// draw mesh
	m_snakeMesh.draw();

	// bind transform
	pvm = projectionMatrix * viewMatrix * m_spearTransform;
	m_texturedShader.bindUniform("ProjectionViewModel", pvm);
	m_texturedShader.bindUniform("ModelMatrix", m_spearTransform);

	// draw mesh
	m_spearMesh.draw();

	// bind transform
	pvm = projectionMatrix * viewMatrix * m_quadTransform;
	m_texturedShader.bindUniform("ProjectionViewModel", pvm);
	m_texturedShader.bindUniform("ModelMatrix", m_quadTransform);

	// bind texture to specified location
	m_gridTexture.bind(0);

	// draw quad
	m_quadMesh.draw();

	// draw 3D gizmos
	Gizmos::draw(projectionMatrix * viewMatrix);

	// draw 2D gizmos using an orthogonal projection matrix (or screen dimensions)
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());
}