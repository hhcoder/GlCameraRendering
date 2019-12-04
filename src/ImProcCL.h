#ifndef _IMPROCCL_H_
#define _IMPROCCL_H_

#include "ImProc.h"
#include "ImProcBase.h"

namespace ImProc
{

class ProcCLBase : public ImProcBase
{
public:
	ProcCLBase(const char* iname)
		: ImProcBase(iname)
	{
	}
	
	~ProcCLBase()
	{
	}
};

}
#endif
