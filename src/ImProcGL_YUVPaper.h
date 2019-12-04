#ifndef _IMPROCGL_YUVPAPER_H_
#define _IMPROCGL_YUVPAPER_H_

#include "ImProcGLTexture_YuvFrm.h"
#include "ImProcGLSL_YuvNone.h"

namespace ImProc
{

static const char vertexShaderSrc_YuvLomo[] =
	"attribute vec4 g_vVertex;\n"
	"varying vec2 g_vTexCoords;\n"
	"void main()\n"
	"{\n"
	"  gl_Position  = vec4( g_vVertex.x, -g_vVertex.y, 0.0, 1.0 );\n"
	"  g_vTexCoords = vec2( g_vVertex.z, g_vVertex.w );\n"
	"}\n";

static const char fragmentShaderSrc_YuvLomo[] = 
	"precision mediump float;\n"
	"uniform sampler2D g_sImageTextureY;\n"
	"uniform sampler2D g_sImageTextureUV;\n"
	"uniform sampler2D g_sImageTextureMask0;\n"
	"uniform sampler2D g_sImageTextureMask1;\n"
	"uniform sampler2D g_sImageTextureMask2;\n"
	"uniform sampler2D g_sImageTextureMask3;\n"
	"varying vec2 g_vTexCoords;\n"
	"void main()\n"
	"{\n"
	"  vec4 yyy = texture2D(g_sImageTextureY, g_vTexCoords);\n"
	"  vec4 uv = texture2D(g_sImageTextureUV, g_vTexCoords);\n"
	"  vec4 m0 = texture2D(g_sImageTextureMask0, g_vTexCoords);\n"
	"  vec4 m1 = texture2D(g_sImageTextureMask1, g_vTexCoords);\n"
	"  vec4 m2 = texture2D(g_sImageTextureMask2, g_vTexCoords);\n"
	"  vec4 m3 = texture2D(g_sImageTextureMask3, g_vTexCoords);\n"
	"  if (yyy.g<0.25)\n"
	"  { \n"
	"      gl_FragColor = vec4(m4.g, 0.5, 0.5, 1.0); \n"
	"  } \n"
	"  else\n"
	"  { \n"
	"      if(yyy.g<0.5)\n"
	"      {\n"
	"      	   gl_FragColor = vec4(m3.g, 0.5, 0.5, 1.0); \n"
	"      }\n"
	"      else\n"
	"      {\n"
	"          if(yyy.g<0.75)\n"
	"          {\n"
	"      	       gl_FragColor = vec4(m2.g, 0.5, 0.5, 1.0); \n"
	"          }\n"
	"          else\n"
	"          {\n"
	"      	       gl_FragColor = vec4(m1.g, 0.5, 0.5, 1.0); \n"
	"          }\n"
	"      }\n"
	"  } \n"
	"      gl_FragColor = vec4(y*m.g, u, v, 1.0); \n"
	"}\n";

class GLShader_YuvPaper : public GLShader_YuvNone
{
public:
	GLShader_YuvPaper(void)
		: GLShader_YuvNone("YUV Lomo", vertexShaderSrc_YuvLomo, fragmentShaderSrc_YuvLomo),
		  sTexLocMask0(NULL),
		  sTexLocMask1(NULL),
		  sTexLocMask2(NULL),
		  sTexLocMask3(NULL)
	{
		sTexLocMask0 = new GLSLVarUniform1i(programObject, "g_sImageTextureMask0");		
		sTexLocMask1 = new GLSLVarUniform1i(programObject, "g_sImageTextureMask1");		
		sTexLocMask2 = new GLSLVarUniform1i(programObject, "g_sImageTextureMask2");		
		sTexLocMask3 = new GLSLVarUniform1i(programObject, "g_sImageTextureMask3");		
	}

	virtual void Update(void)
	{
		UpdateTextureUniforms();

		sTexLocMask0->Set(2);
		sTexLocMask1->Set(3);
		sTexLocMask2->Set(4);
		sTexLocMask3->Set(5);
	}

	~GLShader_YuvLomo(void)
	{
		if(NULL!=sTexLocMask0)
			delete sTexLocMask0;
		if(NULL!=sTexLocMask1)
			delete sTexLocMask1;
		if(NULL!=sTexLocMask2)
			delete sTexLocMask2;
		if(NULL!=sTexLocMask3)
			delete sTexLocMask3;
	}

private:
	GLSLVarUniform1i* sTexLocMask0;
	GLSLVarUniform1i* sTexLocMask1;
	GLSLVarUniform1i* sTexLocMask2;
	GLSLVarUniform1i* sTexLocMask3;
};

}

namespace ImProc
{

class GLTexture_CamYuvPaper : public GLTexture_CamYuvFrame 
{
public:
	GLTexture_CamYuvPaper(GLuint iw, GLuint ih)
		: GLTexture_CamYuvFrame(iw, ih),
		  texMask0(NULL),
		  texMask1(NULL),
		  texMask2(NULL),
		  texMask3(NULL),
		  bufMask(NULL)
	{
		texMask0 = new GLTextureUnit(GL_TEXTURE2);
		texMask1 = new GLTextureUnit(GL_TEXTURE2);
		texMask2 = new GLTextureUnit(GL_TEXTURE2);
		texMask3 = new GLTextureUnit(GL_TEXTURE2);

		LoadTexMask(texMask0, "/data/sketch0.yuv", 640, 480);
		LoadTexMask(texMask1, "/data/sketch1.yuv", 640, 480);
		LoadTexMask(texMask2, "/data/sketch2.yuv", 640, 480);
		LoadTexMask(texMask3, "/data/sketch3.yuv", 640, 480);
	}

	~GLTexture_CamYuvLomo()
	{
		if(NULL!=texMask0)
			delete texMask0;
		if(NULL!=texMask1)
			delete texMask1;
		if(NULL!=texMask2)
			delete texMask2;
		if(NULL!=texMask3)
			delete texMask3;
	}

private:
	void LoadTexMask(GLTextureUnit* texMask, const char* ifname, const unsigned int ifwidth, const unsigned int ifheight)
	{
		bufMask = new GLubyte[ifwidth*ifheight];

		FILE* fp = fopen(ifname, "rb");
		if(fp!=0)
			fread(bufMask, sizeof(GLubyte), ifwidth*ifheight, fp);
		else
			IMPROC_LOGE("Error opening %s", ifname);
		fclose(fp);

		texMask->Load(bufMask, ifwidth, ifheight, GL_LUMINANCE, GL_UNSIGNED_BYTE);

		if(NULL!=bufMask)
			delete[] bufMask;
	}

private:
	GLTextureUnit*		texMask0;
	GLTextureUint* 		texMask1;
	GLTextureUint* 		texMask2;
	GLTextureUint* 		texMask3;
	GLubyte* 			bufMask;
};

}

namespace ImProc
{

class ImProcGLYuvLomo : public ProcGLBaseYUV
{
public:
	ImProcGLYuvLomo()
		: ProcGLBaseYUV("GL YUV Lomo")
	{
	}

	virtual unsigned int InitOperations( void )
	{ 
		return AssignOperations(
					new ImProc::GLFbo(GetFrameWidth(), GetFrameHeight(), GL_RGB, GL_UNSIGNED_BYTE),
					new ImProc::GLVertex_FlatSurface( ),
					new ImProc::GLTexture_CamYuvLomo(GetFrameWidth(), GetFrameHeight()),
					new ImProc::GLShader_YuvLomo() );
	}
};

}

#endif //_IMPROCGL_YUVPAPER_H_
