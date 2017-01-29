#pragma once
#include <vector>
#include "../chunk.hpp"
#include "../math.hpp"

namespace libw3d
{
	class Mesh : public Chunk
	{
	public:
		virtual void Load(std::ifstream& fin, uint32_t chunksize) override;
	public:
		MeshHeader				Header;
		std::vector<Vector3f>	Vertices;
		std::vector<Vector3f>	Normals;
		std::vector<Triangle>	Triangles;
		std::vector<Influence>	VertexInfluences;
		std::vector<uint32_t>	ShadeIndices;
		MaterialInfo			MatInfo;
	};
}