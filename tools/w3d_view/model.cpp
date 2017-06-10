#include "model.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <stack>
#include <iostream>
using namespace w3dview;
std::map<std::string, std::shared_ptr<Texture>> CompiledModel::s_textures;


template<class T>
std::vector<T> getDataVector(std::vector<uint8_t> data)
{
	std::vector<T> result;
	auto num = data.size() / sizeof(T);
	for (int i = 0; i<num; ++i)
	{
		result.push_back(*(T*)&data[i * sizeof(T)]);
	}

	return result;
}

CompiledModel::CompiledModel() : m_currentAni(0)
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

void CompiledModel::Create(libw3d::Model& m,const std::string& basepath)
{
	m_pivots.clear();
	m_meshes.clear();
	m_animations.clear();

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
			p.rotation = glm::quat(pivot.Rotation.W, pivot.Rotation.X,
				pivot.Rotation.Y, pivot.Rotation.Z);
			m_pivots.push_back(p);
		}
	}

	for (auto& mesh : m.Meshes)
	{
		Mesh compiled;
		std::vector<Vertex> verts;

		if (mesh->Header.Attributes & W3D_MESH_FLAG_GEOMETRY_TYPE_SKIN)
			compiled.skinned = true;

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
			verts[i].bones.x = infl.Bone;
			verts[i].bones.y = infl.Bone2;
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
						auto name = entry.ItemName;
						std::transform(name.begin(),name.end(),name.begin(),::tolower);
						if (!handle->Load(basepath+"/"+ name))
							continue;
						
						s_textures[entry.ItemName] = handle;

						if(entry.TypeName=="DiffuseTexture")
							compiled.diffuse = handle;
					}
				}
			}
		}

		if (mesh->Textures)
		{
			for (auto& tex : mesh->Textures->Textures)
			{
				auto handle = std::make_shared<Texture>();
				auto name = tex->Name;
				std::transform(name.begin(),name.end(),name.begin(),::tolower);
				if (!handle->Load(basepath+"/"+ name))
					continue;

				s_textures[tex->Name] = handle;
				if(compiled.diffuse==nullptr)
					compiled.diffuse = handle;
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
		glVertexAttribIPointer(3, 2, GL_INT, sizeof(CompiledModel::Vertex), (void*)offsetof(CompiledModel::Vertex, bones));
		glGenBuffers(1, &compiled.ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, compiled.ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::uint16), indices.data(), GL_STATIC_DRAW);
		compiled.num = indices.size();
		m_meshes.push_back(compiled);
	}

	for (auto& ani : m.Animations)
	{
		Animation a;
		a.frame = 1;
		a.animation = ani;
		a.passed = 0;
		a.delta = 1000.0 / ani->Header.FrameRate;
		m_animations.push_back(a);
	}
 
	ComputePose();
	m_last = clock::now();
}

void CompiledModel::ComputePose()
{
	//precompute bones
	for (unsigned int i=0; i<m_pivots.size(); ++i)
	{
		auto p = m_pivots[i];
		
		std::vector<glm::mat4> bonestack;

		while (p.parent > -1)
		{
			glm::mat4 bone;
			bone = glm::translate(bone, p.translate);
			bone *= glm::mat4_cast(p.rotation);
			bonestack.push_back(bone);
			p = m_pivots[p.parent];
		}

		glm::mat4 bone;
		uint32_t size = bonestack.size();
		for (int i = 0; i <size; ++i)
		{
			auto& b = bonestack[i];
			bone = b * bone;
		}

		m_bones.push_back(bone);
		m_frame_bones.push_back(glm::mat4(1.0));
	}
}

void CompiledModel::nextFrame()
{
	auto now = clock::now();
	double passed = std::chrono::duration_cast<std::chrono::microseconds>(now - m_last).count()/1000.0;
	m_last = now;

	int ca = m_currentAni;
	//compute pose with ani
	if (m_animations.size() > ca)
	{
		//works only for one animation !!!
		auto& ani = m_animations[ca];
		ani.passed += passed;

		int passed_frames = static_cast<int>(ani.passed / ani.delta);
		std::cout << "Passed frames: " << passed_frames << std::endl;

		if(ani.passed > ani.delta)
		{
			int i = 0;
			for (auto& f_bone : m_frame_bones)
			{
				f_bone = m_bones[i];
				++i;
			}
			
			ani.frame += passed_frames;
			ani.passed = fmod(ani.passed, ani.delta);

			ani.frame %= ani.animation->Header.NumFrames;

			for (auto channel : ani.animation->Channels)
			{
				int firstFrame = channel->Header.FirstFrame;
				int lastFrame = channel->Header.LastFrame;
				auto& bone = m_frame_bones[channel->Header.Pivot];
				int f = ani.frame  - firstFrame -1;
				f = std::max(0, f);
				f = std::min(lastFrame - firstFrame -1, f);

				switch (channel->Header.Flags)
				{
				case 0:
				{
					auto data = getDataVector<float>(channel->Data);
					bone = glm::translate(bone, glm::vec3(data[f], 0.0, 0.0));
				}
				break;
				case 1:
				{
					auto data = getDataVector<float>(channel->Data);
					bone = glm::translate(bone, glm::vec3(0.0, data[f], 0.0));
				}
				break;
				case 2:
				{
					auto data = getDataVector<float>(channel->Data);
					bone = glm::translate(bone, glm::vec3(0.0, 0.0, data[f]));
				}
				break;
				case 6:
				{
					auto data = getDataVector<glm::quat>(channel->Data);
					auto raw = data[f];
					auto quad = glm::quat(raw.w, raw.x, raw.y, raw.z);
					auto r_mat = glm::toMat4(quad);
					bone *= r_mat;
				}
				break;
				}
			}

		}
	}
}

void CompiledModel::Render(Shader& s)
{
	nextFrame();
	
	for (auto& mesh : m_meshes)
	{
		glBindBuffer(GL_ARRAY_BUFFER,mesh.vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
		
		glUniform1i(s.uniform("has_diffuse"), (mesh.diffuse!=nullptr));
		if (mesh.diffuse)
		{
			glActiveTexture(GL_TEXTURE0);
			glUniform1i(s.uniform("diffuse"), 0);
			mesh.diffuse->Bind();
		}

		glm::mat4 model;

		if ((mesh.pivot != -1) && (m_bones.size()>0))
		{
			model = m_bones[mesh.pivot];
		}

		glUniform1i(s.uniform("has_skinning"), mesh.skinned);
		if (mesh.skinned)
		{
			if (m_animations.size() > 0)
			{
				glUniformMatrix4fv(s.uniform("bones"), m_frame_bones.size(), false, glm::value_ptr(m_frame_bones.front()));
			}
			else
			{
				glUniformMatrix4fv(s.uniform("bones"), m_bones.size(), false, glm::value_ptr(m_bones.front()));
			}
		}

		glUniformMatrix4fv(s.uniform("m"), 1, false, glm::value_ptr(model));
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CompiledModel::Vertex), (void*)offsetof(CompiledModel::Vertex, position));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(CompiledModel::Vertex), (void*)offsetof(CompiledModel::Vertex, normal));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(CompiledModel::Vertex), (void*)offsetof(CompiledModel::Vertex, txcoord));
		glVertexAttribIPointer(3, 2, GL_INT, sizeof(CompiledModel::Vertex), (void*)offsetof(CompiledModel::Vertex, bones));

		glDrawElements(GL_TRIANGLES, mesh.num, GL_UNSIGNED_SHORT, nullptr);
	}
}

CompiledModel::Mesh::Mesh() : skinned(false), vbo(0),ibo(0),num(0),pivot(-1)
{
}

