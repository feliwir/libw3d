#include <libw3d/chunks/hlod.hpp>
#include "../util.hpp"
using namespace libw3d;

void HLoD::Load(std::ifstream & fin, uint32_t chunksize)
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
		case HLOD_HEADER:
			Header = read<HLodHeader>(fin);
			break;
		case HLOD_LOD_ARRAY:
			SubObjectArray = std::make_shared<HLoDArray>();
			SubObjectArray->Load(fin, size);
			break;
		default:
			fin.seekg(size, std::ios::cur);
			break;
		}
	}
}

void HLoDArray::Load(std::ifstream & fin, uint32_t chunksize)
{
	uint32_t chunkend = static_cast<uint32_t>(fin.tellg()) + chunksize;

	while (fin.tellg() < chunkend)
	{
		ChunkType type = static_cast<ChunkType>(read<uint32_t>(fin));
		uint32_t info = read<uint32_t>(fin);
		bool subChunks = (info >> 31);
		uint32_t size = info & 0x7FFFFFFF;
		uint32_t subend = static_cast<uint32_t>(fin.tellg()) + size;
		HLodSubObject object;

		switch (type)
		{
		case HLOD_SUB_OBJECT_ARRAY_HEADER:
			Header = read<HLodArrayHeader>(fin);
			break;
		case HLOD_SUB_OBJECT:
			object = read<HLodSubObject>(fin);
			SubOjects.push_back(object);
			break;
		default:
			fin.seekg(size, std::ios::cur);
			break;
		}
	}
}
