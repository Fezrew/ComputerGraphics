#include "Instance.h"
#include "Mesh.h"
#include "Shader.h"
#include "OBJMesh.h"
#include "Scene.h"

using namespace aie;
using namespace glm;

Instance::Instance(OBJMesh* mesh, mat4 transform, ShaderProgram* shader)
{
	m_mesh = mesh;
	m_transform = transform;
	m_shader = shader;
}

void Instance::draw(Scene* scene)
{
	// set the shader pipeline
	m_shader->bind();

	// bind transform and other uniforms
	auto pvm = scene->getCamera()->getProjectionMatrix(scene->getWindowSize().x, scene->getWindowSize().y) * scene->getCamera()->getViewMatrix() * m_transform;
	m_shader->bindUniform("ProjectionViewModel", pvm);

	m_shader->bindUniform("ModelMatrix", m_transform);
	m_shader->bindUniform("AmbientColour", scene->getAmbientLight());
	m_shader->bindUniform("LightColour", scene->m_light.colour);
	m_shader->bindUniform("LightDirection", scene->m_light.direction);
	m_shader->bindUniform("cameraPosition", scene->getCamera()->getPosition());

	// draw mesh
	m_mesh->draw();

}
