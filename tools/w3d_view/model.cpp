#include "model.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
std::map<std::string, std::shared_ptr<Texture>> CompiledModel::s_textures;

CompiledModel::CompiledModel()
{

}

CompiledModel::~CompiledModel()
{
	for (auto& mesh : m_meshes)
	{
		glDeleteBuffers(1, &mesh.vbo);
		glDeleteBuffers(1, &mesh.ibo);
	}
}

void CompiledModel::Create(libw3d::Model& m)
{
	//compile all meshes
	if (m.Skeleton)
	{
		for (auto& pivot : m.Skeleton->Bones)
		{
			Pivot p;
			p.parent = pivot.ParentIdx;
			p.eulerAngles = glm::vec3(pivot.EulerAngles.X,
				pivot.EulerAngles.Y, pivot.EulerAngles.Z);
			p.translate = glm::vec3(pivot.Translation.X,
				pivot.Translation.Y, pivot.Translation.Z);
			p.rotation.x = pivot.Rotation.X;
			p.rotation.y = pivot.Rotation.Y;
			p.rotation.z = pivot.Rotation.Z;
			p.rotation.w = pivot.Rotation.W;
			m_pivots.push_back(p);
		}
	}

	for (auto& mesh : m.Meshes)
	{
		Mesh compiled;
		compiled.pivot = -1;
		std::vector<Vertex> verts;

		for (auto& vert : mesh->Vertices)
		{
			Vertex v;
			v.position = glm::vec3(vert.X, vert.Y, vert.Z);
			verts.push_back(v);
		}

		for (unsigned int i = 0; i < mesh->Normals.size(); ++i)
		{
			auto& normal = mesh->Normals[i];
			verts[i].normal = glm::vec3(normal.X, normal.Y, normal.Z);
		}

		for (unsigned int i = 0; i < mesh->VertexInfluences.size(); ++i)
		{
			auto& infl = mesh->VertexInfluences[i];
			verts[i].boneId = infl.Bone;
			verts[i].boneId2 = infl.Bone2;
		}

		if (mesh->MatPass)
		{
			for (unsigned int i=0;i< mesh->MatPass->Texcoords.size();++i)
			{
				auto& tx = mesh->MatPass->Texcoords[i];
				verts[i].txcoord = glm::vec2(tx.x,tx.y);
			}
		}

		if (mesh->TextureMaps)
		{
			for (auto& tex : mesh->TextureMaps->TextureMaps)
			{
				for(auto& entry : tex->Entries)
				{
					if (entry.TypeFlag == libw3d::NORMTYPE_TEXTURE)
					{
						auto handle = std::make_shared<Texture>();
						if (!handle->Load(entry.ItemName))
							continue;
							
						if(entry.TypeName=="DiffuseTexture")
							compiled.diffuse = handle;
					}
				}
			}
		}


		std::vector<glm::uint16_t> indices;
		for (auto& tri : mesh->Triangles)
		{
			indices.push_back(tri.Indices[0]);
			indices.push_back(tri.Indices[1]);
			indices.push_back(tri.Indices[2]);
		}


		if (m.HierarchyLoD)
		{
			if (m.HierarchyLoD->SubObjectArray)
			{
				for (auto& so : m.HierarchyLoD->SubObjectArray->SubOjects)
				{
					std::string name = std::string(mesh->Header.ContainerName) + "." + std::string(mesh->Header.MeshName);
					if (std::string(so.Name) == name)
					{
						compiled.pivot = so.BoneIndex;
					}
				}
			}
		}

		//compile that shit
		glGenBuffers(1, &compiled.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, compiled.vbo);
		glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), verts.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CompiledModel::Vertex), (void*)offsetof(CompiledModel::Vertex, position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(CompiledModel::Vertex), (void*)offsetof(CompiledModel::Vertex, normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(CompiledModel::Vertex), (void*)offsetof(CompiledModel::Vertex, txcoord));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 1, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(CompiledModel::Vertex), (void*)offsetof(CompiledModel::Vertex, boneId));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 1, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(CompiledModel::Vertex), (void*)offsetof(CompiledModel::Vertex, boneId2));
		glGenBuffers(1, &compiled.ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, compiled.ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::uint16), indices.data(), GL_STATIC_DRAW);
		compiled.num = indices.size();
		m_meshes.push_back(compiled);
	}

}

void CompiledModel::Render(Shader& s)
{
	
	for (auto& mesh : m_meshes)
	{
		glBindBuffer(GL_ARRAY_BUFFER,mesh.vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
		if (mesh.diffuse)
		{
			glActiveTexture(GL_TEXTURE0);
			glUniform1i(s.uniform("has_diffuse"), true);
			glUniform1i(s.uniform("diffuse"), 0);
			mesh.diffuse->Bind();
		}

		glm::mat4 model;

		if (mesh.pivot != -1)
		{
			int index = mesh.pivot;

			while (index > 0)
			{
				auto p = m_pivots[index];				
				glm::mat4 rotMat = glm::mat4_cast(p.rotation);
				model *= rotMat;
				model = glm::translate(model, p.translate);
				index = p.parent;
			}
		}

		glUniformMatrix4fv(s.uniform("m"), 1, false, glm::value_ptr(model));
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CompiledModel::Vertex), (void*)offsetof(CompiledModel::Vertex, position));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(CompiledModel::Vertex), (void*)offsetof(CompiledModel::Vertex, normal));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(CompiledModel::Vertex), (void*)offsetof(CompiledModel::Vertex, txcoord));
		glVertexAttribPointer(3, 1, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(CompiledModel::Vertex), (void*)offsetof(CompiledModel::Vertex, boneId));
		glVertexAttribPointer(4, 1, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(CompiledModel::Vertex), (void*)offsetof(CompiledModel::Vertex, boneId2));

		glDrawElements(GL_TRIANGLES, mesh.num, GL_UNSIGNED_SHORT, nullptr);
	}
}
