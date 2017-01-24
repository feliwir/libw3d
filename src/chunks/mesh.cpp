#include <libw3d/chunks/mesh.hpp>
#include "../util.hpp"
using namespace libw3d;

void Mesh::Load(std::ifstream & fin, uint32_t chunksize)
{
	//TODO add parsing mesh
	uint32_t chunkend = static_cast<uint32_t>(fin.tellg()) + chunksize;
	while (fin.tellg() < chunkend)
	{
		Chunk::Type type = static_cast<Chunk::Type>(read<uint32_t>(fin));
		uint32_t info = read<uint32_t>(fin);
		bool subChunks = (info >> 31);
		uint32_t size = info & 0x7FFFFFFF;
		uint32_t subend = static_cast<uint32_t>(fin.tellg()) + size;

		switch (type)
		{
		case Chunk::MESH_HEADER3:
			MeshHeader = read<Header>(fin);
			break;
		case Chunk::VERTICES:
			Vertices = readArray<Vector3f>(fin, subend);
			break;
		case Chunk::VERTEX_NORMALS:
			Normals = readArray<Vector3f>(fin, subend);
			break;
		case Chunk::TRIANGLES:
			Triangles = readArray<Triangle>(fin, subend);
			break;
		case Chunk::VERTEX_INFLUENCES:
			VertexInfluences = readArray<Influence>(fin, subend);
			break;
		case Chunk::VERTEX_SHADE_INDICES:
			ShadeIndices = readArray<uint32_t>(fin, subend);
			break;
		case Chunk::MATERIAL_INFO:
			MatInfo = read<MaterialInfo>(fin);
			break;
		}
	}
}

