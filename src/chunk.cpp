#include <libw3d/chunk.hpp>
using namespace libw3d;

void Chunk::Load(std::ifstream& fin,uint32_t chunksize)
{
	//default behaviour for unknown chunks
	fin.seekg(chunksize, std::ios::cur);
}
