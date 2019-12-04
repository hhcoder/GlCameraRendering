#ifndef _CL_BUFCOPY_H_
#define _CL_BUFCOPY_H_

void CLLibInit(unsigned int iw, unsigned int ih );
int CLLibUpdate(unsigned int iw, unsigned int ih, unsigned char* img_in);
void CLLibReadBuf(unsigned int isz, unsigned char* out);
int CLLibExit();

int cl_copy_main(
   const unsigned int iw, 
   const unsigned int ih,
   unsigned char* img_in);

#endif //_CL_BUFCOPY_H_
