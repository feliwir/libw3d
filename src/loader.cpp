#include <libw3d/loader.hpp>
#include <libw3d/chunks/hlod.hpp>
#include <libw3d/chunks/mesh.hpp>
#include <libw3d/chunks/animation.hpp>
#include <libw3d/chunks/hierarchy.hpp>
#include <iostream>
#include "util.hpp"
using namespace libw3d;

Model Loader::FromFile(const std::string& filename, bool refload)
{
	Model m;
	std::ifstream fin(filename, std::ios::binary);
	if (fin.fail())
	{
		std::cout << "Failed to load: " << filename << std::endl;
		return m;
	}

	fin.seekg(0, std::ios::end);
	std::streampos fileSize = fin.tellg();
	fin.seekg(0, std::ios::beg);
	m.Filesize = fileSize;

	while (fin.tellg() < fileSize)
	{
		m.AddChunk(ReadChunk(fin));	
	}
	fin.close();
	//load skeleton or other stuff referenced
	if (refload)
	{
		//check if a skeleton exists
		auto hlod = m.HierarchyLoD;
		if (hlod)
		{
			std::string skl_name = hlod->Header.HierarchyName;
			if (m.Meshes.size() > 0)
			{
				std::string container_name = m.Meshes[0]->Header.ContainerName;
				if (container_name != skl_name)
				{
					Model skl = FromFile(skl_name + ".w3d");
					m.AddSkeleton(skl);
				}
			}
		}
	}

	m.SetValid(true);
	return m;
}

std::shared_ptr<Chunk> Loader::ReadChunk(std::ifstream& fin)
{
	std::shared_ptr<Chunk> chunk;
	ChunkType type =  static_cast<ChunkType>(read<uint32_t>(fin));
	uint32_t info = read<uint32_t>(fin);
	bool subChunks = (info >> 31);
	uint32_t size = info & 0x7FFFFFFF;
	uint32_t end = static_cast<uint32_t>(fin.tellg()) + size;
	
	switch (type)
	{
	case MESH:
		chunk = std::make_shared<Mesh>();
		break;
	case HIERARCHY:
		chunk = std::make_shared<Hierarchy>();
		break;
	case COMPRESSED_ANIMATION:
		chunk = std::make_shared<CompressedAnimation>();
		break;
	case HLOD:
		chunk = std::make_shared<HLoD>();
		break;
	default:
		std::cout << "Unknown chunk: " << type << std::endl;
		chunk = std::make_shared<Chunk>();
	}

	chunk->Type = type;
	chunk->Load(fin, size);
		
	return chunk;
}
