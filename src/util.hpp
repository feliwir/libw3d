#pragma once
#include <fstream>

namespace libw3d
{
	template <class T>
	inline T read(std::ifstream& fin)
	{
		T result;
		fin.read(reinterpret_cast<char*>(&result), sizeof(T));
		return result;
	}
}