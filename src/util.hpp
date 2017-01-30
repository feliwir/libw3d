#pragma once
#include <fstream>
#include <vector>

namespace libw3d
{
	static inline const std::string readString(std::ifstream& fin)
	{
		std::string buffer;
        char c;
        while ((c = fin.get()) != '\0') {
            buffer += c;
        }

        return buffer;
	}

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

	template <typename T>
	static inline std::vector<T> readArrayByNum(std::ifstream& fin, uint32_t numElements)
	{
		std::vector<T> result;
		for(int i=0;i<numElements;++i)
		{
			result.push_back(read<T>(fin));
		}

		return result;
	}
}