#pragma once
#include <stdint.h>
#include <fstream>
#include "types.hpp"

namespace libw3d
{
	class Chunk
	{
	public:
		ChunkType Type;
	public:
		virtual void Load(std::ifstream& fin, uint32_t chunksize);
	};
}