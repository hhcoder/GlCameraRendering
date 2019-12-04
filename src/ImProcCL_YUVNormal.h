#ifndef _IMPROCCL_YUVNORMAL_
#define _IMPROCCL_YUVNORMAL_

#include "ImProcCL.h"

namespace ImProc
{
class ImProcCLNormal : public ProcCLBase
{
public:
   ImProcCLNormal() 
		: ProcCLBase("CL Normal")
	{
	}
		
	~ImProcCLNormal()
	{
	}

public:
virtual unsigned int Render(unsigned char *iFrame);
virtual unsigned int InitProperties(unsigned int iFrameW, unsigned int iFrameH);

virtual unsigned int InitOperations(void);

//TODO: remove the iFrameW, iFrameH (use GetFrameWidth, GetFrameHeight)
virtual unsigned int InitBufs(unsigned int iFrameW, unsigned int iFrameH);

virtual unsigned int Init(unsigned int iFrameW, unsigned int iFrameH);

virtual unsigned int Exit(void);

virtual unsigned int DeInitProperties(void);

virtual unsigned int DeInitOperations(void);

virtual unsigned int DeInitBufs(void);

};

}

#endif //_IMPROCCL_YUVNORMAL_
