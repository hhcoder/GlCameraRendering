#ifndef _IMPROCGL_RGBNORMAL_H_
#define _IMPROCGL_RGBNORMAL_H_

#include "ImProcGLTexture_RgbFrm.h"
#include "ImProcGLSL_RgbNone.h"


namespace ImProc
{

class ImProcGLRgbNormal : public ProcGLBaseRGB
{
public:
	ImProcGLRgbNormal()
		: ProcGLBaseRGB("GL RGB Normal")
	{
	}

	virtual unsigned int InitOperations( void )
	{ 
		return AssignOperations(
					new ImProc::GLFbo(GetFrameWidth(), GetFrameHeight(), GL_RGB, GL_UNSIGNED_BYTE),
					new ImProc::GLVertex_FlatSurface( ),
					new ImProc::GLTexture_CamRgbFrame(GetFrameWidth(), GetFrameHeight()),
					new ImProc::GLShader_RgbNone() );
	}
};

}


#endif //_IMPROCGL_RGBNORMAL_H_
