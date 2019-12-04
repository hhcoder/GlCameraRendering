#ifndef _IMPROCGL_YUVNORMAL_H_
#define _IMPROCGL_YUVNORMAL_H_

#include "ImProcGLTexture_YuvFrm.h"
#include "ImProcGLSL_YuvNone.h"

namespace ImProc
{

class ImProcGLYuvNormal : public ProcGLBaseYUV
{
public:
	ImProcGLYuvNormal()
		: ProcGLBaseYUV("GL YUV Normal")
	{
	}
		
	virtual unsigned int InitOperations( void )
	{ 
		return AssignOperations(
					new ImProc::GLFbo(GetFrameWidth(), GetFrameHeight(), GL_RGB, GL_UNSIGNED_BYTE),
					new ImProc::GLVertex_FlatSurface( ),
					new ImProc::GLTexture_CamYuvFrame(GetFrameWidth(), GetFrameHeight()),
					new ImProc::GLShader_YuvNone());
	}	
};

}
#endif //_IMPROCGL_RGBCUTOUT_H_

