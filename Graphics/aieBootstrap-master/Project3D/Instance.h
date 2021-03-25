#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Camera.h"
//#include "Scene.h"

using namespace glm;

struct Light;
class Scene;

namespace aie
{
	class ShaderProgram;
	class OBJMesh;

	class Instance
	{
	public:
		Instance(OBJMesh* mesh, mat4 transform, ShaderProgram* shader);
		void draw(Scene* scene);

	protected:
		mat4 m_transform;
		OBJMesh* m_mesh;
		ShaderProgram* m_shader = nullptr;
	};
}

