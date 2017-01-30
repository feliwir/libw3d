#pragma once
#include <memory>
#include <vector>
#include "../chunk.hpp"

namespace libw3d
{
	class HLoDArray : public Chunk
	{
	public:
		virtual void Load(std::ifstream& fin, uint32_t chunksize) override;
	public:
		HLodArrayHeader Header;
		std::vector<HLodSubObject> SubOjects;
	};

	class HLoD : public Chunk
	{
	public:
		virtual void Load(std::ifstream& fin, uint32_t chunksize) override;
	public:
		HLodHeader Header;
		std::shared_ptr<HLoDArray> SubObjectArray;
	};
}
