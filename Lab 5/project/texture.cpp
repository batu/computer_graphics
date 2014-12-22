#include "texture.h"
#include "stb_image.h"
#include <iostream>
#include "GL/freeglut.h"

Texture::Texture(){}

Texture::Texture(const std::string& fileName){
	Load(fileName);
}

void Texture::Load(const std::string& fileName){
	int width, height, numComponents;
	unsigned char* data = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4);

	if(data == NULL)
		std::cerr << "Unable to load texture: " << fileName << std::endl; 
	else{ // FLIP data since STB_IMAGE loads image with top left corner as origin and y axis running from top to bottom
			for(int i = 0; i < height/2; ++i){
			for(int j = 0; j < width; ++j){
				for(int k =0; k<4; ++k){
					int a = 4*(i*width + j)+k; 
					int b = 4*((height - 1 - i)*width + j)+k;
					unsigned char c = data[a];
					data[a] = data[b];
					data[b] = c;
				}
			}
		}
	}
		
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, 
				GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}


Texture::~Texture()
{
	glDeleteTextures(1, &m_texture);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
}
