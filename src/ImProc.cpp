#include "ImProc.h"

#include "ImProcGL.h"
#include "ImProcArm.h"

#include "ImProcGL_RGBNormal.h"
#include "ImProcGL_RGBCutOut.h"
#include "ImProcGL_RGBLens.h"
#include "ImProcGL_RGBEmboss.h"
#include "ImProcGL_RGBLomo.h"
#include "ImProcGL_YUVNormal.h"
#include "ImProcGL_YUVLomo.h"

#include "ImProcArm_RGBNormal.h"
#include "ImProcArm_RGBLomo.h"
#include "ImProcArm_YUVNegative.h"
#include "ImProcArm_YUVBlend.h"

#include "ImProcCL.h"
#include "ImProcCL_YUVNormal.h"

namespace ImProc
{

class ProcContainer
{
public:
	ProcContainer()
		: 	length(RTIP_EFFECT_NUM_MAX), 
			count(0),
			list(NULL)
	{
		list = new ImProcBase*[length];
		for(unsigned int i=0; i<length; i++)
			list[i] = NULL;
	}

	~ProcContainer()
	{
		for(unsigned int i=0; i<length; i++)
		{
			if(NULL!=list[i])
				delete list[i];
		}

		delete[] list;
	}

	inline ImProcBase* GetProc(unsigned int idx)
	{
		if(idx<count)
			return list[idx];
		else
			return list[0];
	}

	inline unsigned int GetCount(void)
	{ 
		return count; 
	}

	unsigned int Add(ImProcBase* proc)
	{
		list[count] = proc;
		count++;

		if(count>length)
		{
			//TODO: Extend the buffer when exceeding the default alloc size
		}

		// return the added index
		return count-1;
	}

private:
	unsigned int	length;
	unsigned int 	count;
	
	ImProcBase**	list;
};

}

namespace ImProc
{

class ProcManager
{
public:
	ProcManager(const RTIPSetInitStruct* iInitSetup)
		: 	procContainer(NULL),
			activeProc(NULL),
			currIdx(9),
			newIdx(currIdx)		
	{
		procContainer = new ImProc::ProcContainer();

		procContainer->Add( new ImProc::ImProcGLRgbNormal() );
		procContainer->Add( new ImProc::ImProcGLRgbCutout() );
		procContainer->Add( new ImProc::ImProcGLRgbEmboss() );
		procContainer->Add( new ImProc::ImProcGLRgbLens() );
		procContainer->Add( new ImProc::ImProcGLRgbLomo() );
		procContainer->Add( new ImProc::ImProcGLYuvNormal() );
		procContainer->Add( new ImProc::ImProcGLYuvLomo() );
		procContainer->Add( new ImProc::ImProcArmRgbNormal() );
		procContainer->Add( new ImProc::ImProcArmRgbLomo() );
		procContainer->Add( new ImProc::ImProcArmYuvNegative() );
		procContainer->Add( new ImProc::ImProcArmYuvBlend() );
		procContainer->Add( new ImProc::ImProcCLNormal() );

		initSetup.iFrameW = iInitSetup->iFrameW;
		initSetup.iFrameH = iInitSetup->iFrameH;

		SetActiveProc( procContainer->GetProc(currIdx), initSetup.iFrameW, initSetup.iFrameH );
	}

	~ProcManager()
	{
		SetActiveProc(NULL, 0, 0);
		activeProc = NULL;

		if(NULL!=procContainer)
		{
			delete procContainer;
			procContainer = NULL;
		}
	}

	unsigned int Render(
		unsigned char* iFrame)
	{
		if(NULL==iFrame)
			return IMPROC_ERROR;
	
		if(newIdx != currIdx)
		{
			SetActiveProc( procContainer->GetProc(newIdx), initSetup.iFrameW, initSetup.iFrameH );
			currIdx = newIdx;
		}

		return activeProc->Render(iFrame);
	}

	unsigned int SetActiveEffect(
		unsigned int iIdx)
	{
		if(iIdx < procContainer->GetCount())
		{
			newIdx = iIdx;
			return IMPROC_SUCCESS;
		}
		else
		{
			return IMPROC_ERROR;
		}
	}

	unsigned int GetEffectName(unsigned int iIdx, char* retName)
	{
		ImProcBase* proc = procContainer->GetProc(iIdx);

		if(NULL!=proc)
			return proc->GetName(retName);
		else
			return IMPROC_ERROR;
	}

	unsigned int GetNumEffects(unsigned int* retNum)
	{
		if(NULL==retNum)
			return IMPROC_ERROR;
	
		*retNum = procContainer->GetCount();
		return IMPROC_SUCCESS;
	}

private:
	unsigned int SetActiveProc(
		ImProcBase* iSetProc,
		unsigned int iFrameW,
		unsigned int iFrameH)
	{
		//TODO: check the iSetProc should be within the list

		if(activeProc!=iSetProc && NULL!=activeProc)
		{
			//TODO: define new Exit error code from API interface
			activeProc->Exit();
		}

		activeProc = iSetProc;

		if(NULL!=activeProc)
		{
			char effName[RTIP_STR_MAXLEN];

			activeProc->GetName(effName);
			IMPROC_LOGI("Init Effect: %s", effName);
			
			return activeProc->Init(iFrameW, iFrameH);
		}
		else
		{
			return IMPROC_SUCCESS;
		}
	}
	

private:
	ProcContainer* 			procContainer;
	ImProcBase* 			activeProc;
	RTIPSetInitStruct 		initSetup;

	unsigned int 			currIdx;
	unsigned int 			newIdx;
};
	
};


static ImProc::ProcManager* procMng = NULL;

unsigned int RTIP_Init(const RTIPSetInitStruct* iInitSetup)
{
	IMPROC_LOGI("RTIP_Init");

	if (NULL==iInitSetup)
		return RTIP_ERROR;

	if (iInitSetup->iFrameW==0||iInitSetup->iFrameH==0)
		return RTIP_ERROR;

	procMng = new ImProc::ProcManager(iInitSetup);

	return RTIP_SUCCESS;
}

unsigned int RTIP_Exit(void)
{
	IMPROC_LOGI("RTIP_Exit");

	delete procMng;
	procMng = NULL;

	return RTIP_SUCCESS;
}

unsigned int RTIP_RenderImg(const RTIPRenderStruct* iproc)
{
	if(NULL==iproc)
		return RTIP_ERROR;

	if(iproc->len>1)
	{
		IMPROC_LOGE("Not support multiple frame rendering for now");
		return RTIP_ERROR;
	}

	return procMng->Render(iproc->iBuf[0]);
}

unsigned int RTIP_GetNumEffects(unsigned int* retNum)
{
	return procMng->GetNumEffects(retNum);
}

unsigned int RTIP_SetActiveEffect(unsigned int setIdx)
{
	return procMng->SetActiveEffect(setIdx);
}

unsigned int RTIP_GetEffectName(unsigned int getIdx, RTIPGetNameStruct* retName)
{
	if(NULL==retName)
		return RTIP_ERROR;
	
	procMng->GetEffectName(getIdx, &retName->name[0]);

	return RTIP_SUCCESS;
}

unsigned int RTIP_SetActivePropertyFArray128(const RTIPPropFArray128* setArray)
{
	return RTIP_SUCCESS;
}
