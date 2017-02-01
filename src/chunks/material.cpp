#include <libw3d/chunks/material.hpp>
#include "../util.hpp"
using namespace libw3d;

void MaterialPass::Load(std::ifstream & fin, uint32_t chunksize)
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
		case TEXTURE_STAGE:
			break;
		case STAGE_TEXCOORDS:
			Texcoords = readArray<Vector2f>(fin, subend);
			break;
		default:
			fin.seekg(size, std::ios::cur);
			break;
		}
	}
}
