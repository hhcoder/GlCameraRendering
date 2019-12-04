#ifndef _IMPROCBASE_H_
#define _IMPROCBASE_H_

#include "ImProcUtilLog.h"
#include "ImProcUtilColorT.h"

namespace ImProc
{

typedef enum
{
	COLORSPACE_SRGB,
	COLORSPACE_YYUV,
	COLORSPACE_ALL
} PROC_COLORSPACE;

#define IMPROC_SUCCESS			(1)
#define IMPROC_ERROR			(0)

class ImProcBase 
{
public:
	ImProcBase(const char* iName)
	{
		StrCpyLen(iName, nameDescp, RTIP_STR_MAXLEN);
	}

	virtual ~ImProcBase()
	{
	}

	virtual unsigned int Init(	unsigned int iFrameW, unsigned int iFrameH)
	{
		InitProperties(iFrameW, iFrameH);
	
		InitOperations( );

		//TODO: remove the iFrameW, iFrameH here, the W/H should come from getFrameWidth/getFrameHeight
		InitBufs(iFrameW, iFrameH);

		//TODO: check error in returning
		return IMPROC_SUCCESS;
	}

	virtual unsigned int Exit( void )
	{
		DeInitBufs();

		DeInitOperations();
	
		DeInitProperties();

		//TODO: check error in returning
		return IMPROC_SUCCESS;
	}

	virtual unsigned int Render(unsigned char* iFrame) = 0;

public:
	virtual unsigned int InitProperties(unsigned int iFrameW, unsigned int iFrameH) = 0;

	virtual unsigned int DeInitProperties(void) = 0;

	//TODO: remove the iFrameW, iFrameH (use GetFrameWidth, GetFrameHeight)
	virtual unsigned int InitBufs(unsigned int iFrameW, unsigned int iFrameH) = 0;

	virtual unsigned int DeInitBufs(void) = 0;

	virtual unsigned int InitOperations(void) = 0;

	virtual unsigned int DeInitOperations(void) = 0;

public:
	unsigned int AssignProperties(
		PROC_COLORSPACE iSpace,
		unsigned int iFrameW,
		unsigned int iFrameH )
	{
		procSpace = iSpace;
		frameWidth = iFrameW;
		frameHeight = iFrameH;

		//TODO: add const definition
		return IMPROC_SUCCESS;
	}

public:	
	inline PROC_COLORSPACE GetProcSpace(void) { return procSpace; }
	
	inline unsigned int GetFrameWidth(void) { return frameWidth; }

	inline unsigned int GetFrameHeight(void) { return frameHeight; }

	unsigned int GetName (char* retName) 
	{
		return StrCpyLen(nameDescp, retName, RTIP_STR_MAXLEN);
	}

private:
	PROC_COLORSPACE 		procSpace;
	unsigned int 			frameWidth;
	unsigned int 			frameHeight;


private:
	char					nameDescp[RTIP_STR_MAXLEN];
	
	int StrCpyLen(const char* src, char* dst, unsigned int len)
	{
	    unsigned int i = 0;
	    const char* psrc = src;
	    char* pdst = dst;

	    if(NULL==src||NULL==dst)
	        return IMPROC_ERROR;

	    while(i<=len-1 && src[i]!='\0')
	    {
	        *pdst = *psrc;
	        psrc++; pdst++; i++;
	    }

	    dst[i] = '\0';

	    return IMPROC_SUCCESS;
	}

public:
	static void Yuv2Rgb(
		unsigned char* yuv, 
		unsigned char* rgb, 
		unsigned int iFrameW, 
		unsigned int iFrameH)
	{
		yyvu2rgb(yuv, rgb, iFrameW, iFrameH);
	}

	static void Rgb2Yuv(
		unsigned char* rgb, 
		unsigned char* yuv,
		unsigned int iFrameW,
		unsigned int iFrameH)
	{
		rgb2yyvu(rgb, yuv, iFrameW, iFrameH);
	}

	static void Yuvyuv2Yyuv(
		unsigned char* yuvyuv, 
		unsigned char* yyuv,
		unsigned int iFrameW,
		unsigned int iFrameH)
	{
		// interlace the YUV input
		//
		// input:  YUVYUVYUVYUV
		//         YUVYUVYUVYUV
		//
		// output: YYYY
		//         YYYY
		//         UV
		//         UV

		yuvyuv2yyuv(yuvyuv, yyuv, iFrameW, iFrameH);
	}
	
};

}

#endif
