#ifndef _IMPROCGL_YUVLOMO_H_
#define _IMPROCGL_YUVLOMO_H_

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
	"uniform sampler2D g_sImageTextureMask;\n"
	"varying vec2 g_vTexCoords;\n"
	"void main()\n"
	"{\n"
	"  vec4 yyy = texture2D(g_sImageTextureY, g_vTexCoords);\n"
	"  vec4 uv = texture2D(g_sImageTextureUV, g_vTexCoords);\n"
	"  vec4 m = texture2D(g_sImageTextureMask, g_vTexCoords);\n"
	"  float y = smoothstep(0.15, 0.85, yyy.g);\n"
	"  float u = smoothstep(0.05, 0.95,  uv.g);\n"
	"  float v = smoothstep(0.05, 0.80,  uv.a);\n"
	"  gl_FragColor = vec4(y*m.g, u, v, 1.0); \n"
	"}\n";

class GLShader_YuvLomo : public GLShader_YuvNone
{
public:
	GLShader_YuvLomo(void)
		: GLShader_YuvNone("YUV Lomo", vertexShaderSrc_YuvLomo, fragmentShaderSrc_YuvLomo),
		  sTexLocMask(NULL)
	{
		sTexLocMask = new GLSLVarUniform1i(programObject, "g_sImageTextureMask");		
	}

	virtual void Update(void)
	{
		UpdateTextureUniforms();

		sTexLocMask->Set(2);
	}

	~GLShader_YuvLomo(void)
	{
		if(NULL!=sTexLocMask)
			delete sTexLocMask;
	}

private:
	GLSLVarUniform1i* sTexLocMask;
};

}

namespace ImProc
{

class GLTexture_CamYuvLomo : public GLTexture_CamYuvFrame 
{
public:
	GLTexture_CamYuvLomo(GLuint iw, GLuint ih)
		: GLTexture_CamYuvFrame(iw, ih),
		  texMask(NULL),
		  bufMask(NULL)
	{
		texMask = new GLTextureUnit(GL_TEXTURE2);

		LoadTexMask("/data/lomo_mask.yyy", 640, 480);
	}

	~GLTexture_CamYuvLomo()
	{
		if(NULL!=texMask)
			delete texMask;
	}

private:
	void LoadTexMask(const char* ifname, const unsigned int ifwidth, const unsigned int ifheight)
	{
		//TODO: make sure this 640x480 is removed!
		bufMask = new GLubyte[ifwidth*ifheight];

		FILE* fp = fopen(ifname, "rb");
		if(fp!=0)
		{
			fread(bufMask, sizeof(GLubyte), ifwidth*ifheight, fp);
		}
		else
		{
			IMPROC_LOGE("Error opening lomo_mask.yyy");
		}
		fclose(fp);

		texMask->Load(bufMask, ifwidth, ifheight, GL_LUMINANCE, GL_UNSIGNED_BYTE);

		if(NULL!=bufMask)
			delete[] bufMask;
	}

private:
	GLTextureUnit*		texMask;
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

#endif //_IMPROCGL_RGBCUTOUT_H_


