#pragma once
#include "../chunk.hpp"
#include "../math.hpp"
#include <vector>
#include <memory>

namespace libw3d
{
    class Hierarchy : public Chunk
	{
	public:
		virtual void Load(std::ifstream& fin, uint32_t chunksize) override;
	public:
		HierarchyHeader Header;
		std::vector<Pivot> Bones;
	};
}