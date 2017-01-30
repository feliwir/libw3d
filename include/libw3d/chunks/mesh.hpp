#pragma once
#include <vector>
#include <memory>
#include "../chunk.hpp"
#include "../math.hpp"
#include "material.hpp"
#include "normalmap.hpp"
#include "texture.hpp"

namespace libw3d
{
	class Mesh : public Chunk
	{
	public:
		virtual void Load(std::ifstream& fin, uint32_t chunksize) override;
	public:
		MeshHeader						Header;
		std::vector<Vector3f>			Vertices;
		std::vector<Vector3f>			Normals;
		std::vector<Triangle>			Triangles;
		std::vector<Influence>			VertexInfluences;
		std::vector<uint32_t>			ShadeIndices;
		MaterialInfo					MatInfo;
		std::shared_ptr<MaterialPass>	MatPass;
		std::shared_ptr<NormalmapArray>	TextureMaps;
		std::shared_ptr<TextureArray>	Textures;
	};
}