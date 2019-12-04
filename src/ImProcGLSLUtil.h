#ifndef _IMPROCGLSL_UTIL_H_
#define _IMPROCGLSL_UTIL_H_

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

class GLSLVar
{
public:
	GLSLVar(GLuint iProgramObj, const char* iVarName)
	{
		nLoc = glGetUniformLocation(iProgramObj, iVarName);
	}

	inline GLint GetLoc()
	{
		return nLoc;
	}

private:
	// No default constructor
	GLSLVar()
		: nLoc(-1) { }
	
private:
	GLint nLoc;
};

class GLSLVarUniform1f : public GLSLVar
{
public:
	GLSLVarUniform1f(GLuint iProgramObj, const char* iVarName)
		: GLSLVar(iProgramObj, iVarName)
	{
	}

	void Set(GLfloat iFloat)
	{
		glUniform1f(GetLoc(), iFloat);
	}
};

class GLSLVarUniform1i : public GLSLVar
{
public:
	GLSLVarUniform1i(GLuint iProgramObj, const char* iVarName)
		: GLSLVar(iProgramObj, iVarName)
	{
	}

	void Set(GLint iInt)
	{
		glUniform1i(GetLoc(), iInt);
	}
};

class GLSLVarUniform2fv : public GLSLVar
{
public:
	GLSLVarUniform2fv(GLuint iProgramObj, const char* iVarName)
		: GLSLVar(iProgramObj, iVarName)
	{
	}

	void Set(GLfloat iFloat1, GLfloat iFloat2)
	{
		// Update for current variables
		GLfloat vFloat[2];
		vFloat[0] = iFloat1;
		vFloat[1] = iFloat2;
		glUniform2fv(GetLoc(), 1, (const GLfloat*)&vFloat);
	}
};


#endif //_IMPROCGLSL_UTIL_H_
