#ifndef _IMPROCGLTEXTURE_RGBFRM_H_
#define _IMPROCGLTEXTURE_RGBFRM_H_

#include "ImProcGLTexture.h"

namespace ImProc
{

class GLTexture_CamRgbFrame : public GLTexture
{
public:
	GLTexture_CamRgbFrame(GLuint iw, GLuint ih)
		: nFrameWidth(iw), 
		  nFrameHeight(ih), 
		  texRGB(NULL)
	{
		IMPROC_LOGI("Constructor: GLTexture_CamRgbFrame");

	    texRGB = new GLTextureUnit(GL_TEXTURE0);
	}
	
 	virtual void Update(GLubyte* iframe)
 	{
		if(NULL!=texRGB)
			texRGB->Load(iframe, nFrameWidth, nFrameHeight, GL_RGB, GL_UNSIGNED_BYTE);
 	}

	~GLTexture_CamRgbFrame()
	{
		if(NULL!=texRGB)
			delete texRGB;
	}

protected:
	GLuint 				nFrameWidth;
	GLuint 				nFrameHeight;
	GLTextureUnit*		texRGB;
};

}; //end namespace ImProc


#endif //_IMPROCGLTEXTURE_RGBFRM_H_
