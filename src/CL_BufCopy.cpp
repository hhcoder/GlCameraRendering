#include "CL_BufCopy.h"

// common SDK header for standard utilities and system libs 
//#include <oclUtils.h>
#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 

static cl_command_queue CommandQueue;// OpenCL command queue

static cl_context cxGPUContext;        // OpenCL context
static cl_platform_id cpPlatform;      // OpenCL platform
static cl_device_id cdDevice;          // OpenCL device

static cl_mem cl_input;
static cl_mem cl_output;     

typedef struct KERNEL_INFO
{
    cl_kernel kernel;
    char name[60];
    size_t WGsize;
    cl_ulong localMemory;
    size_t compiler_WGsize[3];
}KernelInfo;

typedef enum  ENUM_KERNEL{
    enum_forward_haar_img, 
    enum_inverse_haar_img, 
    enum_inverse_haar_img_buffer, 
    enum_forward_haar_img_buffer
}enumKernels;

char* oclLoadProgSource(const char* cFilename)
{
    // locals 
    FILE* pFileStream = NULL;

    //Replaced fopen_s with fopen for building in Unix OS.
    pFileStream = fopen(cFilename, "r");
    if(pFileStream == NULL) 
    {       
    	printf("Error in opening file: %s", cFilename);
        return NULL;
    } 

	size_t szSourceLength;

    fseek(pFileStream, 0, SEEK_END); 
    szSourceLength = ftell(pFileStream);

    // allocate a buffer for the source code string and read it in
    char* cSourceString = (char *)malloc(szSourceLength + 1); 

    if(cSourceString == NULL)
    {
	    printf("Not enough space for allocating %d of string", szSourceLength + 1);
        fclose(pFileStream); 
        return 0;
    }  

    fseek(pFileStream, 0, SEEK_SET); 
    if (fread((cSourceString), szSourceLength, 1, pFileStream) != 1)
    {
	    printf("Error in reading file: %s", cFilename);
        fclose(pFileStream);
        free(cSourceString);
        return 0;
    }

    fclose(pFileStream);

    cSourceString[szSourceLength] = '\0';

    return cSourceString;
}


void cmdque_exit(void)
{
    if(CommandQueue)
		clReleaseCommandQueue(CommandQueue);  
}

void cmdque_init(void)
{
	cl_int clErr;

    fprintf(stderr, "clCreateCommandQueue...\n");  

    // Create a command-queue for each color component 
    CommandQueue = clCreateCommandQueue(cxGPUContext,
        cdDevice, 
        CL_QUEUE_PROFILING_ENABLE, // CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE.  Only two properties are supported in OpenCL createCommandQ. 
        &clErr);

	if(CL_SUCCESS != clErr)
	{
        fprintf (stderr, "\nErr in clCreateCommandQueue!\n");
	}
}

void mem_init(unsigned int iw, unsigned int ih)
{
    fprintf(stderr,  "Allocate and Init Host Mem...\n"); 

}

void mem_exit(void)
{
    // Free host memory
}


void program_buildkernel(KernelInfo *currKernelStr, const char * Kernelname, cl_program Program4theKernel)  
{ 
	cl_int clErr;

    currKernelStr->kernel = clCreateKernel(Program4theKernel, Kernelname, &clErr);  
    if(clErr!= CL_SUCCESS)
    {
        fprintf(stderr, "fail to get the kernel information\n");
		return;
    }

	strcpy(currKernelStr->name, Kernelname);

    clErr = clGetKernelWorkGroupInfo(currKernelStr->kernel,
        cdDevice, 
        CL_KERNEL_WORK_GROUP_SIZE, 
        sizeof( currKernelStr->WGsize ),
        &(currKernelStr->WGsize), NULL );

    if(clErr!= CL_SUCCESS)
    {
        fprintf(stderr, "fail to get the kernel information\n");
		return;
    }

    clErr = clGetKernelWorkGroupInfo(currKernelStr->kernel,
        cdDevice, 
        CL_KERNEL_LOCAL_MEM_SIZE, 
        sizeof( currKernelStr->localMemory ),
        &(currKernelStr->localMemory), NULL );

    if(clErr!= CL_SUCCESS)
    {
        fprintf(stderr, "fail to get the kernel information\n");
		return;
    }

    clErr = clGetKernelWorkGroupInfo(currKernelStr->kernel,
        cdDevice, 
        CL_KERNEL_COMPILE_WORK_GROUP_SIZE, 
        3 * sizeof( size_t ),
        currKernelStr->compiler_WGsize, NULL );

    if(clErr != CL_SUCCESS)
    {
        fprintf(stderr, "fail to get the kernel information\n");
		return;
    } 

    fprintf(stderr, "WGsize of kernel %s: %d\n", Kernelname, currKernelStr->WGsize);
}

