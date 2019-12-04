#ifndef FRAMELOADER_H
#define FRAMELOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef FRMADRENOSDKEXAMPLE

#include <FrmPlatform.h>

#else

//Common data type definition
typedef unsigned char      BYTE;

typedef char               CHAR;
typedef char               CHAR8;
typedef unsigned char      UCHAR;
typedef unsigned char      UCHAR8;

typedef wchar_t            WCHAR;
typedef unsigned short     UCHAR16;

typedef signed char        INT8;
typedef signed short       INT16;
typedef signed int         INT32;
typedef unsigned char      UINT8;
typedef unsigned short     UINT16;
typedef unsigned int       UINT32;

typedef float              FLOAT32;
typedef double             FLOAT64;

typedef int                BOOL;

#ifndef VOID
    #define VOID void
#endif

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

#ifndef NULL
    #define NULL 0
#endif

//Format Definition from OpenGL ES
/* PixelFormat */
#define GL_DEPTH_COMPONENT                0x1902
#define GL_ALPHA                          0x1906
#define GL_RGB                            0x1907
#define GL_RGBA                           0x1908
#define GL_LUMINANCE                      0x1909
#define GL_LUMINANCE_ALPHA                0x190A

/* DataType */
#define GL_BYTE                           0x1400
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_SHORT                          0x1402
#define GL_UNSIGNED_SHORT                 0x1403
#define GL_INT                            0x1404
#define GL_UNSIGNED_INT                   0x1405
#define GL_FLOAT                          0x1406
#define GL_FIXED                          0x140C

#endif

namespace FrameLoaderConst
{
    enum FrameStatus
    {
        UNINIT,
        CLEAR,
        LOADED,
        LOAD_FAIL,
        GENERATED,
        GENERATE_FAIL,
        STATUS_COUNT
    };

    enum GenFrameType
    {
        BW_CHECKERBOARD,
        MCBETH_COLORCHECKER,
    };

}

class FrameT
{
public:
    FrameT()
        : 	m_nWidth(0), 
        	m_nHeight(0), 
        	m_nSize(0), 
        	m_nStatus(FrameLoaderConst::UNINIT), 
        	m_pBuf(NULL),
        	m_nColorType(GL_RGB),
        	m_nDataType(GL_UNSIGNED_BYTE)
    {
    }

private:
    void InitVal(UINT32 iw, UINT32 ih, UINT32 icolor)
    {
    	UINT32 nChannel;

    	m_nColorType = icolor;
    	switch(m_nColorType)
    	{
    		case GL_RGB:
	    		nChannel = 3;
	    		break;
	    	case GL_LUMINANCE:
	    		nChannel = 1;
	    		break;
    	}
    
        m_nWidth = iw;
        m_nHeight = ih;
        m_nSize = iw*ih*nChannel;
    }

public:
    void AssignBuf(UINT32 iw, UINT32 ih, UINT32 icolor, BYTE* ibuf)
    {
        InitVal(iw, ih, icolor);

        if(NULL!=m_pBuf)
            delete[] m_pBuf;

        m_pBuf = ibuf;
    }

    void AllocBuf(UINT32 iw, UINT32 ih, UINT32 icolor)
    {
        InitVal(iw, ih, icolor);

        if(NULL!=m_pBuf)
          delete[] m_pBuf;

        m_pBuf = new BYTE[m_nSize]; 
    }

    void LoadFrmAssets(UINT32 iw, UINT32 ih, const char* fname, UINT32 icolor)
    {
        using namespace FrameLoaderConst;

        AllocBuf(iw, ih, icolor);

        if(NULL==fname)
        {
            memset(m_pBuf, 0x00, sizeof(BYTE)*m_nSize);
            m_nStatus = CLEAR;
        }
        else
        {
#ifdef FRMADRENOSDKEXAMPLE
			FRM_FILE* pFile;
			FrmFile_Open( fname, FRM_FILE_READ, &pFile );
			if ( 0 > FrmFile_Read(pFile, (VOID*)m_pBuf, sizeof(BYTE)*m_nSize) )
			{
                memset(m_pBuf, 0xAA, sizeof(BYTE)*m_nSize);
                m_nStatus = LOAD_FAIL;
			}
			else
			{
                m_nStatus = LOADED;
			}
			FrmFile_Close( pFile );
#else
		    
            FILE* fp;
            fp = fopen(fname, "rb");
            if(NULL!=fp)
            {
                fread(m_pBuf, sizeof(BYTE), m_nSize, fp);
                m_nStatus = LOADED;
            }
            else
            {
                memset(m_pBuf, 0xAA, sizeof(BYTE)*m_nSize);
                m_nStatus = LOAD_FAIL;
            }
            fclose(fp);
#endif
        }
    }

    BYTE* GetBuf(void)
    {
        return m_pBuf;
    }

    UINT32 GetSize(void)
    {
	    return m_nSize;
    }

    UINT32 GetWidth(void)
    {
	    return m_nWidth;
    }

    UINT32 GetHeight(void)
    {
		return m_nHeight;
    }

    UINT32 GetColorType(void)
    {
    	return m_nColorType;
    }

    UINT32 GetDataType(void)
    {
    	return m_nDataType;
    }

    void Write(const char* fname)
    {
#ifdef FRMADRENOSDKEXAMPLE
	// There's no WRITE function in Adreno SDK
#endif
		//Make sure write to /data
        FILE* fp;
        fp = fopen(fname, "wb");
        if(NULL!=fp)
            fwrite(m_pBuf, sizeof(BYTE), m_nSize, fp);
        fclose(fp);
    }

