#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "ImProcUtilLog.h"

#include "ImProcGLUtil.h"

#include <time.h>

void checkGlError(const char* op) 
{
    for (GLint error = glGetError(); error; error = glGetError()) 
	{
        IMPROC_LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

void printGLString(const char *name, GLenum s) 
{
    const char *v = (const char *) glGetString(s);
    IMPROC_LOGI("GL %s = %s\n", name, v);
}

float GetEllipseTime(void)
{
	static unsigned int bInitialized = 0;
	static clock_t	baseTime;
	
	if ( 0 == bInitialized )
	{
		baseTime = clock();
		bInitialized = 1;
		return 0.0f;
	}

	float fAppTime = (GLfloat)(clock() - baseTime) / (GLfloat)CLOCKS_PER_SEC;
	return fAppTime;
}

