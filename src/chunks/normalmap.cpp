#include <libw3d/chunks/normalmap.hpp>
#include <stdint.h>
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
		uint32_t offset = fin.tellg();
		ChunkType type = static_cast<ChunkType>(read<uint32_t>(fin));
		uint32_t info = read<uint32_t>(fin);
		bool subChunks = (info >> 31);
		uint32_t size = info & 0x7FFFFFFF;
		uint32_t subend = static_cast<uint32_t>(fin.tellg()) + size;
		NormMapEntryStruct Entry;
		switch (type)
		{
		case NORMALMAP_HEADER:
			sizeof(NormMapHeaderStruct);
			Header = read<NormMapHeaderStruct>(fin);
			break;
		case NORMALMAP_CONTENT:
			Entry.TypeFlag = read<NormMapHeaderType>(fin);
			Entry.TypeSize = read<uint32_t>(fin);
			Entry.TypeName = readString(fin);
					
			switch(Entry.TypeFlag)
			{
				case NORMTYPE_TEXTURE:
				Entry.ItemSize = read<uint32_t>(fin);
				Entry.ItemName = readString(fin);
				break;
				case NORMTYPE_BUMP:
				Entry.ItemScalar = read<float>(fin);
				break;
				case NORMTYPE_COLORS:
				Entry.ItemColor = read<Vector4f>(fin);
				break;
				case NORMTYPE_ALPHA:
				Entry.ItemAlpha = read<uint8_t>(fin);
				break;
			}
			
			Entries.push_back(Entry);
			break;
		default:
			break;
			fin.seekg(size, std::ios::cur);
			break;
		}
	}
}