#define NUM_OF_KERNELS 1
static KernelInfo StrArayKernels;  

cl_program cpProgramList;

void program_exit(void)
{
    if(StrArayKernels.kernel)
    {
        if(clReleaseKernel(StrArayKernels.kernel) != CL_SUCCESS)
        {
            fprintf(stderr, "  clReleaseKernel unsuccessful...\n\n"   );
        }
    }

    if(cpProgramList)
    {
        if(clReleaseProgram(cpProgramList) != CL_SUCCESS)
        {
            fprintf(stderr, "  clReleaseProgram unsuccessful...\n\n"   );
        }
    }
}

void program_init(void)
{
	cl_int clErr;

	size_t szKernelLength;			// Byte size of kernel code
	char	* cSourceCL = NULL;         // Buffer to hold source for compilation 
	char	* cBinaryCL = NULL;         // Buffer to hold source for compilation 

	const char* cSourceFileList = "/data/HaarTransform_haar_kernel.cl";
  	const char *cKernelNames[4] = {"forward_haar", "forward_haar", "forward_haar", "forward_haar"};
	const char* cKernelName = "forward_haar";

	#define USE_SOURCE (1)
    if (USE_SOURCE) 
    {  
        // Read the OpenCL kernel in from source file
        fprintf(stderr, "oclLoadProgSource (%s)...\n", cSourceFileList); 

        cSourceCL = oclLoadProgSource(cSourceFileList);

        if (cSourceCL == NULL)
        {
            fprintf(stderr, "Error with clCreateProgramWithSource... %s\n", cSourceFileList); 
            program_exit();
			return;
        }

        // Create the program	
        fprintf(stderr, "clCreateProgramWithSource...\n"); 
        cpProgramList = clCreateProgramWithSource(cxGPUContext, 1, (const char **)&cSourceCL, &szKernelLength, &clErr);

	    if(cSourceCL)
	        free(cSourceCL); 

        if (clErr != CL_SUCCESS)
        {
            fprintf(stderr, "Error in clCreateProgramWithSource, Line %u in file %s !!!\n\n", __LINE__, __FILE__);
            program_exit();
			return;
        }
    }

    // build the program
    fprintf(stderr, "clBuildProgram...\n"); 

    const char* buildOpts = "";  // build options. 

    if ((CL_SUCCESS != clBuildProgram(cpProgramList, 0, NULL, buildOpts, NULL, NULL)))
    {
        fprintf(stderr, "Error in clBuildProgram, Line %u in file %s !!!\n\n", __LINE__, __FILE__);
        program_exit();
		return;
    } 

    cl_build_status buildStatus;

    if (CL_SUCCESS != clGetProgramBuildInfo(cpProgramList, cdDevice, CL_PROGRAM_BUILD_STATUS, sizeof(buildStatus), &buildStatus, NULL))
	{
        fprintf(stderr, "Error in clGetProgramBuildInfo, Line %u in file %s !!!\n\n", __LINE__, __FILE__);
        program_exit();
		return;
    } 
	
    if(buildStatus != CL_BUILD_SUCCESS)
    {
        fprintf(stderr, "The program is not built (buildStatus), Line %u in file %s !!!\n\n", __LINE__, __FILE__);
        program_exit();
		return;
    }  

    // start to create kernels.  
    fprintf(stderr, "clCreateKernel (edge_weighting, epsilon_smooth, haar, ihaar )...\n");  

    program_buildkernel(&StrArayKernels, cKernelName, cpProgramList);  


    return; 
}

void buf_exit(void)
{
    if(cl_input) 
        clReleaseMemObject(cl_input);

    if(cl_output)
        clReleaseMemObject(cl_output);  
}

void buf_init(unsigned int iw, unsigned int ih)
{
	cl_int clErr;

	unsigned int isz = iw*ih;

    cl_output = clCreateBuffer(cxGPUContext, CL_MEM_WRITE_ONLY, sizeof(unsigned char) * isz, NULL, &clErr);
    if (clErr != CL_SUCCESS)
    {
        fprintf(stderr, "Error in clCreateCommandQueue, Line %u in file %s !!!\n\n", __LINE__, __FILE__);
        buf_exit();
		return;
    }
}

