#include <libw3d/model.hpp>
using namespace libw3d;

Model::Model() : m_valid(false)
{
}

void Model::AddChunk(const std::shared_ptr<Chunk> chunk)
{
	switch (chunk->Type)
	{
	case MESH:
		Meshes.push_back(std::dynamic_pointer_cast<Mesh>(chunk));
		break;
	case HIERARCHY:
		Skeleton = std::dynamic_pointer_cast<Hierarchy>(chunk);
		break;
	case COMPRESSED_ANIMATION:
		CompressedAnimations.push_back(std::dynamic_pointer_cast<CompressedAnimation>(chunk));
		break;
	case HLOD:
		HierarchyLoD = std::dynamic_pointer_cast<HLoD>(chunk);
		break;
	default:
		Unknown.push_back(chunk);
		break;
	}
}

void Model::AddSkeleton(const Model& skl)
{
	Skeleton = skl.Skeleton;
}

void Model::AddAnimation(const Model& ani)
{
	Animations.insert(Animations.end(), ani.Animations.begin(), ani.Animations.end());
}