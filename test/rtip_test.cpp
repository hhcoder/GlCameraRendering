#include "ImProc.h"

#include <stdio.h>
#include <stdlib.h>

class ImYuv
{
public:
    ImYuv(unsigned int iwidth, unsigned int iheight)
        :   w(iwidth),
            h(iheight),
            sz(w*h*3/2),
            buf(NULL)
    {
    }

    ~ImYuv()
    {
        DeAlloc();
    }

    unsigned int Read(const char* fname)
    {
        FILE* fp = fopen(fname, "rb");
        if(NULL==fp)
        {
            return 0;
        }

        Allocate();
    
        if(NULL!=buf)
            fread(buf, sizeof(unsigned char), sz, fp);
        fclose(fp);
        return 1;
    }

    unsigned int Write(const char* fname)
    {
        FILE* fp = fopen(fname, "wb");
        if(NULL!=buf)
            fwrite(buf, sizeof(unsigned char), sz, fp);
        fclose(fp);
        return 1;
    }

    unsigned char* GetBuf(void)
    {
        return buf;
    }
private:
    unsigned int Allocate(void)
    {
        if(NULL==buf)
        {
            buf = new unsigned char[sz];
        }

        if(NULL==buf)
            return 0;

        return 1;
    }

    unsigned int DeAlloc(void)
    {
        if(NULL!=buf)
        {
            delete[] buf;
        }
        buf = NULL;
        return 1;
    }

private:
    unsigned int    w;
    unsigned int    h;
    unsigned int    sz;
    unsigned char*  buf;
};

int main(int argc, char* argv[])
{
	const char* ifname = argv[1];
	unsigned int iwidth = atoi(argv[2]);
	unsigned int iheight = atoi(argv[3]);
	unsigned int iidx = atoi(argv[4]);
	const char* ofname = argv[5];

	printf("Cmdline: ifname=%s, iwidth=%s, iheight=%s, iidx=%d, ofname=%s\n", ifname, argv[2], argv[3], iidx, ofname);

    ImYuv* im = new ImYuv(iwidth, iheight);

    im->Read(ifname);

	RTIPSetInitStruct init;
	init.iFrameW = iwidth;
	init.iFrameH = iheight;
	RTIP_Init(&init);
	
	unsigned int numeffects;
	RTIP_GetNumEffects(&numeffects);
	printf("Support effect number: %d", numeffects);

	RTIP_SetActiveEffect(iidx);

	RTIPRenderStruct rend;
	rend.iBuf[0] = im->GetBuf();
	rend.len = 1;

	RTIP_RenderImg(&rend);
	
	RTIP_Exit();

    im->Write(ofname);

    delete im;


	return 0;
}
