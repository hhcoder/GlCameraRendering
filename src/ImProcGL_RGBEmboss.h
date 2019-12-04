#ifndef _IMPROCGL_RGBEMBOSS_H_
#define _IMPROCGL_RGBEMBOSS_H_

#include "ImProcGLTexture_RgbFrm.h"
#include "ImProcGLSL.h"

namespace ImProc
{

static const char vertexShaderSrc_Emboss[] =
	"uniform float g_fHalfStepSizeX;\n"
	"uniform float g_fHalfStepSizeY;\n"
	"attribute vec4 g_vVertex;\n"
	"varying vec2 g_vTexCoords0;\n"
	"varying vec2 g_vTexCoords1;\n"
	"void main()\n"
	"{\n"
	"  gl_Position  = vec4( g_vVertex.x, -g_vVertex.y, 0.0, 1.0 );\n"
	"  vec2 vTexCoords = vec2( g_vVertex.z, g_vVertex.w ) + 2.0 * vec2( g_fHalfStepSizeX, g_fHalfStepSizeY );\n"
	"  g_vTexCoords0 = vTexCoords - vec2( g_fHalfStepSizeX, g_fHalfStepSizeY );\n"
	"  g_vTexCoords1 = vTexCoords + vec2( g_fHalfStepSizeX, g_fHalfStepSizeY );\n"
	"}\n";

static const char fragmentShaderSrc_Emboss[] = 
	"precision mediump float;\n"
	"uniform sampler2D g_sImageTexture;\n"
	"varying vec2 g_vTexCoords0;\n"
	"varying vec2 g_vTexCoords1;\n"
	"void main()\n"
	"{\n"
	"  vec4 vEmboss = 4.0 * ( texture2D( g_sImageTexture, g_vTexCoords0 ) - texture2D( g_sImageTexture, g_vTexCoords1 ) );\n"
	"  gl_FragColor = vEmboss + 0.5;\n"
	"}\n";

class GLShader_Emboss : public GLShader
{
public:
	GLShader_Emboss(void)
		: 	GLShader("Emboss", vertexShaderSrc_Emboss, fragmentShaderSrc_Emboss), 
			fHalfStepSize(1.0f / 1024.0f),
			uniformXLoc(programObject, "g_fHalfStepSizeX"),
			uniformYLoc(programObject, "g_fHalfStepSizeY")
			
	{
	}

	virtual void Update(void)
	{
		uniformXLoc.Set(fHalfStepSize);
		uniformYLoc.Set(fHalfStepSize);
	}

	~GLShader_Emboss()
	{
	}

private:
	float fHalfStepSize;
	GLint stepSizeXLoc;
	GLint stepSizeYLoc;

	GLSLVarUniform1f uniformXLoc;
	GLSLVarUniform1f uniformYLoc;
};

};	//end namespace ImProc

namespace ImProc
{

class ImProcGLRgbEmboss : public ProcGLBaseRGB
{
public:
	ImProcGLRgbEmboss()
		: ProcGLBaseRGB("GL RGB Emboss")
	{
	}

	virtual unsigned int InitOperations( void )
	{ 
		return AssignOperations(
					new ImProc::GLFbo(GetFrameWidth(), GetFrameHeight(), GL_RGB, GL_UNSIGNED_BYTE),
					new ImProc::GLVertex_FlatSurface( ),
					new ImProc::GLTexture_CamRgbFrame(GetFrameWidth(), GetFrameHeight()),
					new ImProc::GLShader_Emboss());
 	}

};

}
#endif //_IMPROCGL_RGBEMBOSS_H_

