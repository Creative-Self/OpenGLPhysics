#include "Texture.h"



Texture::Texture()
{
}

Texture::Texture(string path)
{
	FIBITMAP* bitimage = FreeImage_Load(FreeImage_GetFileType(path.c_str(), 0), path.c_str()); // texture data
	if (bitimage != nullptr) { std::cout << "Texture loaded successfully." << std::endl; }
	else { std::cout << "Texture load failed." << std::endl; }

	FIBITMAP* textureImage = FreeImage_ConvertTo32Bits(bitimage); // convert texture data to image 
	int width = FreeImage_GetWidth(textureImage); 
	int height = FreeImage_GetHeight(textureImage); 
	void* address = FreeImage_GetBits(textureImage);

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(
		GL_TEXTURE_2D, 0,
		GL_SRGB8_ALPHA8, width, height, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, address);

	FreeImage_Unload(bitimage); 
	FreeImage_Unload(textureImage); 

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bindTexture()
{
	glBindTexture(GL_TEXTURE_2D, texID); // bind texture on model
}

void Texture::unloadTexture()
{
	glDeleteTextures(1, &texID);
}


Texture::~Texture()
{
	
}
