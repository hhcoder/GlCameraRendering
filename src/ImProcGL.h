#ifndef _IMPROCGLPROC_H_
#define _IMPROCGLPROC_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ImProcBase.h"

#include "ImProcGLVertex.h"
#include "ImProcGLFBO.h"
#include "ImProcGLSL.h"
#include "ImProcGLTexture.h"

namespace ImProc
{

class ProcGLBase : public ImProcBase
{
public:
	ProcGLBase(const char* iName)
		: 	ImProcBase(iName),
			rgbFrame(NULL),
			pShader(NULL),
			pTexture(NULL),
			pVertex(NULL),
			pFbo(NULL)

	{
	}

	virtual unsigned int Init(
		unsigned int iFrameW, 
		unsigned int iFrameH )
	{
		InitProperties(iFrameW, iFrameH);

		InitEGL(iFrameW, iFrameH);
	
	    printGLString("Version", GL_VERSION);
	    printGLString("Vendor", GL_VENDOR);
	    printGLString("Renderer", GL_RENDERER);
	    printGLString("Extensions", GL_EXTENSIONS);

		InitOperations();

		InitBufs(iFrameW, iFrameH);

		if(NULL!=pFbo)
			pFbo->Begin();

		//TODO: errors for returning
		return IMPROC_SUCCESS;
	}

	virtual unsigned int InitBufs(unsigned int iFrameW, unsigned int iFrameH)
	{
		return InitRgbBuffer(iFrameW, iFrameH);
	}

	virtual unsigned int DeInitBufs(void)
	{
		return DeInitRgbBuffer();
	}

	virtual unsigned int DeInitOperations(void)
	{
		if(NULL!=pShader) 
		{
			delete pShader;
			pShader = NULL;
		}

		if(NULL!=pTexture)
		{
			delete pTexture;
			pTexture = NULL;
		}

		if(NULL!=pVertex)
		{
			delete pVertex;
			pVertex = NULL;
		}

		if(NULL!=pFbo)
		{
			pFbo->End();	//Is this End necessary?
			delete pFbo;
			pFbo = NULL;
		}

		return IMPROC_SUCCESS;
	}

public:
	unsigned int AssignOperations(
		GLFbo* iFbo,
		GLVertex* iVertex,
		GLTexture* iTexture,
		GLShader* iShader )
	{
		pFbo = iFbo;
		pVertex = iVertex;
		pTexture = iTexture;
		pShader = iShader;

		return IMPROC_SUCCESS;
	}

	void GLUpdate(unsigned char* iframe)
	{
		// Set the view port
	    glViewport(0, 0, GetFrameWidth(), GetFrameHeight());

		// Clear the color buffer
	    glClear( GL_COLOR_BUFFER_BIT );

		//Example code of multi-texturing:
		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, baseMapTexID);
		// glUniform1l(baseMapLoc, 0);
		// glActiveTexture(GL_TEXTURE1);
		// glBindTexture(GL_TEXTURE_2D, lightMapTexID);
		// glUniform1i(lightMapLoc, 1);

		// So, the update sequence (texture->shader->vertex) matters?

		//TODO: check for NULL on texture, shader, vertex
		// Texture update
		pTexture->Update((GLubyte*)iframe);

		// Shader update
		pShader->UseProgram();
		pShader->Update();

		// Vertex update
		pVertex->Update();
	}

	void ReadFbo(GLubyte* oframe)
	{
		GLint readType, readFormat;
		glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_TYPE, &readType);
		glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_FORMAT, &readFormat);

		glReadPixels(0, 0, GetFrameWidth(), GetFrameHeight(), (GLenum)readFormat, (GLenum)readType, oframe);
	}

