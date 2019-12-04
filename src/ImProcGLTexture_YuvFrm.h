#ifndef _IMPROCGLTEXTURE_YUVFRM_H_
#define _IMPROCGLTEXTURE_YUVFRM_H_

#include "ImProcGLTexture.h"

namespace ImProc
{

class GLTexture_CamYuvFrame : public GLTexture
{
public:
	GLTexture_CamYuvFrame(GLuint iw, GLuint ih)
		: nYFrameWidth(iw), 
		  nYFrameHeight(ih), 
		  texY(NULL),
		  nUVFrameWidth(iw/2),
		  nUVFrameHeight(ih/2),
		  texUV(NULL)
	{

		texY = new GLTextureUnit(GL_TEXTURE0);
		
		texUV = new GLTextureUnit(GL_TEXTURE1);
	}

 	virtual void Update(GLubyte* iframe)
 	{
 		if(texY)
 			texY->Load(iframe, nYFrameWidth, nYFrameHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE);

 		if(texUV)
 			texUV->Load(iframe+(nYFrameWidth*nYFrameHeight), 
 					nUVFrameWidth, nUVFrameHeight, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE);
 	}

	~GLTexture_CamYuvFrame()
	{
		if(NULL!=texY)
			delete texY;

		if(NULL!=texUV)
			delete texUV;
	}

protected:
	GLuint 				nYFrameWidth;
	GLuint 				nYFrameHeight;
	GLTextureUnit*		texY;

protected:
	GLuint				nUVFrameWidth;
	GLuint				nUVFrameHeight;
	GLTextureUnit* 		texUV;
};

}; //end namespace ImProc


#endif //_IMPROCGLTEXTURE_YUVFRM_H_
