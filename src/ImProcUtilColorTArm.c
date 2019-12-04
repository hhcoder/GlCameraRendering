#include "ImProcUtilColorT.h"

typedef short          int16_t;
typedef int            int32_t;
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

#define _CLAMP(x) ( ((x) & 0xFFFFFF00) ? ((~(x)>>31) & 0xFF) : (x))

void yyvu2rgb(unsigned char  *p_luma,
              unsigned char  *p_rgba,
              unsigned int width, 
              unsigned int height)
{
    unsigned int i, j;

    signed long  C, D, E;
    signed long  sub_chroma_r, sub_chroma_g, sub_chroma_b, sub_luma;

    unsigned char  *p_chroma = p_luma + width*height;

    for (j = 0; j < height/2; j++)
    {
            unsigned char* p_chroma1 = p_chroma;

            for (i = 0; i < width/2; i++)
            {
                // first pixel
                C = *(p_luma++);
                E = (*p_chroma1++) - 128;
                D = (*p_chroma1++) - 128;

                sub_chroma_r =           359*E + 128;
                sub_chroma_g = (-88)*D - 183*E + 128;
                sub_chroma_b =   454*D         + 128;

                sub_luma = 256*C;

                *(p_rgba++) = (unsigned char) (_CLAMP ((sub_luma + sub_chroma_r) >> 8));
                *(p_rgba++) = (unsigned char) (_CLAMP ((sub_luma + sub_chroma_g) >> 8));
                *(p_rgba++) = (unsigned char) (_CLAMP ((sub_luma + sub_chroma_b) >> 8));
                //*(p_rgba++) = 0xFF;

                // second pixel, re-use sub_chroma
                C         = *(p_luma++);
                sub_luma  = 256*C;

                *(p_rgba++) = (unsigned char) (_CLAMP ((sub_luma + sub_chroma_r) >> 8));
                *(p_rgba++) = (unsigned char) (_CLAMP ((sub_luma + sub_chroma_g) >> 8));
                *(p_rgba++) = (unsigned char) (_CLAMP ((sub_luma + sub_chroma_b) >> 8));
                //*(p_rgba++) = 0xFF;
            }

            p_chroma1 = p_chroma;
            for (i = 0; i < width/2; i++)
            {
                // first pixel
                C = *(p_luma++);
                E = (*p_chroma1++) - 128;
                D = (*p_chroma1++) - 128;

                sub_chroma_r =           359*E + 128;
                sub_chroma_g = (-88)*D - 183*E + 128;
                sub_chroma_b =   454*D         + 128;

                sub_luma = 256*C;

                *(p_rgba++) = (unsigned char) (_CLAMP ((sub_luma + sub_chroma_r) >> 8));
                *(p_rgba++) = (unsigned char) (_CLAMP ((sub_luma + sub_chroma_g) >> 8));
                *(p_rgba++) = (unsigned char) (_CLAMP ((sub_luma + sub_chroma_b) >> 8));
                //*(p_rgba++) = 0xFF;

                // second pixel, re-use sub_chroma
                C         = *(p_luma++);
                sub_luma  = 256*C;

                *(p_rgba++) = (unsigned char) (_CLAMP ((sub_luma + sub_chroma_r) >> 8));
                *(p_rgba++) = (unsigned char) (_CLAMP ((sub_luma + sub_chroma_g) >> 8));
                *(p_rgba++) = (unsigned char) (_CLAMP ((sub_luma + sub_chroma_b) >> 8));
                //*(p_rgba++) = 0xFF;
            }
            p_chroma = p_chroma1;
    }
}

void rgb2yyvu(unsigned char *p_input,
              unsigned char *p_output,
              unsigned int width,
              unsigned int height)
{
    uint32_t  line_width;
    uint8_t  *p_luma   = p_output;
    uint8_t  *p_chroma = p_output + width * height;
    uint8_t   R, G, B;
    uint32_t  i, j;
    
    // Outer loop process two lines each iteration
    for (i = 0; i < height; i += 2)
    {
        // The first inner loop process two horizontal RGB pixels each iteration,
        // and save the converted two Y pixels and one CrCb pixel pair
        for (j = 0; j < width; j += 2)
        {
            R = *(p_input++);   // obtain the B pixel
            G = *(p_input++);   // obtain the G pixel
            B = *(p_input++);   // obtain the R pixel
            //p_input++;          // skip the A pixel

            // output the first Y pixel and the first CrCb pixel pair
            *(p_luma++)   = ( (  66 * R + 129 * G +  25 * B + 128 ) >> 8 ) +  16; // Y
            *(p_chroma++) = ( ( 112 * R -  94 * G -  18 * B + 128 ) >> 8 ) + 128; // Cr
            *(p_chroma++) = ( ( -38 * R -  74 * G + 112 * B + 128 ) >> 8 ) + 128; // Cb  

            R = *(p_input++);   // obtain the B pixel
            G = *(p_input++);   // obtain the G pixel
            B = *(p_input++);   // obtain the R pixel
            //p_input++;          // skip the A pixel

            // output the second Y pixel
            *(p_luma++)   = ( ( 66 * R + 129 * G +  25 * B + 128 ) >> 8 ) +  16; // Y
        }

        // The second inner loop process two horizontal RGB pixels each iteration,
        // and save the converted two Y pixels only       
        for (j = 0; j < width; j += 2)
        {
            R = *(p_input++);   // obtain the B pixel
            G = *(p_input++);   // obtain the G pixel
            B = *(p_input++);   // obtain the R pixel
            //p_input++;          // skip the A pixel

            // output the third Y pixel 
            *(p_luma++)   = ( ( 66 * R + 129 * G +  25 * B + 128 ) >> 8 ) +  16; // Y			

            R = *(p_input++);   // obtain the B pixel
            G = *(p_input++);   // obtain the G pixel
            B = *(p_input++);   // obtain the R pixel
            //p_input++;          // skip the A pixel

            //
            // output the forth Y pixel
            *(p_luma++)   = ( ( 66 * R + 129 * G +  25 * B + 128 ) >> 8 ) +  16; // Y
        }

        //printf("%d\n", i);
    }
}