void clbuf_update(unsigned int iw, unsigned int ih, unsigned char* in)
{
	cl_int clErr;

	unsigned int isz = iw*ih;

    cl_input = clCreateBuffer(cxGPUContext, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(unsigned char) * isz, in, &clErr);
    if (clErr != CL_SUCCESS)
    {
        fprintf(stderr, "Error in clCreateCommandQueue, Line %u in file %s !!!\n\n", __LINE__, __FILE__);
        buf_exit();
		return;
    }
}

void context_exit(void)
{
    if(cxGPUContext)
        clReleaseContext(cxGPUContext); 
}

void context_init(void)
{
	cl_int clErr;

    fprintf(stderr, "clCreateContext...\n"); 

    cxGPUContext = clCreateContext(0, 1, &cdDevice, NULL, NULL, &clErr);
    if (clErr != CL_SUCCESS)
    {
        fprintf(stderr, "Error in clCreateContext, Line %u in file %s !!!\n\n", __LINE__, __FILE__);
        context_exit();
    } 
}

int CLLibExit()
{ 
    fprintf(stderr, "CLLibExit\n");

	program_exit();

	context_exit();

	buf_exit();

	cmdque_exit();

    exit(0);
}

void platform_init(void)
{
	cl_int clErr;

    // 1st step: check if how many platforms are available. 
	cl_uint numOfPlatforms;
    clErr = clGetPlatformIDs(0, //num_entries
        NULL,                  // *platforms
        &numOfPlatforms);   

    if(clErr != CL_SUCCESS)
    {
        fprintf(stderr, "Error in clGetPlatformIDs, Line %u in file %s !!!\n\n", __LINE__, __FILE__);
		return;
    }

    if (numOfPlatforms == 0)
    {
        fprintf(stderr, "No platform is available\n Exiting ...");
		return;
    }

    // 2nd step: query the platforms.
    cl_platform_id* platforms = new cl_platform_id[numOfPlatforms];

    clErr = clGetPlatformIDs(numOfPlatforms, platforms, NULL);

    if(clErr != CL_SUCCESS)
    {
        fprintf(stderr, "Error in clGetPlatformIDs, Line %u in file %s !!!\n\n", __LINE__, __FILE__);
    }

    if(platforms == NULL)
    {		
        fprintf(stderr, "No platform found, Line %u in file %s !!!\n\n", __LINE__, __FILE__);
    }

    char platformName[100];

    cpPlatform = NULL;

    for(unsigned i = 0; i < numOfPlatforms; i++)
    {
        clErr = clGetPlatformInfo(platforms[i], // platform ID
            CL_PLATFORM_VENDOR, // property to query
            sizeof(platformName),	// sizeof the string to store the query information
            platformName,			// ptr to the string to store the query information
            NULL);

        if(clErr != CL_SUCCESS)
        {
            fprintf(stderr, "Error in clGetPlatformInfo, Line %u in file %s !!!\n\n", __LINE__, __FILE__);
        }

        if (!strcmp(platformName, "Advanced Micro Devices, Inc.")) 
        {  
            cpPlatform = platforms[i];		
            break;
        }
        else if (!strcmp(platformName, "QUALCOMM")) 
        {  
            cpPlatform = platforms[i]; 
            break;
        }  
    } 

    if(cpPlatform)
    {
        printf("Platform found : %s \n", platformName);
    }
    else
    {
        printf("Desired platform not found\n Using default one\n");
        cpPlatform = platforms[0];
    }
    delete[] platforms; 
	
}

void device_init(void)
{
	cl_int clErr;

    int numDevices;
    int numCPUDevices;
    int numGPUDevices; 

    clErr = clGetDeviceIDs(cpPlatform,
        CL_DEVICE_TYPE_ALL, 
        0, 
        0, 
        (cl_uint*)&numDevices);

    if(numDevices == 0)
    {
        fprintf(stderr, "No device found! Line %u in file %s !!!\n\n", __LINE__, __FILE__);
        return;
    } 
	
    if( CL_SUCCESS != clGetDeviceIDs(cpPlatform,
	        CL_DEVICE_TYPE_CPU, 
	        0, 
	        0, 
	        (cl_uint*)&numCPUDevices) )
    {
    	fprintf(stderr, "Cannot get CPU device id \n");
		return;
    }
	
    if( CL_SUCCESS != clGetDeviceIDs(cpPlatform,
	        CL_DEVICE_TYPE_GPU, 
    	    0, 
        	0, 
	        (cl_uint*)&numGPUDevices) )
    {
    	fprintf(stderr, "Cannot get GPU device id \n");
		return;
   	}

    fprintf(stderr, "devices on the platform: ALL: %d, CPU: %d, GPU, %d\n", numDevices, numCPUDevices, numGPUDevices);
    fprintf(stderr, "clGetDeviceIDs...\n");  

    if( CL_SUCCESS != clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_ALL, 1, &cdDevice, NULL) )
    {
        fprintf(stderr, "Error in clGetDeviceIDs, Line %u in file %s !!!\n\n", __LINE__, __FILE__);
        return;
    }
}


