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

	if (m_snakeMesh.load("./stanford/Snake/Snake.obj") == false)
	{
		printf("Snake Mesh Error!\n");
		return false;
	}

	m_snakeTransform =
	{
		0.3f,0,0,0,
		0,0.3f,0,0,
		0,0,0.3f,0,
		0,0,0,1
	};

	m_light.colour = { 1, 1, 1 };
	m_ambientLight = { 0.25f, 0.25f, 0.25f };

	return true;
}

void Application3D::shutdown() {

	Gizmos::destroy();
}

void Application3D::update(float deltaTime) {

	// query time since application started
	float time = getTime();

	// rotate camera & light
	//m_viewMatrix = glm::lookAt(vec3(glm::sin(time) * 10, 10, glm::cos(time) * 10), vec3(0), vec3(0, 1, 0));
	//m_light.direction = normalize(vec3(cos(time * 2), sin(time * 2), 0));

	// move light to camera [BROKEN]
	//m_light.direction = m_camera.getViewMatrix();

	ImGui::Begin("Light Settings");
	//ImGui::DragFloat3("Sunlight Direction", &m_light.direction[0], 0.1f, -1.0f, 1.0f);
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

void Application3D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// bind shader
	m_shader.bind();

	// bind light
	m_shader.bindUniform("AmbientColour", m_ambientLight);
	m_shader.bindUniform("LightColour", m_light.colour);
	m_shader.bindUniform("LightDirection", m_light.direction);

	// bind transform
	mat4 projectionMatrix = m_camera.getProjectionMatrix(getWindowWidth(), (float)getWindowHeight());
	mat4 viewMatrix = m_camera.getViewMatrix();

	auto pvm = projectionMatrix * viewMatrix * m_snakeTransform;
	m_shader.bindUniform("ProjectionViewModel", pvm);

	// bind transforms for lighting
	m_shader.bindUniform("ModelMatrix", m_snakeTransform);

	// draw mesh
	m_snakeMesh.draw();

	// draw 3D gizmos
	//Gizmos::draw(m_projectionMatrix * m_viewMatrix);

	// draw 2D gizmos using an orthogonal projection matrix (or screen dimensions)
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());
}