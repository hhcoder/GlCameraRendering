#ifndef _IMPROCGL_RGBLENS_H_
#define _IMPROCGL_RGBLENS_H_

#include "ImProcGL.h"

#include "ImProcGLTexture_RgbFrm.h"
#include "ImProcGLSL_RgbNone.h"

namespace ImProc
{

static const char vertexShaderSrc_Lens[] =
	"attribute vec4 g_vVertex;\n"
	"uniform vec2 g_vPosition;\n"
	"varying vec2 g_vTexCoords;\n"
	"varying vec3 g_vMidCoords;\n"
	"void main()\n"
	"{\n"
	"  gl_Position  = vec4( g_vVertex.x, -g_vVertex.y, 0.0, 1.0 );\n"
	"  g_vTexCoords = vec2( g_vVertex.z, g_vVertex.w );\n"
	"  g_vMidCoords = vec3( 1.8 * ( 2.0 * vec2( g_vVertex.z, g_vVertex.w ) - 1.0 + g_vPosition ), 0.5 );\n"
	"}\n";

static const char fragmentShaderSrc_Lens[] =
    "precision mediump float;\n"
    "uniform sampler2D g_sImageTexture;\n"
	"varying vec2 g_vTexCoords;\n"
	"varying vec3 g_vMidCoords;\n"
	"void main()\n"
	"{\n"
	"  float off = 0.1 * exp2( -dot( g_vMidCoords.xy, g_vMidCoords.xy ) );\n"
	"  gl_FragColor = texture2D( g_sImageTexture, g_vTexCoords - off * normalize( g_vMidCoords).xy );\n"
	"}\n";

class GLShader_Lens : public GLShader_RgbNone
{

private:
	void UpdateLoc(void)
	{
		GLfloat fRate;

		fRate = GetEllipseTime();
		lensLoc.Set(0.6f * cosf( fRate ), 0.6f * sinf( fRate ));
	}

public:
	GLShader_Lens(void)
		: GLShader_RgbNone("Lens", vertexShaderSrc_Lens, fragmentShaderSrc_Lens),
		  lensLoc(programObject, "g_vPosition")
	{
		IMPROC_LOGI("Constructor: GLShader_Lens");
		UpdateLoc();
	}

	virtual void Update(void)
	{
		UpdateLoc();
	}


private:
	GLSLVarUniform2fv lensLoc;
};

}


namespace ImProc
{

class ImProcGLRgbLens : public ProcGLBaseRGB
{
public:
	ImProcGLRgbLens()
		: ProcGLBaseRGB("GL RGB Lens")
	{
		IMPROC_LOGI("Constructor: ImProcGLRgbLens");
	}

	virtual unsigned int InitOperations( void )
	{ 
		return AssignOperations(
					new ImProc::GLFbo(GetFrameWidth(), GetFrameHeight(), GL_RGB, GL_UNSIGNED_BYTE),
					new ImProc::GLVertex_FlatSurface( ),
					new ImProc::GLTexture_CamRgbFrame(GetFrameWidth(), GetFrameHeight()),
					new ImProc::GLShader_Lens());
	}
};

}
#endif //_IMPROCGL_RGBLENS_H_

