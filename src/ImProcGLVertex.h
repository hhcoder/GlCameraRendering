#ifndef _IMPROCGLVERTEX_H_
#define _IMPROCGLVERTEX_H_

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace ImProc
{

class GLVertex
{
public:
	GLVertex()	{	}

 	virtual void Update() = 0;

	virtual ~GLVertex()	{	}
};

};

namespace ImProc
{

class GLVertex_FlatSurface : public GLVertex
{
public:
	GLVertex_FlatSurface( void )
	{
	    IMPROC_LOGI("Constructor: GLVertex_FlatSurface");

		// Load the vertex data

	    /*
		GLfloat ratioW;
		GLfloat ratioH;
		GLfloat fAspectRatio = (GLfloat)iw/ (GLfloat)ih;

	    ratioH = ( fAspectRatio < 1.0f ) ? 1.0f : 1.0f / fAspectRatio;
	    ratioW = ( fAspectRatio < 1.0f ) ? fAspectRatio : 1.0f;

	    GLfloat vVert[] =
	    {
	        -ratioW, -ratioH, 0.0f, 1.0f,
	        +ratioW, -ratioH, 1.0f, 1.0f,
	        +ratioW, +ratioH, 1.0f, 0.0f,
	        -ratioW, +ratioH, 0.0f, 0.0f
		};
		*/

	    GLfloat vVert[] =
	    {
	    	//Vertex coord	|	  Texture coord
	    	//  x, y		|          x, y
	        -1.0f, -1.0f, 			0.0f, 1.0f,
	        +1.0f, -1.0f, 			1.0f, 1.0f,
	        +1.0f, +1.0f, 			1.0f, 0.0f,
	        -1.0f, +1.0f, 			0.0f, 0.0f
		};

		for(unsigned int i=0; i<sizeof(vVert)/sizeof(GLfloat); i++)
		{
			vVertices[i] = vVert[i];
		}
	}
	
	virtual void Update()
	{
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, vVertices);
		glEnableVertexAttribArray(0);

	    // Draw the textured quad
	    glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
	}

	~GLVertex_FlatSurface()
	{
	}

private:
	GLfloat vVertices[16];
};

};

#endif //_IMPROCGLVERTEX_H_
