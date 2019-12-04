#ifndef _IMPROCGLSHADER_H_
#define _IMPROCGLSHADER_H_

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "ImProcGLUtil.h"

#include "ImProcGLSLUtil.h"

namespace ImProc
{

class GLShader
{
public:
	GLuint ShaderLoadStrConst(GLenum shaderType, const char* pSource)
	{
	    GLuint shader = glCreateShader(shaderType);
		
	    if (shader) 
		{
	        glShaderSource(shader, 1, &pSource, NULL);
			glCompileShader(shader);
	        GLint compiled = 0;
		    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		    if (!compiled) 
			{
		        GLint infoLen = 0;
		        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		        if (infoLen) 
				{
		            char* buf = (char*) malloc(infoLen);
		            if (buf) 
					{
		                glGetShaderInfoLog(shader, infoLen, NULL, buf);
		                IMPROC_LOGE("Could not compile shader %d:\n%s\n", shaderType, buf);
		                free(buf);
		            }
		            glDeleteShader(shader);
		            shader = 0;
		        }
				return 0;
		    }
			IMPROC_LOGI("ShaderLoadStrConst %d done", shaderType);
	    }
	    return shader;
	}
	
	GLuint ShaderProgramLink(GLuint program)
	{
		// link the program
		glLinkProgram(program);

		// check link status
	    GLint linkStatus = GL_FALSE;
	    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	    if (linkStatus != GL_TRUE) 
		{
	        GLint bufLength = 0;
	        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
	        if (bufLength) 
			{
	            char* buf = (char*) malloc(bufLength);
	            if (buf) 
				{
	                glGetProgramInfoLog(program, bufLength, NULL, buf);
	                IMPROC_LOGE("Could not link program:\n%s\n", buf);
	                free(buf);
	            }
	        }
	        glDeleteProgram(program);
			return 0;
	    }

		return 1;
	}

	GLuint ShaderProgramCreate()
	{
		GLuint program = glCreateProgram();
		if(program!=0)
			IMPROC_LOGI("ShaderProgramCreate done");
		return program;
	}

	GLShader(const char* iname, const char* iStrVertex, const char* iStrFrag)
		: name(iname)
	{
		// Create and load shader
		shaderObject[0] = ShaderLoadStrConst(GL_VERTEX_SHADER, iStrVertex);
		shaderSource[0] = iStrVertex;
		if(0==shaderObject[0])
		{
			checkGlError("Creating fragment shader object");
			IMPROC_LOGE("in %s", iname);
			return;
		}
		
		shaderObject[1] = ShaderLoadStrConst(GL_FRAGMENT_SHADER, iStrFrag);
		shaderSource[1] = iStrFrag;
		if(0==shaderObject[1])
		{
			checkGlError("Creating vertex shader object");
			IMPROC_LOGE("Error in creating %s", iname);
			return;
		}

		// Create and load program
		programObject = ShaderProgramCreate();

		if(0==programObject)
		{
			checkGlError("Creating Program Object");
			IMPROC_LOGE("Error in creating %s", iname);
			return;
		}

		// Attach the shaders with program	
		glAttachShader(programObject, shaderObject[0]);
		glAttachShader(programObject, shaderObject[1]);

		// Bind g_vVertex to vertext shader attribute 0
		glBindAttribLocation(programObject, 0, "g_vVertex");

		// Link the program
		if( 0 == ShaderProgramLink(programObject))
		{
			return;
		}
	}

	virtual ~GLShader()
	{
		glDeleteProgram(programObject);
	}

	virtual void Update(void) = 0;

	void UseProgram(void)
	{
		glUseProgram(programObject);
	}


protected:
	const char* name;

	GLuint programObject;

	GLuint shaderObject[2];
	const char* shaderSource[2];
};

};

#endif //_IMPROCGLSHADER_H_
