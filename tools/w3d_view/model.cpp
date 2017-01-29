#include "model.hpp"()

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
	for (auto& mesh : m.Meshes)
	{
		std::vector<Vertex> verts;
		for (auto& vert : mesh->Vertices)
		{
			Vertex v;
			v.position = glm::vec3(vert.X, vert.Y, vert.Z);
			verts.push_back(v);
		}

		for (int i = 0; i < mesh->Normals.size(); ++i)
		{
			auto& normal = mesh->Normals[i];
			verts[i].normal = glm::vec3(normal.X, normal.Y, normal.Z);
		}

		for (int i = 0; i < mesh->VertexInfluences.size(); ++i)
		{
			auto& infl = mesh->VertexInfluences[i];
			verts[i].boneId = infl.Bone;
			verts[i].boneId2 = infl.Bone2;
		}

		std::vector<glm::uint16_t> indices;
		for (auto& tri : mesh->Triangles)
		{
			indices.push_back(tri.Indices[0]);
			indices.push_back(tri.Indices[1]);
			indices.push_back(tri.Indices[2]);
		}

		//compile that shit
		Mesh compiled;
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

void CompiledModel::Render()
{
	for (auto& mesh : m_meshes)
	{
		glBindBuffer(GL_ARRAY_BUFFER,mesh.vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
		glDrawElements(GL_TRIANGLES, mesh.num, GL_UNSIGNED_SHORT, nullptr);
	}
}
