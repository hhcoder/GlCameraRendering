#ifndef _IMPROCCOLORTRANS_H_
#define _IMPROCCOLORTRANS_H_

#ifdef __cplusplus
extern "C" {
#endif

void yyvu2rgb(unsigned char  *p_luma,
              unsigned char  *p_rgba,
              unsigned int width, 
              unsigned int height);

void rgb2yyvu(unsigned char *p_input,
              unsigned char *p_output,
              unsigned int width,
              unsigned int height);

void yyvu2bgra(unsigned char  *p_luma,
               unsigned char  *p_rgba,
               unsigned int width, 
               unsigned int height);

void yuvyuv2yyuv(unsigned char* yuvyuv, 
				unsigned char* yyuv, 
				unsigned int width, 
				unsigned int height);

#ifdef __cplusplus
}
#endif

#endif //_IMPROCCOLORTRANS_H_
