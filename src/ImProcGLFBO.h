#ifndef _IMPROCGLFBO_H_
#define _IMPROCGLFBO_H_

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

//--------------------------------------------------------------------------------------
// Name: class CFrmFrameBufferObject
// Desc: Wrapper class for rendering to offscreen textures via FBOs
//--------------------------------------------------------------------------------------

namespace ImProc
{

typedef GLuint 	UINT32;
typedef GLint 	INT32;
typedef GLuint	BOOL;
#define FALSE 	0
#define TRUE 	1
#define VOID 	void

class GLFbo
{
public:
    ~GLFbo()
    {
	    Destroy();
    }

    GLFbo( UINT32 nWidth, UINT32 nHeight, UINT32 nFormat, UINT32 nType )
    	: 	m_nWidth(nWidth),
    		m_nHeight(nHeight),
    		m_nFormat(nFormat),
    		m_nType(nType),
    		m_hTexture(0), 
    		m_hFrameBuffer(0),
    		m_hRenderBuffer(0),
    		m_bBegined(FALSE)
	{
	    //Destroy();

	    //m_hTexture      = 0;
	    //m_hFrameBuffer  = 0;
	    //m_hRenderBuffer = 0;
	    
	    //m_nWidth  = nWidth;
	    //m_nHeight = nHeight;
	    //m_nFormat = nFormat;
	    //m_nType   = nType;

	    IMPROC_LOGI("Constructor: GLFbo");

	    // Create an offscreen texture
	    glGenTextures( 1, &m_hTexture );
	    glBindTexture( GL_TEXTURE_2D, m_hTexture );
	    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE );
	    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE );
	    glTexImage2D( GL_TEXTURE_2D, 0, nFormat, nWidth, nHeight, 0, nFormat, nType, NULL );

	    if( m_nFormat == GL_DEPTH_COMPONENT )
	    {
	        glGenFramebuffers( 1, &m_hFrameBuffer );
	        glBindFramebuffer( GL_FRAMEBUFFER, m_hFrameBuffer );
	        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_hTexture, 0 );
	    }
	    else // ( m_nFormat == GL_RGB || m_nFormat == GL_RGBA )
	    {
	        //glGenRenderbuffers( 1, &m_hRenderBuffer );
	        //glBindRenderbuffer( GL_RENDERBUFFER, m_hRenderBuffer );
	        //glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_nWidth, m_nHeight );

	        glGenFramebuffers( 1, &m_hFrameBuffer );
	        glBindFramebuffer( GL_FRAMEBUFFER, m_hFrameBuffer );
	        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_hTexture, 0 );
	        //glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_hRenderBuffer );
	    }

	    if( GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus( GL_FRAMEBUFFER ) )
	    {
	    	//TODO: move constructor outside that we can check error
	    	IMPROC_LOGE("FrameBuffer creation error!");	    
	        //return FALSE;
	    }

	    glBindFramebuffer( GL_FRAMEBUFFER, m_hFrameBuffer );
	    //glBindRenderbuffer( GL_RENDERBUFFER, m_hRenderBuffer );
	}
	
    VOID   Destroy()
    {
	    glDeleteTextures( 1, &m_hTexture );
	    glDeleteFramebuffers( 1, &m_hFrameBuffer );
	    glDeleteRenderbuffers( 1, &m_hRenderBuffer );
    }
    
    VOID   Begin()
    {
    	m_bBegined = TRUE;
	    // Save values so we can restore the viewport when finished rendering to the FBO
	    glGetIntegerv( GL_VIEWPORT, (GLint*)&m_PreviousViewport );

	    glBindFramebuffer( GL_FRAMEBUFFER, m_hFrameBuffer );
	 
	    if( m_nFormat == GL_DEPTH_COMPONENT )
	    {
	        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_hTexture, 0 );
	    }
	    else // ( m_nFormat == GL_RGB || m_nFormat == GL_RGBA )
	    {
	        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_hTexture, 0 );
	        glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_hRenderBuffer );
	    }
	    
	    glViewport( 0, 0, m_nWidth, m_nHeight );
    }

    VOID   End()
    {
    	if(m_bBegined)
    	{
		    // Restore rendering the previous framebuffer
		    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	    	glViewport( m_PreviousViewport.x, m_PreviousViewport.y,
	        	        m_PreviousViewport.w, m_PreviousViewport.h );
			m_bBegined = FALSE;
		}
    }
    UINT32 GetTexture() 
    { 
	    return m_hTexture; 
	}

public: 
    UINT32 m_nWidth;
    UINT32 m_nHeight;
    UINT32 m_nFormat;
    UINT32 m_nType;

    UINT32 m_hTexture;
    UINT32 m_hFrameBuffer;
    UINT32 m_hRenderBuffer;

    BOOL m_bBegined;

    struct
    { 
        INT32 x, y, w, h;
    } m_PreviousViewport;
};

}// End namespace ImProc

#endif //_IMPROCGLFBO_H_

