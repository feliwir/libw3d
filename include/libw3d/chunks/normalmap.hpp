#pragma once
#include <memory>
#include <vector>
#include "../chunk.hpp"
#include "../math.hpp"

namespace libw3d
{
	class Normalmap : public Chunk
	{
	public:
		virtual void Load(std::ifstream& fin, uint32_t chunksize) override;
	public:
		NormMapHeaderStruct Header;
		NormMapEntryStruct Entry;
	};

	class NormalmapArray : public Chunk
	{
	public:
		virtual void Load(std::ifstream& fin, uint32_t chunksize) override;
	public:
		std::vector<std::shared_ptr<Normalmap>> TextureMaps;
	};
}