void yyvu2bgra(unsigned char  *p_luma,
               unsigned char  *p_rgba,
               unsigned int width, 
               unsigned int height)
{
    unsigned int i, j;

    signed long  C, D, E;
    signed long  sub_chroma_r, sub_chroma_g, sub_chroma_b, sub_luma;
    unsigned char  *p_chroma = p_luma + width*height;

    for (j = 0; j < height/2; j++)
    {
            unsigned char* p_chroma1 = p_chroma;

            for (i = 0; i < width/2; i++)
            {
                // first pixel
                C = *(p_luma++);
                E = (*p_chroma1++) - 128;
                D = (*p_chroma1++) - 128;

                sub_chroma_b =   454*D         + 128;
                sub_chroma_g = (-88)*D - 183*E + 128;
                sub_chroma_r =           359*E + 128;

                sub_luma = 256*C;

                *(p_rgba++) = (unsigned char) (_CLAMP ((sub_luma + sub_chroma_b) >> 8));
                *(p_rgba++) = (unsigned char) (_CLAMP ((sub_luma + sub_chroma_g) >> 8));
                *(p_rgba++) = (unsigned char) (_CLAMP ((sub_luma + sub_chroma_r) >> 8));
                *(p_rgba++) = 0xFF;

                // second pixel, re-use sub_chroma
                C         = *(p_luma++);
                sub_luma  = 256*C;

                *(p_rgba++) = (unsigned char) (_CLAMP ((sub_luma + sub_chroma_b) >> 8));
                *(p_rgba++) = (unsigned char) (_CLAMP ((sub_luma + sub_chroma_g) >> 8));
                *(p_rgba++) = (unsigned char) (_CLAMP ((sub_luma + sub_chroma_r) >> 8));
                *(p_rgba++) = 0xFF;
            }

            p_chroma1 = p_chroma;
            for (i = 0; i < width/2; i++)
            {
                // first pixel
                C = *(p_luma++);
                E = (*p_chroma1++) - 128;
                D = (*p_chroma1++) - 128;

                sub_chroma_b =   454*D         + 128;
                sub_chroma_g = (-88)*D - 183*E + 128;
                sub_chroma_r =           359*E + 128;

                sub_luma = 256*C;

                *(p_rgba++) = (unsigned char) (_CLAMP ((sub_luma + sub_chroma_b) >> 8));
                *(p_rgba++) = (unsigned char) (_CLAMP ((sub_luma + sub_chroma_g) >> 8));
                *(p_rgba++) = (unsigned char) (_CLAMP ((sub_luma + sub_chroma_r) >> 8));
                *(p_rgba++) = 0xFF;

                // second pixel, re-use sub_chroma
                C         = *(p_luma++);
                sub_luma  = 256*C;

                *(p_rgba++) = (unsigned char) (_CLAMP ((sub_luma + sub_chroma_b) >> 8));
                *(p_rgba++) = (unsigned char) (_CLAMP ((sub_luma + sub_chroma_g) >> 8));
                *(p_rgba++) = (unsigned char) (_CLAMP ((sub_luma + sub_chroma_r) >> 8));
                *(p_rgba++) = 0xFF;
            }
            p_chroma = p_chroma1;
    }
}

void yuvyuv2yyuv(unsigned char* yuvyuv, unsigned char* yyuv, unsigned int width, unsigned int height)
{
    unsigned char* psrc;
    unsigned char* py;
    unsigned char* puv;
    unsigned int i, j;

    psrc = yuvyuv;

    py = yyuv;
    puv = yyuv + width*height;

    for(j=0; j<height; j++)
    {
        for(i=0; i<width; i++)
        {
            *py = *psrc; 
            py++;

            if(0==i%2 && 0==j%2)
            {
                *puv = *(psrc+1);
                puv++;
                *puv = *(psrc+2);
                puv++;
            }

            psrc+=3;
        }
    }

}

