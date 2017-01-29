#pragma once
#include <string>
#include <fstream>
#include "model.hpp"

namespace libw3d
{
	class Loader
	{
	public:
		static Model FromFile(const std::string& filename,bool loadref=false);
	private:
		static std::shared_ptr<Chunk> ReadChunk(std::ifstream& fin);
	};
}