void profiler_init(void)
{
    size_t profiling_resolution = 0;

    if( CL_SUCCESS != 
			clGetDeviceInfo(cdDevice,
		        CL_DEVICE_PROFILING_TIMER_RESOLUTION, // property to query
		        sizeof(profiling_resolution), 
		        &profiling_resolution, 
		        NULL) )
    {
        fprintf(stderr, "clGetDeviceInfo for CL_DEVICE_PROFILING_TIMER_RESOLUTION failed., Line %u in file %s !!!\n\n", __LINE__, __FILE__);
		return;
    }
}

void CLLibInit(unsigned int iw, unsigned int ih )
{
	platform_init();

	device_init();

    context_init();

	cmdque_init();

	program_init();

	profiler_init();

	buf_init(iw, ih);
}


void process_buf_copy(cl_kernel f_haar_kernel,
                          cl_mem input, 
                          cl_mem output, 
                          int widthinWG, 
                          cl_command_queue CmdQueue,
                          size_t *NumofWI, 
                          size_t *WGsize, 
                          cl_event *event)
{ 
	cl_int clErr;

    if( CL_SUCCESS != clSetKernelArg(f_haar_kernel, 0, sizeof(cl_mem), (void*)&input) )
    {
		fprintf(stderr, "Error in clSetKernelArg, Line %u in file %s !!!\n\n", __LINE__, __FILE__);
		return;
    }
		
    if( CL_SUCCESS != clSetKernelArg(f_haar_kernel, 1, sizeof(int), (void*)&widthinWG) )
    {
		fprintf(stderr, "Error in clSetKernelArg, Line %u in file %s !!!\n\n", __LINE__, __FILE__);
		return;
    }

    if( CL_SUCCESS != clSetKernelArg(f_haar_kernel, 2, sizeof(cl_mem), (void*)&output) )
    {
		fprintf(stderr, "Error in clSetKernelArg, Line %u in file %s !!!\n\n", __LINE__, __FILE__);
		return;
    }

    clErr = clEnqueueNDRangeKernel(
        CmdQueue,			/* command_queue */
        f_haar_kernel, 		/* kernel */
        2,					/* work_dim */
        NULL,				/* global_work_offset */
        NumofWI, 			/* global_work_size */
        WGsize,				/* local_work_size */
        0,					/* num_events_in_wait_list */
        NULL, 				/* event_wait_list */
        event);  		/* event */

    if( CL_SUCCESS != clErr)
    {
        fprintf(stderr, "Error in fdwt_haar_kernel, Line %u in file %s !!!\n\n", __LINE__, __FILE__);
        return;
    }
}

int CLLibUpdate(unsigned int iw, unsigned int ih, unsigned char* img_in)
{
	clbuf_update(iw, ih, img_in);
	
	cl_event event;

    size_t globalworksize[2] =  {iw,  ih};

    fprintf(stderr, "process_buf_copy\n");
    process_buf_copy(StrArayKernels.kernel, 
        cl_input,  
        cl_output,
        iw, 
        CommandQueue,
        globalworksize,
        NULL,
        &event);    

    clFinish(CommandQueue);
	fprintf(stderr, "after clFinish\n");

	//Profiling session

    cl_ulong startTime = 0;
	cl_ulong endTime = 0;

	//HH test
	clWaitForEvents(1, &event);

    while (clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_QUEUED, sizeof(cl_ulong), &startTime, NULL) ==
        CL_PROFILING_INFO_NOT_AVAILABLE );

    while (clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &endTime, NULL) ==
        CL_PROFILING_INFO_NOT_AVAILABLE ); 

    fprintf(stderr, "Kernel execution: %.5f s \n", 1.0e-9 * ((double)(endTime - startTime)) );

    clReleaseEvent(event);

	return 1;

}

void CLLibReadBuf(
   unsigned int iw, 
   unsigned int ih,
   unsigned char* out)
{
	unsigned int isz = iw*ih;
    memset(out, 0x00, sizeof(unsigned char)*isz);

    if( CL_SUCCESS != clEnqueueReadBuffer(
            CommandQueue, 
            cl_output, 
            CL_TRUE, 
            0, 
            sizeof(unsigned char ) * isz, 
            out, 0, NULL, NULL) )
    {
        printf("Error in readin buffer");
        return;
    }

}

