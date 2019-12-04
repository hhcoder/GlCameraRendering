#ifndef _IMPROCARM_H_
#define _IMPROCARM_H_

#include "ImProcBase.h"

#include "ImProcUtilLog.h"

namespace ImProc
{

typedef enum
{
    IMTYPE_YUV,
    IMTYPE_RGB,
    IMTYPE_ALL
}IMTYPE;

typedef enum
{
    BUFTYPE_NON,
    BUFTYPE_HEAP,
    BUFTYPE_EXTERNAL,
    BUFTYPE_ALL
}BUFTYPE;

//TODO: make this class available across ProcArmBase, ProcGLBase
class imbuf
{
public:

    imbuf(unsigned int iw, unsigned int ih, IMTYPE iType)
        :   w(iw), 
            h(ih), 
            buf(NULL),
            btype(BUFTYPE_NON)
    {
        switch(iType)
        {
            case IMTYPE_YUV:
                sz = w*h*3/2;
                bpp = 1.5;
            break;

			case IMTYPE_ALL:
            case IMTYPE_RGB:
                sz = w*h*3;
                bpp = 3;
            break;
        }
    }

    ~imbuf()
    {
        if(NULL!=buf && BUFTYPE_HEAP==btype)
            delete[] buf;
    }

    void set_source(unsigned char* ibuf)
    {
        if(NULL!=buf && BUFTYPE_HEAP==btype)
        {
            delete[] buf;
        }

        buf = ibuf;
        btype = BUFTYPE_EXTERNAL;
    }

    void copy_from(imbuf* src)
    {
    	unsigned char* pdst = get_source();
    	unsigned char* psrc = src->get_source();

    	if(size() != src->size())
	    	return;
    	
    	memcpy(pdst, psrc, src->size());
    }

    unsigned int heap_allocate(void)
    {
    	if(NULL!=buf)
    		delete[] buf;

		buf = new unsigned char[sz];
   	    if(NULL==buf)
   	    {
   	    	IMPROC_LOGE("Error in allocating buf from heap: %d", sz);
   	    	btype = BUFTYPE_NON;
   	    	return IMPROC_ERROR;
   	    }
   	    else
   	    {
	        btype = BUFTYPE_HEAP;   	
	        return IMPROC_SUCCESS;
	    }
    }
    

    unsigned char* get_source(void)
    {
        if(NULL==buf && BUFTYPE_NON==btype)
        {
            buf = new unsigned char[sz];
            btype = BUFTYPE_HEAP;
        }
        
        return buf;
    }

    inline BUFTYPE get_buftype(){ return btype; }
    inline unsigned int width(){ return w; }
    inline unsigned int height(){ return h;}
    inline size_t size(){ return sz;}
    inline float bytesperpixel(){ return bpp;}

private:
    unsigned int        w;
    unsigned int        h;
    size_t              sz;
    float               bpp; //bytes per pixel
    unsigned char*      buf;
    BUFTYPE             btype;
};

}

namespace ImProc
{

class ProcArmBase : public ImProcBase
{
public:
	ProcArmBase(const char* iName)
		  :	ImProcBase(iName),
			yuv(NULL)
	{
	}

public:
	unsigned int InitYUVBuf(unsigned int iFrameW, unsigned int iFrameH)
	{
		if(NULL!=yuv)
			delete yuv;
	    yuv = new imbuf(iFrameW, iFrameH, IMTYPE_YUV);

	    if(NULL==yuv)
	    {
	    	IMPROC_LOGE("Out of memory in InitYUVBUf");
	    	return IMPROC_ERROR;
	    }

		return IMPROC_SUCCESS;
	}

	unsigned int DeInitYUVBuf(void)
	{
		if(NULL!=yuv)
			delete yuv;
		yuv = NULL;

		return IMPROC_SUCCESS;
	}

public:
	imbuf*			yuv;
};

class ImProcArmRgb : public ProcArmBase
{
public:
	ImProcArmRgb(const char* iName)
		: 	ProcArmBase(iName),
			rgb(NULL)
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

	virtual unsigned int InitBufs(unsigned int iFrameW, unsigned int iFrameH)
	{
		if(IMPROC_SUCCESS == InitYUVBuf(iFrameW, iFrameH))
			return InitRGBBuf(iFrameW, iFrameH);
		else
			return IMPROC_ERROR;
	}

	virtual unsigned int DeInitBufs(void)
	{
		if(IMPROC_SUCCESS == DeInitYUVBuf())
			return DeInitRGBBuf();
		else
			return IMPROC_ERROR;
	}

private:
	unsigned int InitRGBBuf(unsigned int iFrameW, unsigned int iFrameH)
	{
		if(NULL!=rgb)
			delete rgb;
    	rgb = new imbuf(iFrameW, iFrameH, IMTYPE_RGB);

    	if(NULL==rgb)
    	{
	    	IMPROC_LOGE("Out of memory in InitYUVBUf");
	    	return IMPROC_ERROR;
    	}
    	else
    	{
    		return rgb->heap_allocate();
    	}
	}

	unsigned int DeInitRGBBuf(void)
	{
		if(NULL!=rgb)
		{
			delete rgb;
			rgb = NULL;
		}

		return IMPROC_SUCCESS;
	}

public:
	imbuf*			rgb;
};

class ImProcArmYuv : public ProcArmBase
{
public:
	ImProcArmYuv(const char* iName)
		: 	ProcArmBase(iName)
	{
	}

public:
	virtual unsigned int InitProperties(unsigned int iFrameW, unsigned int iFrameH)
	{
		return AssignProperties(COLORSPACE_YYUV, iFrameW, iFrameH);
	}

	virtual unsigned int DeInitProperties(void)
	{
		return IMPROC_SUCCESS;
	}

	virtual unsigned int InitOperations(void)
	{
	    return IMPROC_SUCCESS;
	}

	virtual unsigned int DeInitOperations( void )
	{
	    return IMPROC_SUCCESS;
	}

	virtual unsigned int InitBufs(unsigned int iFrameW, unsigned int iFrameH)
	{
		return InitYUVBuf(iFrameW, iFrameH);
	}
	
	virtual unsigned int DeInitBufs(void)
	{
		return DeInitYUVBuf();
	}
};

}

#endif
