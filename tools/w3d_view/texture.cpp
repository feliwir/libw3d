#include "texture.hpp"
#include "targa.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>

Texture::Texture() : m_texId(0)
{
	glGenTextures(1, &m_texId);
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::~Texture()
{
	if (m_texId)
	{
		glDeleteTextures(1, &m_texId);
		m_texId = 0;
	}
}

bool Texture::Load(const std::string& filename)
{
	std::string name = filename;
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	m_texture = gli::load(name);
 	if (m_texture.empty())
	{
		GLuint format = GL_BGR;
		std::ifstream fin(name,std::ios::binary);
		if (fin.fail())
		{
			std::cout << "Failed to load: " << filename << std::endl;
			return false;
		}
			

		Targa tga;
		fin.read((char*)tga.Header, 12);
		fin.read((char*)&tga.Width, 2);
		fin.read((char*)&tga.Height, 2);
		fin.read((char*)&tga.Bpp, 1);
		fin.read((char*)&tga.Id, 1);
		uint32_t size = tga.Width*tga.Height*(tga.Bpp/8);
		tga.Data = new uint8_t[size];
		fin.read((char*)tga.Data, size);
		
		switch (tga.Bpp)
		{
		case 24:
			format = GL_BGR;
			break;
		case 32:
			format = GL_BGRA;
			break;
		}
		glBindTexture(GL_TEXTURE_2D, m_texId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tga.Width, tga.Height, 0, format, GL_UNSIGNED_BYTE, tga.Data);
		delete[] tga.Data;
	}
	else
	{
		gli::gl GL(gli::gl::PROFILE_GL33);
		gli::gl::format const Format = GL.translate(m_texture.format(), m_texture.swizzles());
		GLenum Target = GL.translate(m_texture.target());

		glBindTexture(Target, m_texId);
		glTexParameteri(Target, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(Target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(m_texture.levels() - 1));
		glTexParameteriv(Target, GL_TEXTURE_SWIZZLE_RGBA, &Format.Swizzles[0]);
		
		for (std::size_t Level = 0; Level < m_texture.levels(); ++Level)
		{
			glm::tvec3<GLsizei> Extent(m_texture.extent(Level));
			glCompressedTexImage2D(Target, Level, Format.Internal, Extent.x, Extent.y, 0, static_cast<GLsizei>(m_texture.size(Level)), m_texture.data(0, 0, Level));
		}

	}

	return true;
}

void Texture::Bind()
{

	glBindTexture(GL_TEXTURE_2D, m_texId);
}
