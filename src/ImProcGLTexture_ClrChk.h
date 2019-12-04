#ifndef _IMPROCGLTEXTURE_CLRCHK_H_
#define _IMPROCGLTEXTURE_CLRCHK_H_

#include "ImProcGLTexture.h"

#include "ImProcUtilColorChecker.h"

namespace ImProc
{

class GLTexture_ColorChecker : public GLTexture
{
public:
	GLTexture_ColorChecker(GLuint iw, GLuint ih)
		: nFrameWidth(iw), 
		  nFrameHeight(ih), 
		  textureId(0),
		  pCC(NULL)
	{
		pCC = new img_rgb(nFrameWidth, nFrameHeight);

		glGenTextures(1, &textureId);

		glBindTexture(GL_TEXTURE_2D, textureId);
	}

	virtual void Update(GLubyte* iframe)
	{
		GLubyte* pixels = pCC->get_buf();

		ActiveTexture0();

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, nFrameWidth, nFrameHeight, 0, GL_RGB,
			GL_UNSIGNED_BYTE, pixels);
	}

	~GLTexture_ColorChecker()
	{
		if(NULL!=pCC)
			delete pCC;

		if(0!=textureId)
			glDeleteTextures(1, &textureId);
	}

private:
	void ActiveTexture0(void)
	{
		// Set up the interpolation method
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_MIRRORED_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_MIRRORED_REPEAT);

		// Active this texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);
	}

private:
	GLuint textureId;
	GLuint nFrameWidth;
	GLuint nFrameHeight;

private:
	img_rgb*	pCC;
};//end namespace ImProc

};


#endif
