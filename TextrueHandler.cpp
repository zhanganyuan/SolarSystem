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
	// ���ļ�
	FILE* pFile;
	char buffer[36];
	snprintf(buffer, sizeof(buffer),"images/%s", fileName);
	std::cout << buffer << std::endl;
	fopen_s(&pFile, buffer, "rb");
	if (pFile == nullptr)
		exit(0);

	// ��ȡͼ��Ĵ�С��Ϣ
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&ImageWidth, sizeof(ImageWidth), 1, pFile);
	fread(&ImageHeight, sizeof(ImageHeight), 1, pFile);

	// �����������ݳ���
	GLint PixelLength = ImageWidth * 3;
	while (PixelLength % 4 != 0)
		++PixelLength;
	PixelLength *= ImageHeight;

	// ��ȡ��������
	GLubyte* PixelData = static_cast<GLubyte*>(malloc(PixelLength));
	if (PixelData == nullptr)
		exit(0);

	fseek(pFile, 54, SEEK_SET);
	fread(PixelData, PixelLength, 1, pFile);

	// �ر��ļ�
	fclose(pFile);
	// make a gl texture
	//glGenTextures(GLsizei n, GLuint *textures)
	//n�������������������
	//textures���洢���������ĵ�һ��Ԫ��ָ��
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
