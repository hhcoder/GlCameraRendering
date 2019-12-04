#ifndef _IMPROC_H_
#define _IMPROC_H_

#ifdef __cplusplus
extern "C" {
#endif

// ================================================= //
//                    Constants                      //
// ================================================= //

#define RTIP_SUCCESS					(1)
#define RTIP_ERROR						(0)

#define RTIP_STR_MAXLEN					(64)

// Maximal number of effects 
#define RTIP_EFFECT_NUM_MAX				(32)

#ifndef NULL
#define NULL							((void*)0)
#endif

// ================================================= //
//              Constructor / Destructor             //
// ================================================= //

typedef struct _rtipsetinitstruct
{
	unsigned int 		iFrameW;
	unsigned int 		iFrameH;
} RTIPSetInitStruct;

unsigned int RTIP_Init(const RTIPSetInitStruct* setup);

unsigned int RTIP_Exit(void);

// ================================================= //
//                    Image Process                  //
// ================================================= //

#define RTIP_RENDER_IMAGES_MAX			(8)

typedef struct _rtiprenderstruct
{
	unsigned char*	 	iBuf[RTIP_RENDER_IMAGES_MAX];
	unsigned int		len;
}RTIPRenderStruct;

unsigned int RTIP_RenderImg(const RTIPRenderStruct* proc);

// ================================================= //
//           Choose in between Effects               //
// ================================================= //

unsigned int RTIP_GetNumEffects(unsigned int* retNum);

unsigned int RTIP_SetActiveEffect(unsigned int setIdx);

// ================================================= //
//              Get Name Information                 //
// ================================================= //

typedef struct _rtipgetnamestruct
{
	char				name[RTIP_STR_MAXLEN];
} RTIPGetNameStruct;

unsigned int RTIP_GetEffectName(unsigned int getIdx, RTIPGetNameStruct* retName);

// ================================================= //
//              Set Effect Property                  //
// ================================================= //

typedef struct _rtipproparray
{
	float		array[128];
}RTIPPropFArray128;

unsigned int RTIP_SetActivePropertyFArray128(const RTIPPropFArray128* setArray);

// Possible future extension?
// unsigned int RTIP_SetActivePropertyArray512(RTIPPropFArray512* setArray);

#ifdef __cplusplus
}
#endif


#endif