    ~FrameT()
    {
        delete[] m_pBuf;
    }

public:
    FrameLoaderConst::FrameStatus    m_nStatus;

private:
    UINT32      m_nWidth;
    UINT32      m_nHeight;
    UINT32      m_nSize;
    UINT32		m_nColorType;
    UINT32		m_nDataType;
    BYTE*       m_pBuf;

};

class CFrameContainer
{
public:
    CFrameContainer()
        :   m_nIdx(0),
            m_nNumFrames(0),
            m_FrameVec(NULL)
    {}

    ~CFrameContainer()
    {
        Clear();
    }

    void Init(INT32 inum)
    {
        m_nNumFrames = inum;

        if(m_FrameVec!=NULL)
            Clear();

        m_FrameVec = new FrameT[m_nNumFrames];

        m_nIdx=0;
    }

    void Clear(void)
    {
        delete[] m_FrameVec;
    }

    virtual FrameT* GetFrameT() = 0;

    void IdxInc(INT32 inc)
    {
        m_nIdx = (m_nIdx+inc)%m_nNumFrames;
    }

protected:
    UINT32      m_nNumFrames;
    UINT32      m_nIdx;
    FrameT*     m_FrameVec;
};

class CFileFrameLoader : protected CFrameContainer
{
public:
    CFileFrameLoader()
    {
    }

    ~CFileFrameLoader()
    {
    }

    void LoadFArrayStart(UINT32 inum)
    {
        Init(inum);
    }

    void LoadFile(UINT32 iw, UINT32 ih, const char* fname, UINT32 icolor)
    {
        FrameT* ft = &m_FrameVec[m_nIdx];
	    ft->LoadFrmAssets(iw, ih, fname, icolor);
        IdxInc(1);
    }

    void LoadFArrayEnd(void)
    {
        if(m_nIdx!=0)
        {
            //something wrong happened!
            //but, still, you might see something
        }
    }

    virtual FrameT* GetFrameT(void)
    {
		static INT32 nInc = 1;
        FrameT* ft = &m_FrameVec[m_nIdx];

        if(m_nIdx==(m_nNumFrames-1))
        	nInc = -1;	//bounce back
        if(m_nIdx==0)
            nInc = 1;
        IdxInc(nInc);
        
        return ft;
    }
};

class CGenFrameLoader : protected CFrameContainer
{
public:
    CGenFrameLoader()
    {
    }

    void LoadGenFrameStart()
    {
        Init(1);
    }

    void LoadGenType(
            UINT32 iw, 
            UINT32 ih,
            FrameLoaderConst::GenFrameType eGenType)
    {
        FrameT* ft = &m_FrameVec[m_nIdx];
        ft->AllocBuf(iw, ih, GL_RGB);
        SetGenType(ft, eGenType);
    }

     void LoadGenFrameEnd()
    {
        //Does nothing
    }

   void SetGenType(FrameT* ft, FrameLoaderConst::GenFrameType eGenType)
    {
        using namespace FrameLoaderConst;

       switch(eGenType)
        {
            case BW_CHECKERBOARD:
                GenerateBwCheckerBoard(ft, 40, 40, 0xFF, 0x00);
                break;

            case MCBETH_COLORCHECKER:
                break;
        }
    }

    virtual FrameT* GetFrameT(void)
    {
        return &m_FrameVec[m_nIdx];
    }

private:
    void GenerateBwCheckerBoard(
            FrameT* ft,
            UINT32 nBlockW, 
            UINT32 nBlockH, 
            UINT8 nColorFore, 
            UINT8 nColorBack)
    {

	    UINT8 fore, back;
    	UINT32 inc;
        UINT32 i,j;
        UINT32 nDimH = ft->GetHeight();
        UINT32 nDimW = ft->GetWidth();
	    UINT8* ptr = ft->GetBuf();
        UINT32 CHANNELS=3;

        j=0;
        while(j<nDimH)
        {
            if((j/nBlockH)%2==0)
            {
                fore = nColorFore; back = nColorBack;
            }
            else
            {
                fore = nColorBack; back = nColorFore;
            }

            i=0;
            while(i<nDimW)
            {
                if((i+nBlockW)>nDimW)
                    inc = nDimW-i;
                else
                    inc = nBlockW;
                i+=inc;

                memset(ptr, fore, inc*CHANNELS);
                ptr+=inc*CHANNELS;

                if((i+nBlockW)>nDimW)
                    inc = nDimW-i;
                else
                    inc = nBlockW;
                i+=inc;

                memset(ptr, back, inc*CHANNELS);
                ptr+=inc*CHANNELS;
            }
            j++;
        }
    }
};

class CCameraFrameLoader : protected CFrameContainer
{
public:
    CCameraFrameLoader()
    {
    }

    ~CCameraFrameLoader()
    {
    }

    void LoadCameraStart()
    {
        Init(1);
    }

    void LoadCameraFrame()
    {
        const UINT32 nPrevWidth = 640;
        const UINT32 nPrevHeight = 480;
        const UINT32 nPrevSize = nPrevWidth*nPrevHeight;
        const UINT32 nColor = GL_LUMINANCE;
        BYTE* pPrevFrame;

        FrameT* ft;

        ft = &(m_FrameVec[m_nIdx]);

        // Initialize the camera
        
        // Get preview frame from somewhere 
        
        // for now, just create a gray frame
        pPrevFrame = new BYTE[nPrevSize];
        memset(pPrevFrame, 0x33, sizeof(BYTE)*nPrevSize);

        ft->AssignBuf(nPrevWidth, nPrevHeight, nColor, pPrevFrame);
    }

    void LoadCameraEnd()
    {
    }

    virtual FrameT* GetFrameT(void)
    {
        return &m_FrameVec[m_nIdx];
    }

private:
    BYTE*       pPrevFrame;
};

#endif //FRAMELOADER_H
