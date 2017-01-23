#include <libw3d/model.hpp>
using namespace libw3d;

Model::Model() : m_valid(false)
{
}

void Model::AddChunk(const std::shared_ptr<Chunk> chunk)
{
	m_chunks.push_back(chunk);
}
