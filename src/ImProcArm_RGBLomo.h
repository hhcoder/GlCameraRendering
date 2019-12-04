#ifndef _IMPROCARM_RGBLOMO_H_
#define _IMPROCARM_RGBLOMO_H_

#include "ImProcArm.h"

#include "ImProcArmUtilLut.h"

namespace ImProc
{

class ImProcArmRgbLomo : public ImProcArmRgb
{
public:
	ImProcArmRgbLomo( )
		: 	ImProcArmRgb("ARM RGB Lomo"),
			mask(NULL),
			lutr(NULL),
			lutg(NULL),
			lutb(NULL)
	{
	}

	virtual unsigned int InitOperations(void)
	{
	    return InitLutMask(GetFrameWidth(), GetFrameHeight());
	}

	virtual unsigned int DeInitOperations( void )
	{
		return DeInitLutMask();
	}
	
	virtual unsigned int Render(unsigned char* iFrame)
	{
		yuv->set_source(iFrame);
		
		yyvu2rgb(yuv->get_source(), rgb->get_source(), yuv->width(), yuv->height());

		ProcLomo();
			
		rgb2yyvu(rgb->get_source(), yuv->get_source(), rgb->width(), rgb->height());

		return IMPROC_SUCCESS;
	}

private:
	unsigned int InitLutMask(unsigned int iFrameW, unsigned int iFrameH)
	{
		if(NULL!=mask) delete mask;
		if(NULL!=lutr) delete lutr;
		if(NULL!=lutg) delete lutg;
		if(NULL!=lutb) delete lutb;

		//TODO: take care of null allocation
		lutr = new PixelLut();
	    lutr->SmoothStep(50, 185, 0, 200);

		lutg = new PixelLut();
    	lutg->SmoothStep(20, 220, 0, 220);

		lutb = new PixelLut();	
	    lutb->SmoothStep(20, 200, 0, 255);

	    return ReadMaskFile("/data/lomo_mask.rgb");
	}

	unsigned int DeInitLutMask(void)
	{
		if(NULL!=lutb) delete lutb;
		if(NULL!=lutg) delete lutg;
		if(NULL!=lutr) delete lutr;
		if(NULL!=mask) delete mask;

		return IMPROC_SUCCESS;
	}

private:
	void ProcLomo(void)
	{
        unsigned char* ptr = rgb->get_source();
        unsigned char* end = ptr+rgb->size();
        unsigned char* ptr_mask = mask->get_source();

        unsigned int val;

        while(ptr!=end)
        {
            val = lutr->Calc(*ptr);
            *ptr = (val * (*ptr_mask)) >> 8;
            ptr++; ptr_mask++;

            val = lutg->Calc(*ptr);
            *ptr = (val * (*ptr_mask)) >> 8;
            ptr++; ptr_mask++;
            
            val = lutb->Calc(*ptr);
            *ptr = (val * (*ptr_mask)) >> 8;
            ptr++; ptr_mask++;
        }
	}


private:
	unsigned int ReadMaskFile(const char* fname)
	{
		mask = new imbuf(640, 480, IMTYPE_RGB);
		//TODO: check error IO code
		FILE* fp = fopen(fname, "rb");
		if(NULL==fp)
		{
			IMPROC_LOGE("Error in opening file %s", fname);
			return IMPROC_ERROR;
		}
		fread(mask->get_source(), sizeof(unsigned char), mask->size(), fp);
		fclose(fp);

		return IMPROC_SUCCESS;
	}

private:
	imbuf* 			mask;
	PixelLut* 		lutr;
	PixelLut* 		lutg;
	PixelLut* 		lutb;
};

}
#endif //_IMPROCARM_RGBLOMO_H_
