#pragma once
#include <fstream>
#include <vector>

namespace libw3d
{
	template <typename T>
	static inline T read(std::ifstream& fin)
	{
		T result = T();

		fin.read(reinterpret_cast<char*>(&result), sizeof(T));

		return result;
	}

	template <typename T>
	static inline std::vector<T> readArray(std::ifstream& fin, uint32_t end)
	{
		std::vector<T> result;
		while (fin.tellg() < end)
		{
			result.push_back(read<T>(fin));
		}

		return result;
	}
}