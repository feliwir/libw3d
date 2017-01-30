#include <libw3d/chunks/texture.hpp>
#include <stdint.h>
#include <memory>
#include "../util.hpp"
using namespace libw3d;

void TextureArray::Load(std::ifstream & fin, uint32_t chunksize)
{
	uint32_t chunkend = static_cast<uint32_t>(fin.tellg()) + chunksize;
	while (fin.tellg() < chunkend)
	{
		ChunkType type = static_cast<ChunkType>(read<uint32_t>(fin));
		uint32_t info = read<uint32_t>(fin);
		bool subChunks = (info >> 31);
		uint32_t size = info & 0x7FFFFFFF;
		uint32_t subend = static_cast<uint32_t>(fin.tellg()) + size;
		std::shared_ptr<Texture> tex;

		switch (type)
		{
		case TEXTURE:
			tex = std::make_shared<Texture>();
			tex->Load(fin, size);
			Textures.push_back(tex);
			break;
		default:
			fin.seekg(size, std::ios::cur);
			break;
		}
	}
}

void Texture::Load(std::ifstream & fin, uint32_t chunksize)
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
		case TEXTURE_NAME:
			Name = readString(fin);
			break;
		default:
			fin.seekg(size, std::ios::cur);
			break;
		}
	}
}
