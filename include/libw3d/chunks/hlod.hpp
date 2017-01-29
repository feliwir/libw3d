#pragma once
#include "../chunk.hpp"

namespace libw3d
{
	class HLoD : public Chunk
	{
	public:
		virtual void Load(std::ifstream& fin, uint32_t chunksize) override;
	public:
		HLodHeader Header;
	};
}