void CLLibCopyBufTest(
	unsigned int iw, 
	unsigned int ih, 
	unsigned char* in)
{
	fprintf(stderr, "Performing Test...\n");

    int mismatches = 0;
	unsigned int isz = iw*ih;

	unsigned char* temp_imgbuf = (unsigned char*)malloc(sizeof(unsigned char)*isz);
    memset(temp_imgbuf, 0x00, sizeof(unsigned char)*isz);

	if(NULL==temp_imgbuf)
	{
		printf("Not enough memory for testing\n");
		return;
	}

	if( CL_SUCCESS != clEnqueueReadBuffer(
			CommandQueue, 
			cl_output, 
			CL_TRUE, 
			0, 
			sizeof(unsigned char ) * isz, 
			temp_imgbuf, 0, NULL, NULL) )
	{
		printf("Error in readin buffer");
		return;
	}
	
   	unsigned char* psrc = in;
	unsigned char* pdst = temp_imgbuf;
    for(unsigned int i = 0; i < isz; i++)
    {
        if(*psrc!=*pdst)
            mismatches++;
		psrc++;
		pdst++;
    }

   free(temp_imgbuf);

	// Silent if test passed
    if(mismatches)
    {
    	for(int i=0; i<3; i++)
	        printf ("*********************************\n");
        printf ("CLLibCopyBufTest failed: %d mismatches found!\n", mismatches);
    	for(int i=0; i<3; i++)
	        printf ("*********************************\n");
    }

	fprintf(stderr, "\n...End of CLLibCopyBufTest\n");

}


// const unsigned int iwidth = 640;
// const unsigned int iheight = 480*3/2;
// const char* ifname = "/data/src.yuv";

int FrameBufInit(unsigned char** img, unsigned int iwidth, unsigned int iheight)
{
	unsigned int isz = iwidth*iheight;
	
	unsigned char* img_in = (unsigned char *)malloc(sizeof(char)*isz);
    if(img_in == NULL)
    {
        fprintf (stderr, "\nNo sufficient memory (img_in)!\n");
		return -1;
    }

	*img = img_in;
	return 1;
}

int FrameBufUpdate(unsigned char* img_in, unsigned int iwidth, unsigned int iheight, const char* ifname)
{
    FILE* fp_in = fopen (ifname, "rb"); 
    if(NULL==fp_in)
    {
        fprintf(stderr, "Invalid fp_in.\n"); 
		return 0;
    }
	else
	{
		fread(img_in, sizeof (unsigned char), iwidth*iheight, fp_in);
	}
	fclose(fp_in);
	return 1;
}

int FrameBufClear(unsigned char* img)
{
	if(NULL!=img)
		free(img);

	return 1;
}

int cl_test_copy()
{    
	const char* ifname = "data/clsrc.yuv"; //argv[1];
	const unsigned int iwidth = 640; //atoi(argv[2]);
	const unsigned int iheight = 480*3/2; //atoi(argv[3])*3/2;
	const char* ofname = "data/cldst.yuv"; //argv[4];
	
	unsigned char* img_in = NULL;

	printf("\n Cmdline input: ifname, width, iheight, ofname = %s, %d, %d, %s\n", ifname, iwidth, iheight, ofname);

    //Read in
    img_in = (unsigned char *)malloc(sizeof(char)*iwidth*iheight);
    if(img_in == NULL)
    {
        fprintf (stderr, "\nNo sufficient memory (img_in)!\n");
        return -1;
    }
    
    FILE* fp_in = fopen (ifname, "rb"); 
    if(NULL==fp_in)
    {
        fprintf(stderr, "Invalid fp_in.\n"); 
		return 0;
    }
	else
	{
		fread(img_in, sizeof (unsigned char), iwidth*iheight, fp_in);
	}
	fclose(fp_in);

    //Init
    CLLibInit(iwidth, iheight);

   //Render
   	CLLibUpdate(iwidth, iheight, img_in);
    
	// Test
	CLLibCopyBufTest(iwidth, iheight, img_in);
  
    // Read
	CLLibReadBuf(iwidth, iheight, img_in);

    if(NULL!=ofname)
    {
        FILE* fp = fopen(ofname, "wb");
        if(NULL==fp)
        {
            printf("Error in writing out: %s\n", ofname);
        }
        else
        {
            fwrite(img_in, sizeof(unsigned char), iwidth*iheight, fp);
        }
        fclose(fp);
    }

    free(img_in);

    //Exit
    CLLibExit();
	
	return 0;
}

int cl_copy_main(
   const unsigned int iw, 
   const unsigned int ih,
   unsigned char* img_in)
{
	cl_test_copy();  
	return 0;
}


