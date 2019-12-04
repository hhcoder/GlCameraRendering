#ifndef _IMPROCGLSL_RGBNONE_H_
#define _IMPROCGLSL_RGBNONE_H_

#include "ImProcGLSL.h"

namespace ImProc
{

static const char vertexShaderSrc_None[] =
	"attribute vec4 g_vVertex;\n"
	"varying vec2 g_vTexCoords;\n"
	"void main()\n"
	"{\n"
	"  gl_Position  = vec4( g_vVertex.x, -g_vVertex.y, 0.0, 1.0 );\n"
	"  g_vTexCoords = vec2( g_vVertex.z, g_vVertex.w );\n"
	"}\n";

static const char fragmentShaderSrc_None[] = 
	"precision mediump float;\n"
	"uniform sampler2D g_sImageTexture;\n"
	"varying vec2 g_vTexCoords;\n"
	"void main()\n"
	"{\n"
	"  gl_FragColor = texture2D(g_sImageTexture, g_vTexCoords);\n"
	"}\n";

class GLShader_RgbNone : public GLShader
{
public:
	GLShader_RgbNone(void)
		: GLShader("No Effect", vertexShaderSrc_None, fragmentShaderSrc_None),
		  sTexLocRGB(NULL)
	{
		Initialize();
	}

	GLShader_RgbNone(const char* iname, const char* iStrVertex, const char* iStrFrag)
		: GLShader(iname, iStrVertex, iStrFrag)
	{
		Initialize();
	}

	virtual void Update(void) 
	{
		UpdateTextureUniforms();
	}

	~GLShader_RgbNone(void)
	{
		DeInitialize();
	}

public:
	void UpdateTextureUniforms(void)
	{
		sTexLocRGB->Set(0);
	}

private:
	void Initialize()
	{
		// Setup channel to fragment shader
		sTexLocRGB = new GLSLVarUniform1i(programObject, "g_sImageTexture");

		UpdateTextureUniforms();
	}

	void DeInitialize()
	{
		if(NULL!=sTexLocRGB)
			delete sTexLocRGB;
	}

protected:
	GLSLVarUniform1i* sTexLocRGB;
};

}//end namespace ImProc;

#endif //_IMPROCGLSL_RGBNONE_H_
