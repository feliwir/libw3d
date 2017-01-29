#include <libw3d/chunks/hierarchy.hpp>
#include "../util.hpp"
using namespace libw3d;

void Hierarchy::Load(std::ifstream & fin, uint32_t chunksize)
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
		case HIERARCHY_HEADER:
			Header = read<HierarchyHeader>(fin);
			break;
		case PIVOTS:
			Bones = readArray<Pivot>(fin, subend);
			break;
		default:
			fin.seekg(size, std::ios::cur);
			break;
		}
	}
}
