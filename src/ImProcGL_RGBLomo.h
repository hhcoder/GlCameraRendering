#ifndef _IMPROCGL_RGBLOMO_H_
#define _IMPROCGL_RGBLOMO_H_

#include "ImProcGLTexture_RgbFrm.h"

#include "ImProcGLSL_RgbNone.h"

namespace ImProc
{

static const char vertexShaderSrc_Lomo[] =
	"attribute vec4 g_vVertex;\n"
	"varying vec2 g_vTexCoords;\n"
	"void main()\n"
	"{\n"
	"  gl_Position  = vec4( g_vVertex.x, -g_vVertex.y, 0.0, 1.0 );\n"
	"  g_vTexCoords = vec2( g_vVertex.z, g_vVertex.w );\n"
	"}\n";

static const char fragmentShaderSrc_Lomo[] = 
	"precision mediump float;\n"
	"uniform sampler2D g_sImageTexture;\n"
	"varying vec2 g_vTexCoords;\n"
	"const vec2 g_vLensCenters = vec2(0.5, 0.5);\n"
	"const float fPi_2 = 1.57079;\n"
	" vec4 Tone(vec4 v)\n"
	"{\n"
	"float s = 0.2;\n"
	"float d = 2.8;\n"
    "vec4 vPow = pow(v,vec4(d,d,d,d));\n"
    "return vPow/(vec4(s,s,s,s)+vPow);\n"
	"}\n"
	"void main()\n"
	"{\n"
	"  vec4 vColor = texture2D(g_sImageTexture, g_vTexCoords);\n"
	"  vColor = Tone(vColor);\n"
	"  float fDist = distance(g_vLensCenters, g_vTexCoords);\n"
	"  float fDark = pow(cos(fDist*fPi_2), 2.0);\n"
	"  gl_FragColor = vColor*fDark;\n"
	"}\n";

class GLShader_Lomo : public GLShader_RgbNone
{
public:
	GLShader_Lomo(void)
		: GLShader_RgbNone("Lomo", vertexShaderSrc_Lomo, fragmentShaderSrc_Lomo)
	{}
};

}//end namespace ImProc;

namespace ImProc
{

class ImProcGLRgbLomo : public ProcGLBaseRGB
{
public:
	ImProcGLRgbLomo()
		: ProcGLBaseRGB("GL RGB Lomo")
	{
	}

	virtual unsigned int InitOperations( void )
	{ 
		return AssignOperations(
					new ImProc::GLFbo(GetFrameWidth(), GetFrameHeight(), GL_RGB, GL_UNSIGNED_BYTE),
					new ImProc::GLVertex_FlatSurface( ),
					new ImProc::GLTexture_CamRgbFrame(GetFrameWidth(), GetFrameHeight()),
					new ImProc::GLShader_Lomo());
	}
};

}

#endif //_IMPROCGL_RGBLOMO_H_
