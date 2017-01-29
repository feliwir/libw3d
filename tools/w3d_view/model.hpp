#pragma once
#include <libw3d/w3d.hpp>
#include <glm/glm.hpp>
#include "flextGL.h"
#include <gl/GL.h>
#include "shader.hpp"
#include "texture.hpp"
class CompiledModel
{
public:
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 txcoord;
		glm::uint16 boneId;
		glm::uint16 boneId2;
	};

	struct Mesh
	{
		//vertexbuffer
		GLuint vbo;
		//indexbuffer
		GLuint ibo;
		uint32_t num;
		std::shared_ptr<Texture> diffuse;
	};
public:
	CompiledModel();
	~CompiledModel();

	void Create(libw3d::Model& m);
	void Render(Shader& s);
private:
	std::vector<Mesh> m_meshes;
	static std::map < std::string, std::shared_ptr<Texture>> s_textures;
};