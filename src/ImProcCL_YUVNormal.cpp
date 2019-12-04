#include "ImProcCL.h"
#include "ImProcCL_YUVNormal.h"

#include "CL_BufCopy.h"

#include <CL/cl.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h> 

using namespace ImProc;


unsigned int ImProcCLNormal::DeInitBufs(void)
{
	return IMPROC_SUCCESS;
}

unsigned int ImProcCLNormal::DeInitOperations(void)
{
	return IMPROC_SUCCESS;
}

unsigned int ImProcCLNormal::DeInitProperties(void)
{
	return IMPROC_SUCCESS;
}

unsigned int ImProcCLNormal::Exit(void)
{
	return IMPROC_SUCCESS;
}

unsigned int ImProcCLNormal::Init(unsigned int iFrameW, unsigned int iFrameH)
{
	AssignProperties(COLORSPACE_YYUV, iFrameW, iFrameH);
	return IMPROC_SUCCESS;
}

//TODO: remove the iFrameW, iFrameH (use GetFrameWidth, GetFrameHeight)
unsigned int ImProcCLNormal::InitBufs(unsigned int iFrameW, unsigned int iFrameH)
{
	return IMPROC_SUCCESS;
}

unsigned int ImProcCLNormal::InitOperations(void)
{
	return IMPROC_SUCCESS;
}

unsigned int ImProcCLNormal::InitProperties(unsigned int iFrameW, unsigned int iFrameH)
{
	return IMPROC_SUCCESS;
}

unsigned int ImProcCLNormal::Render(unsigned char *iFrame)
{
	cl_copy_main(GetFrameWidth(), GetFrameHeight(), iFrame);
	
	return IMPROC_SUCCESS;
}

