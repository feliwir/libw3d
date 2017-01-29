#pragma once
#include "../chunk.hpp"
#include "../math.hpp"
#include <vector>
#include <memory>

namespace libw3d
{
	class MotionChannel
	{
	public:
		virtual void Load(std::ifstream& fin, uint32_t chunksize, const MotionChannelHeader& header) = 0;
	public:
		MotionChannelHeader Header;
	};

	class TimeCodedChannel : public MotionChannel
	{
	public:
		virtual void Load(std::ifstream& fin, uint32_t chunksize, const MotionChannelHeader& header) override;
	public:
		std::vector<uint16_t> Keyframes;
		std::vector<float> Data;
	};

	class AdaptiveDelta4 : public MotionChannel
	{
	public:
		virtual void Load(std::ifstream& fin, uint32_t chunksize, const MotionChannelHeader& header) override;
	public:
		float Increment;
		std::vector<float> Vector;
		std::vector<uint8_t> Data;
	};

	class Animation : public Chunk
	{
	public:
		virtual void Load(std::ifstream& fin, uint32_t chunksize) override;
	};

	class CompressedAnimation : public Chunk
	{
	public:

	public:
		virtual void Load(std::ifstream& fin, uint32_t chunksize) override;
	private:
		void AddChannel(std::ifstream& fin, uint32_t chunksize);
	public:
		CompressedAnimationHeader Header;
		std::vector<std::shared_ptr<MotionChannel>> Channels;
	};
}