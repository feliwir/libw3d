#include <libw3d/chunks/mesh.hpp>
#include "../util.hpp"
using namespace libw3d;

void Mesh::Load(std::ifstream & fin, uint32_t chunksize)
{
	uint32_t chunkend = static_cast<uint32_t>(fin.tellg()) + chunksize;
	while (fin.tellg() < chunkend)
	{
		ChunkType type = static_cast<ChunkType>(read<uint32_t>(fin));
		uint32_t info = read<uint32_t>(fin);
		bool subChunks = (info >> 31);
		uint32_t size = info & 0x7FFFFFFF;
		uint32_t subend = static_cast<uint32_t>(fin.tellg()) + size;

		switch (type)
		{
		case MESH_HEADER3:
			Header = read<MeshHeader>(fin);
			break;
		case VERTICES:
			Vertices = readArray<Vector3f>(fin, subend);
			break;
		case VERTEX_NORMALS:
			Normals = readArray<Vector3f>(fin, subend);
			break;
		case TRIANGLES:
			Triangles = readArray<Triangle>(fin, subend);
			break;
		case VERTEX_INFLUENCES:
			VertexInfluences = readArray<Influence>(fin, subend);
			break;
		case TEXTURES:
			Textures = std::make_shared<TextureArray>(); 
			Textures->Load(fin,size);
			break;
		case NORMALMAP_INFO:
			TextureMaps = std::make_shared<NormalmapArray>();
			TextureMaps->Load(fin,size);
			break;
		case MATERIAL_PASS:
			MatPass = std::make_shared<MaterialPass>();
			MatPass->Load(fin, size);
			break;
		case VERTEX_SHADE_INDICES:
			ShadeIndices = readArray<uint32_t>(fin, subend);
			break;
		case MATERIAL_INFO:
			MatInfo = read<MaterialInfo>(fin);
			break;
		default:
			fin.seekg(size, std::ios::cur);
			break;
		}
	}
}

