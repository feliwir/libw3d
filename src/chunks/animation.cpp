#include <libw3d/chunks/animation.hpp>
#include "../util.hpp"
#include <cassert>
using namespace libw3d;

void Animation::Load(std::ifstream & fin, uint32_t chunksize)
{
	uint32_t chunkend = static_cast<uint32_t>(fin.tellg()) + chunksize;
	while (fin.tellg() < chunkend)
	{
		ChunkType type = static_cast<ChunkType>(read<uint32_t>(fin));
		uint32_t info = read<uint32_t>(fin);
		uint32_t size = info & 0x7FFFFFFF;
		uint32_t subend = static_cast<uint32_t>(fin.tellg()) + size;

		switch (type)
		{
		case ANIMATION_HEADER:
			Header = read<AnimationHeader>(fin);
			break;
		case ANIMATION_CHANNEL:
			AddChannel(fin, size);
			break;
		default:
			fin.seekg(size, std::ios::cur);
			break;
		}
	}
}

void Animation::AddChannel(std::ifstream& fin, uint32_t chunksize)
{
	std::shared_ptr<Channel> channel = std::make_shared<Channel>();
	assert(chunksize > sizeof(ChannelHeader));
	ChannelHeader header;
	header.FirstFrame = read<uint16_t>(fin);
	header.LastFrame = read<uint16_t>(fin);
	header.VectorLen = read<uint16_t>(fin);
	header.Flags = read<uint16_t>(fin);
	header.Pivot = read<uint16_t>(fin);
	header.Pad = read<uint16_t>(fin);

	channel->Load(fin, chunksize, header);
	Channels.push_back(channel);
}

void CompressedAnimation::Load(std::ifstream & fin, uint32_t chunksize)
{
	uint32_t chunkend = static_cast<uint32_t>(fin.tellg()) + chunksize;
	while (fin.tellg() < chunkend)
	{
		ChunkType type = static_cast<ChunkType>(read<uint32_t>(fin));
		uint32_t info = read<uint32_t>(fin);
		bool subChunks = (info >> 31);
		uint32_t size = info & 0x7FFFFFFF;
		uint32_t subend = static_cast<uint32_t>(fin.tellg()) + size;

		switch (type)
		{
		case COMPRESSED_ANIMATION_HEADER:
			Header = read<CompressedAnimationHeader>(fin);
			break;
		case COMPRESSED_ANIMATION_MOTION_CHANNEL:
			AddChannel(fin, size);
			break;
		default:
			fin.seekg(size, std::ios::cur);
			break;
		}
	}
}

void CompressedAnimation::AddChannel(std::ifstream& fin, uint32_t chunksize)
{
	std::shared_ptr<MotionChannel> channel;
	assert(chunksize > sizeof(MotionChannelHeader));
	MotionChannelHeader header = read<MotionChannelHeader>(fin);

	switch (header.DeltaType)
	{
	case TIMECODED:
		channel = std::make_shared<TimeCodedChannel>();
		break;
	case DELTA4:
		channel = std::make_shared<AdaptiveDelta4>();
	case DELTA8:
		break;
	}

	if (channel)
	{
		channel->Load(fin, chunksize, header);
		Channels.push_back(channel);
	}	
	else
	{
		fin.seekg(chunksize - sizeof(MotionChannelHeader), std::ios::cur);
	}
}

void Channel::Load(std::ifstream &fin, uint32_t chunksize, const ChannelHeader &header)
{
	Header = header;
	Data = readArrayByNum<uint8_t>(fin, chunksize - 12);
}

void TimeCodedChannel::Load(std::ifstream & fin, uint32_t chunksize, const MotionChannelHeader & header)
{
	Header = header;
	Keyframes = readArrayByNum<uint16_t>(fin, Header.Framecount);
	if (Header.Framecount & 1)
		fin.seekg(2, std::ios::cur);
	Data = readArrayByNum<float>(fin, Header.Framecount*Header.VectorLen);
}

void AdaptiveDelta4::Load(std::ifstream & fin, uint32_t chunksize, const MotionChannelHeader & header)
{
	Header = header;
	Increment = read<float>(fin);
	Vector = readArrayByNum<float>(fin, Header.VectorLen);
	uint32_t Remaining = chunksize - sizeof(MotionChannelHeader) - Header.VectorLen * sizeof(float);
	Data = readArrayByNum<uint8_t>(fin, Remaining);
}
