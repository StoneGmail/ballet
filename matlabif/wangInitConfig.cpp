/*=================================================================
 * revord.c
 * example for illustrating how to copy the string data from MATLAB
 * to a C-style string and back again
 *
 * takes a row vector string and returns a string in reverse order.
 *
 * This is a MEX-file for MATLAB.
 * Copyright 1984-2011 The MathWorks, Inc.
 *=============================================================*/
#include "mex.h"
#include <iostream>
#include <string>

#include "ctpif_matlab.h"



//mex -I/data/znl/wangchoi/projects/md/include -L/usr/local/lib matlabif.cpp -lctpif -lutility

// /usr/lib:/usr/local/lib:/usr/local/gcc62/lib:/usr/local/gcc62/lib64:/usr/local/gcc62/libexec:/data/znl/wangchoi/MATLAB/R2016a/sys/opengl/lib/glnxa64:/data/znl/wangchoi/MATLAB/R2016a/sys/os/glnxa64:/data/znl/wangchoi/MATLAB/R2016a/bin/glnxa64:/data/znl/wangchoi/MATLAB/R2016a/extern/lib/glnxa64:/data/znl/wangchoi/MATLAB/R2016a/runtime/glnxa64:/data/znl/wangchoi/MATLAB/R2016a/sys/java/jre/glnxa64/jre/lib/amd64/native_threads:/data/znl/wangchoi/MATLAB/R2016a/sys/java/jre/glnxa64/jre/lib/amd64/server:.

void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[])
{
    char *input_buf;
    size_t buflen;

    /* check for proper number of arguments */
    if(nrhs!=1)
      mexErrMsgIdAndTxt( "MATLAB:revord:invalidNumInputs",
              "One input required.");
    else if(nlhs > 1)
      mexErrMsgIdAndTxt( "MATLAB:revord:maxlhs",
              "Too many output arguments.");

    /* input must be a string */
    if ( mxIsChar(prhs[0]) != 1)
      mexErrMsgIdAndTxt( "MATLAB:revord:inputNotString",
              "Input must be a string.");

    /* input must be a row vector */
    if (mxGetM(prhs[0])!=1)
      mexErrMsgIdAndTxt( "MATLAB:revord:inputNotVector",
              "Input must be a row vector.");

    /* get the length of the input string */
    buflen = (mxGetM(prhs[0]) * mxGetN(prhs[0])) + 1;

    /* copy the string data from prhs[0] into a C string input_ buf.    */
    input_buf = mxArrayToString(prhs[0]);

    if(input_buf == NULL)
      mexErrMsgIdAndTxt( "MATLAB:revord:conversionFailed",
              "Could not convert input to string.");

    /* call the C subroutine */
    uint64_t* dynamicData = (uint64_t*)mxCalloc(1, sizeof(uint64_t));
    dynamicData[0] = (uint64_t)mexInitConfig(input_buf);

    /* set C-style string output_buf to MATLAB mexFunction output*/

    /* allocate memory for return config */
    // output_buf=(char*)mxCalloc(buflen, sizeof(char));

    plhs[0] = mxCreateNumericMatrix(1,1,mxINT64_CLASS,mxREAL);

    mxSetData(plhs[0], dynamicData);

    mxFree(input_buf);
    return;
}

