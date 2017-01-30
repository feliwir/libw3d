#pragma once
#include <vector>
#include <memory>
#include "../chunk.hpp"
#include "../math.hpp"

namespace libw3d
{
    class Texture : public Chunk
	{
	public:
		virtual void Load(std::ifstream& fin, uint32_t chunksize) override;
	public:
		std::string Name;
	};

	class TextureArray : public Chunk
	{
	public:
		virtual void Load(std::ifstream& fin, uint32_t chunksize) override;
	public:
		std::vector<std::shared_ptr<Texture>> Textures;
	};
}