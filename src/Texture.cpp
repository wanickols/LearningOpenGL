#include "pch.h"
#include "Texture.h"
#include "stb_image/stb_image.h"

Texture::Texture(const string& path)
	: m_FilePath(path), m_LocalBuffer(nullptr), 
	m_Width(0), m_Height(0), m_BPP(0)
{
	//flips file because open gl uses bottom left as start
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);


	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	//settings
	
	//glViewport(0, 0, m_Width, m_Height);

	//If you don't specify these you will likely get a black texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//send data to opengl
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
	Unbind();

	//If you want to keep the data around for sampling or other this is not technically necessary
	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot); //goes up to 31 (it does not mean the platform has that many)
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
