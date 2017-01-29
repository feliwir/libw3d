#pragma once
#include <string>
#include <gli/gli.hpp>
#include "flextGL.h"

class Texture
{
public:
	enum Format
	{
		RGB = 0,
		RGBA = 1,
	};

	Texture();
	~Texture();

	bool Load(const std::string& filename);

	void Bind();
private:
	GLuint m_texId;
	gli::texture m_texture;
	Format m_format;
};
