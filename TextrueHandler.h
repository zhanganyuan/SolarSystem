#pragma once
#include <GL/glut.h>

class TextrueHandler
{
	GLuint textureHandle;
public:
	TextrueHandler(char* fileNmae);
	~TextrueHandler();
	GLuint getTextureHandle() const;
};