private:
	unsigned int InitRgbBuffer(unsigned int iFrameW, unsigned int iFrameH)
	{
		if(NULL==rgbFrame)
		{
			rgbFrame = new GLubyte[iFrameW*iFrameH*3];
			if(rgbFrame==NULL)
				return IMPROC_ERROR;
				
			memset(rgbFrame, 0xFF, iFrameW*iFrameH*3);
		}

		return IMPROC_SUCCESS;
	}

	unsigned int DeInitRgbBuffer(void)
	{
		if(NULL!=rgbFrame)
			delete rgbFrame;
		rgbFrame = NULL;

		return IMPROC_SUCCESS;
	}

	unsigned int InitEGL(
		unsigned int iFrameW, 
		unsigned int iFrameH)
	{
		IMPROC_LOGI("InitEGL");

		EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		eglInitialize(display, 0, 0);

		EGLConfig config;
		EGLint numConfigs;
		EGLint format;
		EGLint configAttribs[] = 
		{
			EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,
			EGL_RED_SIZE,			8,
			EGL_GREEN_SIZE,	    	8,
			EGL_BLUE_SIZE,	    	8,
			EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
			EGL_NONE
		};
		
	//	EGLNativeWindowType win;

		eglChooseConfig(display, configAttribs, &config, 1, &numConfigs);
		eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
	//	ANativeWindow_setBuffersGeometry(m_pAndroidApp->window, 0, 0, format);
		
	//	EGLSurface surface = eglCreateWindowSurface(display, config, win, NULL);
		EGLint pBufAttribs[] = 
		{
			EGL_WIDTH,				iFrameW,
			EGL_HEIGHT,				iFrameH,
			EGL_LARGEST_PBUFFER,	EGL_TRUE,
			EGL_NONE
		};
		EGLSurface surface = eglCreatePbufferSurface(display, config, pBufAttribs);
		if(surface==EGL_NO_SURFACE)
		{
			IMPROC_LOGE("Error in eglCreatePbufferSurface");
			return IMPROC_ERROR;
		}
		
		EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE	};
		EGLContext context = eglCreateContext(display, config, NULL, contextAttribs);
		
		if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
		{
			IMPROC_LOGE("Error in eglMakeCurrent");
			return IMPROC_ERROR;
		}

		return IMPROC_SUCCESS;
	}

public:
	unsigned char* rgbFrame;

private:
	GLShader* 		pShader;
	GLTexture* 		pTexture;
	GLVertex* 		pVertex;
	GLFbo*			pFbo;
};

class ProcGLBaseRGB : public ProcGLBase
{
public:
	ProcGLBaseRGB(const char* iName)
			: 	ProcGLBase(iName)
	{
	}

	virtual unsigned int InitProperties(unsigned int iFrameW, unsigned int iFrameH)
	{
		return AssignProperties(COLORSPACE_SRGB, iFrameW, iFrameH);
	}

	virtual unsigned int DeInitProperties(void)
	{
		return IMPROC_SUCCESS;
	}

	virtual unsigned int Render(unsigned char* iFrame)
	{
		Yuv2Rgb(iFrame, rgbFrame, GetFrameWidth(), GetFrameHeight());

		GLUpdate(rgbFrame);

		ReadFbo(rgbFrame);

		Rgb2Yuv(rgbFrame, iFrame, GetFrameWidth(), GetFrameHeight());

		return IMPROC_SUCCESS;
	}
};

class ProcGLBaseYUV : public ProcGLBase
{
public:
	ProcGLBaseYUV(const char* iName)
		: ProcGLBase(iName)
	{
	}
	
	virtual unsigned int InitProperties(unsigned int iFrameW, unsigned int iFrameH)
	{
		return AssignProperties(COLORSPACE_YYUV, iFrameW, iFrameH);
	}

	virtual unsigned int DeInitProperties(void)
	{
		return IMPROC_SUCCESS;
	}

	virtual unsigned int Render(unsigned char* iFrame)
	{
		GLUpdate(iFrame);

		ReadFbo(rgbFrame);

		Yuvyuv2Yyuv(rgbFrame, iFrame, GetFrameWidth(), GetFrameHeight());

		return IMPROC_SUCCESS;
	}
};

}

#endif //_IMPROCGLPROC_H_
