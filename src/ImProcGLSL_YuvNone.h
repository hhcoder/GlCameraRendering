#ifndef _IMPROCGLSL_YUVNONE_H_
#define _IMPROCGLSL_YUVNONE_H_

#include "ImProcGLSL.h"

namespace ImProc
{

static const char vertexShaderSrc_YuvNone[] =
	"attribute vec4 g_vVertex;\n"
	"varying vec2 g_vTexCoords;\n"
	"void main()\n"
	"{\n"
	"  gl_Position  = vec4( g_vVertex.x, -g_vVertex.y, 0.0, 1.0 );\n"
	"  g_vTexCoords = vec2( g_vVertex.z, g_vVertex.w );\n"
	"}\n";

static const char fragmentShaderSrc_YuvNone[] = 
	"precision mediump float;\n"
	"uniform sampler2D g_sImageTextureY;\n"
	"uniform sampler2D g_sImageTextureUV;\n"
	"varying vec2 g_vTexCoords;\n"
	"void main()\n"
	"{\n"
	"  vec4 yyy = texture2D(g_sImageTextureY, g_vTexCoords);\n"
	"  vec4 uv = texture2D(g_sImageTextureUV, g_vTexCoords);\n"
	"  vec4 color = vec4(yyy.g, uv.g, uv.a, 1.0); \n"
	"  gl_FragColor = color; \n"
	"}\n";

class GLShader_YuvNone : public GLShader
{
public:
	GLShader_YuvNone(void)
		: GLShader("No Effect", vertexShaderSrc_YuvNone, fragmentShaderSrc_YuvNone),
  		  sTexLocY(NULL),
		  sTexLocUV(NULL)
	{
		Initialize();
	}

	GLShader_YuvNone(const char* iname, const char* iStrVertex, const char* iStrFrag)
		: GLShader(iname, iStrVertex, iStrFrag),
  		  sTexLocY(NULL),
		  sTexLocUV(NULL)
	{
		Initialize();
	}

	virtual void Update(void)
	{
		UpdateTextureUniforms();
	}


	~GLShader_YuvNone(void)
	{
		DeInitialize();
	}

public:
	void UpdateTextureUniforms(void)
	{
		sTexLocY->Set(0);
		sTexLocUV->Set(1);
	}

private:
	void Initialize()
	{
		sTexLocUV = new GLSLVarUniform1i(programObject, "g_sImageTextureUV");
		sTexLocY = new GLSLVarUniform1i(programObject, "g_sImageTextureY");

		UpdateTextureUniforms();
	}

	void DeInitialize()
	{
		if(NULL!=sTexLocY)
			delete sTexLocY;
		if(NULL!=sTexLocUV)
			delete sTexLocUV;
	}

protected:
	GLSLVarUniform1i* sTexLocY;
	GLSLVarUniform1i* sTexLocUV;

};

}


#endif //_IMPROCGLSL_YUVNONE_H_
