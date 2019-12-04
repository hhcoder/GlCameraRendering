#ifndef _IMPROCGLTEXTURE_H_
#define _IMPROCGLTEXTURE_H_

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace ImProc
{

class GLTextureUnit
{
public:
	GLTextureUnit(GLenum idShader)
		: textureId(0),
		  textureIdShader(idShader)
	{
		glGenTextures(1, &textureId);

		glBindTexture(GL_TEXTURE_2D, textureId);
		SetUpInterpolation();
	}

    // Set up the interpolation method (overridable)
	virtual void SetUpInterpolation(void)
	{	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_MIRRORED_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_MIRRORED_REPEAT);
	}

	virtual void Load(GLubyte* iSrc, GLuint iWidth, GLuint iHeight, GLint iColorFormat, GLenum iByteType)
	{
		glActiveTexture(textureIdShader);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, iColorFormat, iWidth, iHeight, 0, iColorFormat,
			iByteType, iSrc);
	}

	void UnLoad(void)
	{
		glActiveTexture(textureIdShader);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}
	
	virtual ~GLTextureUnit(void)
	{
		UnLoad();

		if(0!=textureId)
			glDeleteTextures(1, &textureId);
	}

private:
	GLuint			textureId;
	GLenum			textureIdShader;
};

class GLTexture
{
public:
	GLTexture()	{ }

 	virtual void Update(GLubyte* iframe) = 0;

	virtual ~GLTexture() { }

};
}; //end namespace ImProc


#endif //_IMPROCGLTEXTURE_H_
