#ifndef _IMPROCARM_LUT_H_
#define _IMPROCARM_LUT_H_

#include <stdio.h>
#include <stdlib.h>

namespace ImProc
{

#define pixel_range 	(256)
#define pixel_max  		(255)
#define pixel_min 		(0)

class PixelLut
{
public:
    PixelLut()
        : sz(pixel_range)
    {
        buf = new unsigned char[sz];
        for(unsigned int i=0; i<sz; i++)
            buf[i] = i;
    }

    ~PixelLut()
    {
        delete[] buf;
    }

    inline unsigned char Calc(unsigned char val)
    {
        return buf[val];
    }

    void SmoothStep(
    	const unsigned char p1, 
    	const unsigned char p2, 
    	const unsigned char v1, 
    	const unsigned char v2)
    {
        int val;
        for(unsigned int i=0; i<sz; i++)
        {
            if(i<=p1)
            {
                val = v1;
            }
            else
            {
                if (i>=p2)
                {
                    val = v2;
                }
                else
                {
                    val = ((i-p1)*(v2-v1)/(p2-p1))+v1;
                }
            }
            buf[i] = Clamp(val);
        }
    }


public:
	//Debug use
    void Write(const char* fname)
    {
        FILE* fp = fopen(fname, "wb");
        fprintf(fp,"lut=[\n");
        for(unsigned int i=0; i<sz; i++)
        {
            fprintf(fp,"%d ", buf[i]);
        }
        fprintf(fp,"];\n");
        fprintf(fp,"plot(lut);\n");
        fprintf(fp, "axis([0 255 0 255]);\n");
        fclose(fp);
    }

private:
    inline unsigned char Clamp(int val)
    {
        if(val>pixel_max)
            return (unsigned char)pixel_max;
        else 
            if(val<pixel_min)
                return (unsigned char)pixel_min;
            else
                return (unsigned char)val;
    }

private:
    unsigned char*       buf;
    unsigned int        sz;
};

}

#endif
