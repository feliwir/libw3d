#pragma once
#include <vector>
#include <memory>
#include "chunk.hpp"
#include "chunks/hlod.hpp"
#include "chunks/hierarchy.hpp"
#include "chunks/mesh.hpp"
#include "chunks/animation.hpp"

namespace libw3d
{
    class Model
    {
	public:
		Model();

		void AddChunk(const std::shared_ptr<Chunk> chunk);

		void AddSkeleton(const Model& skl);

		inline void SetValid(bool valid)
		{
			m_valid = valid;
		}
	public:
		std::vector<std::shared_ptr<Mesh>> Meshes;
		std::vector<std::shared_ptr<CompressedAnimation>> CompressedAnimations;
		std::vector<std::shared_ptr<Chunk>> Unknown;
		std::shared_ptr<HLoD> HierarchyLoD;
		std::shared_ptr<Hierarchy> Skeleton;
		uint32_t Filesize;
		bool m_valid;
    };
}