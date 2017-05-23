#include <cstdio>
#include <cstdlib>
#include "TextrueHandler.h"
#include <string>
#include <iostream>

const int BMP_Header_Length = 54;

TextrueHandler::TextrueHandler(char* fileName)
{
	GLint ImageWidth;
	GLint ImageHeight;
	// 打开文件
	FILE* pFile;
	char buffer[36];
	snprintf(buffer, sizeof(buffer),"images/%s", fileName);
	std::cout << buffer << std::endl;
	fopen_s(&pFile, buffer, "rb");
	if (pFile == nullptr)
		exit(0);

	// 读取图象的大小信息
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&ImageWidth, sizeof(ImageWidth), 1, pFile);
	fread(&ImageHeight, sizeof(ImageHeight), 1, pFile);

	// 计算像素数据长度
	GLint PixelLength = ImageWidth * 3;
	while (PixelLength % 4 != 0)
		++PixelLength;
	PixelLength *= ImageHeight;

	// 读取像素数据
	GLubyte* PixelData = static_cast<GLubyte*>(malloc(PixelLength));
	if (PixelData == nullptr)
		exit(0);

	fseek(pFile, 54, SEEK_SET);
	fread(PixelData, PixelLength, 1, pFile);

	// 关闭文件
	fclose(pFile);
	// make a gl texture
	//glGenTextures(GLsizei n, GLuint *textures)
	//n：用来生成纹理的数量
	//textures：存储纹理索引的第一个元素指针
	glGenTextures(1, &textureHandle);

	glBindTexture(GL_TEXTURE_2D, textureHandle);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
	                             GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

//	glTexImage2D(GL_TEXTURE_2D, 0, 3, ImageWidth, ImageHeight, 0,
//	                          GL_BGR_EXT, GL_UNSIGNED_BYTE, PixelData);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, ImageWidth, ImageHeight,
		                               GL_BGR_EXT, GL_UNSIGNED_BYTE, PixelData);

	// delete the pixel data
	free(PixelData);
}

GLuint TextrueHandler::getTextureHandle() const
{
	return textureHandle;
}

TextrueHandler::~TextrueHandler()
{
}
