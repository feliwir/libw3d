#include <libw3d/loader.hpp>
#include <libw3d/chunks/mesh.hpp>
#include <iostream>
#include "util.hpp"
using namespace libw3d;

Model Loader::FromFile(const std::string& filename)
{
	Model m;
	std::ifstream fin(filename,std::ios::binary);
	if (fin.fail())
		return m;
	
	fin.seekg(0, std::ios::end);
	std::streampos fileSize = fin.tellg();
	fin.seekg(0, std::ios::beg);

	while (fin.tellg() < fileSize)
	{
		m.AddChunk(ReadChunk(fin));
	}

	return m;
}

std::shared_ptr<Chunk> Loader::ReadChunk(std::ifstream& fin)
{
	std::shared_ptr<Chunk> chunk;
	Chunk::Type type =  static_cast<Chunk::Type>(read<uint32_t>(fin));
	uint32_t info = read<uint32_t>(fin);
	bool subChunks = (info >> 31);
	uint32_t size = info & 0x7FFFFFFF;
	uint32_t end = static_cast<uint32_t>(fin.tellg()) + size;
	
	switch (type)
	{
	case Chunk::MESH:
		chunk = std::make_shared<Mesh>();
		break;
	default:
		std::cout << "Unknown chunk: " << type << std::endl;
		chunk = std::make_shared<Chunk>();
	}

	chunk->Load(fin, size);
		
	return chunk;
}
