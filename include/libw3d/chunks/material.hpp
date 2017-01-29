#pragma once
#include <vector>
#include "../chunk.hpp"
#include "../math.hpp"

namespace libw3d
{
	class MaterialPass : public Chunk
	{
	public:
		virtual void Load(std::ifstream& fin, uint32_t chunksize) override;
	public:
		std::vector<Vector2f> Texcoords;
	};
}