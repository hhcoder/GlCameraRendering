#ifndef _IMPROCARM_RGBNORMAL_H_
#define _IMPROCARM_RGBNORMAL_H_

#include "ImProcArm.h"

namespace ImProc
{

class ImProcArmRgbNormal : public ImProcArmRgb
{
public:
	ImProcArmRgbNormal( )
		: ImProcArmRgb("ARM RGB Normal")
	{	
	}

	virtual unsigned int InitOperations(void)
	{
	    return IMPROC_SUCCESS;
	}

	virtual unsigned int DeInitOperations( void )
	{
	    return IMPROC_SUCCESS;
	}

	unsigned int Render( unsigned char* iFrame)
	{
		yuv->set_source(iFrame);
		
		yyvu2rgb(yuv->get_source(), rgb->get_source(), yuv->width(), yuv->height());
			
		rgb2yyvu(rgb->get_source(), yuv->get_source(), rgb->width(), rgb->height());
		
		return 0;
	}
};

}


#endif //_IMPROCARM_RGBNORMAL_H_
