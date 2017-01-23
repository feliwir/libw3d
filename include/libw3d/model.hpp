#pragma once
#include <vector>
#include <memory>
#include "chunk.hpp"

namespace libw3d
{
    class Model
    {
	public:
		Model();

		void AddChunk(const std::shared_ptr<Chunk> chunk);

		inline void SetValid(bool valid)
		{
			m_valid = valid;
		}
	private:
		std::vector<std::shared_ptr<Chunk>> m_chunks;
		uint32_t m_filesize;
		bool m_valid;
    };
}