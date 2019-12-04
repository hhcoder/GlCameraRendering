#ifndef _IMPROCGL_RGBCUTOUT_H_
#define _IMPROCGL_RGBCUTOUT_H_

#include "ImProcGLTexture_RgbFrm.h"

#include "ImProcGLSL_RgbNone.h"

namespace ImProc
{
static const char vertexShaderSrc_CutOut[] =
	"attribute vec4 g_vVertex;\n"
	"varying vec2 g_vTexCoords;\n"
	"void main()\n"
	"{\n"
	"  gl_Position  = vec4( g_vVertex.x, -g_vVertex.y, 0.0, 1.0 );\n"
	"  g_vTexCoords = vec2( g_vVertex.z, g_vVertex.w );\n"
	"}\n";

static const char fragmentShaderSrc_CutOut[] = 
	"precision mediump float;\n"
	"uniform sampler2D g_sImageTexture;\n"
	"varying vec2 g_vTexCoords;\n"
	"void main()\n"
	"{\n"
	"  vec4 color = texture2D(g_sImageTexture, g_vTexCoords);\n"
	"  vec4 scalar = vec4(4.0,4.0,4.0,4.0);\n"
	"  gl_FragColor = ceil(color * scalar)/scalar;\n"
	"}\n";

class GLShader_CutOut : public GLShader_RgbNone
{
public:
	GLShader_CutOut(void)
		: GLShader_RgbNone("Cut Out", vertexShaderSrc_CutOut, fragmentShaderSrc_CutOut)
	{
	}
};

};//end namespace ImProc

namespace ImProc
{

class ImProcGLRgbCutout : public ProcGLBaseRGB
{
public:
	ImProcGLRgbCutout()
		: ProcGLBaseRGB("GL RGB CutOut")
	{
	}

	virtual unsigned int InitOperations( void )
	{ 
		return AssignOperations(
					new ImProc::GLFbo(GetFrameWidth(), GetFrameHeight(), GL_RGB, GL_UNSIGNED_BYTE),
					new ImProc::GLVertex_FlatSurface(),
					new ImProc::GLTexture_CamRgbFrame(GetFrameWidth(), GetFrameHeight()),
					new ImProc::GLShader_CutOut() );
	}

};

}
#endif //_IMPROCGL_RGBCUTOUT_H_
