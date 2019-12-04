#ifndef _IMPROCARM_YUV_BLEND_H_
#define _IMPROCARM_YUV_BLEND_H_
#include "ImProcArm.h"

namespace ImProc
{

class ImProcArmYuvBlend : public ImProcArmYuv
{
public:
	ImProcArmYuvBlend()
		: 	ImProcArmYuv("ARM YUV Blending"),
			count(0),
			frames(5),
			yuvarray(NULL)
	{
	}

	virtual unsigned int InitOperations(void)
	{
	    return YuvArrayInit(GetFrameWidth(), GetFrameHeight());
	}

	virtual unsigned int DeInitOperations( void )
	{
		return YuvArrayDeInit();
	}

	virtual unsigned int Render( unsigned char* iFrame)
	{
		count++;

		yuv->set_source(iFrame);

		if(count >= frames)
		{
			YuvMerge(
				yuv->get_source(), 
				yuvarray[0]->get_source(), 
				yuvarray[1]->get_source(), 
				yuvarray[2]->get_source(),
				yuvarray[3]->get_source(),
				yuvarray[4]->get_source(),
				iFrame, yuv->size() );
		}

		yuvarray[count%frames]->copy_from(yuv);

		return IMPROC_SUCCESS;
	}

private:
	unsigned int YuvArrayInit(unsigned int iFrameW, unsigned int iFrameH)
	{
		if(NULL==yuvarray)
			yuvarray = new imbuf*[frames];

	    for(unsigned int i=0; i<frames; i++)
	    {
	    	yuvarray[i] = new imbuf(iFrameW, iFrameH, IMTYPE_YUV);
	    	yuvarray[i]->heap_allocate();
	    	if(NULL==yuvarray[i])
	    	{
	    		IMPROC_LOGE("Not enough space in YUVBlend effect");
		    	return IMPROC_ERROR;
		    }
	    }

		return IMPROC_SUCCESS;
	}

	unsigned int YuvArrayDeInit(void)
	{
		for(unsigned int i=0; i<frames; i++)
		{
			if( NULL != yuvarray[i])
			{
				delete yuvarray[i];
				yuvarray[i] = NULL;
			}
		}

		if( yuvarray != NULL)
		{
			delete[] yuvarray;
			yuvarray = NULL;
		}
	
		return IMPROC_SUCCESS;
	}

	void YuvMerge(
		unsigned char* src, 
		unsigned char* src0, 
		unsigned char* src1, 
		unsigned char* src2, 
		unsigned char* src3, 
		unsigned char* src4, 
		unsigned char* dst, 
		size_t sz)
	{
		unsigned char* psrc = src;
		unsigned char* psrc0 = src0;
		unsigned char* psrc1 = src1;
		unsigned char* psrc2 = src2;
		unsigned char* psrc3 = src3;
		unsigned char* psrc4 = src4;
		unsigned char* pdst = dst;

		unsigned int i = 0;
		
		while(i<=sz)
		{
			unsigned int val;

			val = 	*psrc  + *psrc  + *psrc  + *psrc  + *psrc  + *psrc  + *psrc  + 
					*psrc  + *psrc0 + *psrc0 +
					*psrc0 + *psrc1 + 
					*psrc1 + *psrc2 + 
					*psrc3 + 
					*psrc4;
					
			*pdst = val >> 4;

			psrc++;  psrc0++; psrc1++; psrc2++; psrc3++; psrc4++; 
			pdst++;
		
			i++;
		}
	}

private:
	unsigned int	count;
	unsigned int	frames;
	imbuf**			yuvarray;
};

}

#endif //_IMPROCARM_YUV_BLEND_H_
