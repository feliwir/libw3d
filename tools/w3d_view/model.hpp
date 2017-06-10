#pragma once
#include <libw3d/w3d.hpp>
#include <glm/glm.hpp>
#include "flextGL.h"
#include "shader.hpp"
#include "texture.hpp"

namespace w3dview
{
	class CompiledModel
	{
	public:
		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 txcoord;
			glm::ivec2 bones;
		};

		struct Pivot
		{
			int32_t parent;
			glm::vec3 translate;
			glm::vec3 eulerAngles;
			glm::quat rotation;
		};

		struct Mesh
		{
			//vertexbuffer
			GLuint vbo;
			//indexbuffer
			GLuint ibo;
			uint32_t num;
			std::shared_ptr<Texture> diffuse;
			int32_t pivot;
			bool skinned;
			Mesh();
		};
	public:
		CompiledModel();
		~CompiledModel();

		void Create(libw3d::Model& m,const std::string& basepath=".");
		void Render(Shader& s);
	private:
		std::vector<Mesh> m_meshes;
		static std::map < std::string, std::shared_ptr<Texture>> s_textures;
		std::vector<Pivot> m_pivots;
		std::vector<std::shared_ptr<libw3d::Animation>> m_animations;
		//precomputed bones for the shader
		std::vector<glm::mat4> m_bones;
		std::vector<glm::mat4> m_frame_bones;

		int frame;
	private:
		void ComputePose();
	};
}