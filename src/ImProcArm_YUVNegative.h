#ifndef _IMPROCARM_YUVNEGATIVE_H_
#define _IMPROCARM_YUVNEGATIVE_H_

#include "ImProcArm.h"

namespace ImProc
{

class ImProcArmYuvNegative : public ImProcArmYuv
{
public:
	ImProcArmYuvNegative()
		: 	ImProcArmYuv("ARM YUV Negative")
	{
	}

	virtual unsigned int Render( unsigned char* iFrame)
	{
		unsigned char* ptr;
		unsigned char* end;
		
		yuv->set_source(iFrame);
		
		ptr = yuv->get_source();
		end = ptr+yuv->size();

		while(ptr!=end)
		{
			(*ptr) = 0xFF - (*ptr);
			ptr++;
		}

		return RTIP_SUCCESS;
	}


};

}

#endif //_IMPROCARM_YUVNEGATIVE_H_
