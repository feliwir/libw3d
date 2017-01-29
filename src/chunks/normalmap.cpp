#include <libw3d/chunks/normalmap.hpp>
#include "../util.hpp"
using namespace libw3d;

void NormalmapArray::Load(std::ifstream & fin, uint32_t chunksize)
{
	uint32_t chunkend = static_cast<uint32_t>(fin.tellg()) + chunksize;
	while (fin.tellg() < chunkend)
	{
		ChunkType type = static_cast<ChunkType>(read<uint32_t>(fin));
		uint32_t info = read<uint32_t>(fin);
		bool subChunks = (info >> 31);
		uint32_t size = info & 0x7FFFFFFF;
		uint32_t subend = static_cast<uint32_t>(fin.tellg()) + size;
		std::shared_ptr<Normalmap> entry;
		
		switch (type)
		{
		case NORMALMAP_ENTRY:
			entry = std::make_shared<Normalmap>();
			entry->Load(fin, size);
			TextureMaps.push_back(entry);
			break;
		default:
			fin.seekg(size, std::ios::cur);
			break;
		}
	}
}

void Normalmap::Load(std::ifstream & fin, uint32_t chunksize)
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
		case NORMALMAP_HEADER:
			sizeof(NormMapHeaderStruct);
			Header = read<NormMapHeaderStruct>(fin);
			break;
		case NORMALMAP_CONTENT:
			Entry = read<NormMapEntryStruct>(fin);
			break;
		default:
			fin.seekg(size, std::ios::cur);
			break;
		}
	}
}
