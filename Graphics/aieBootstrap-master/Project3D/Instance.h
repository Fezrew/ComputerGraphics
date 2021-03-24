#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Mesh.h"
#include "Shader.h"
#include "OBJMesh.h"

using namespace aie;
using namespace glm;

class Instance
{
protected:
	glm::mat4 m_transform;
	aie::OBJMesh* m_mesh;
	aie::ShaderProgram* m_shader;
};

