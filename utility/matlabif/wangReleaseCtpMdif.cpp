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
    /* check for proper number of arguments */
    if(nrhs!=1)
      mexErrMsgIdAndTxt( "MATLAB:revord:invalidNumInputs",
              "One input required.");
    else if(nlhs > 1)
      mexErrMsgIdAndTxt( "MATLAB:revord:maxlhs",
              "Too many output arguments.");

    /* input must be not a string */
    if ( mxIsInt64(prhs[0]) != 1)
      mexErrMsgIdAndTxt( "MATLAB:revord:inputNotString",
              "Input must not be 64bit Pointer/Integer.");

    uint64_t mdif = 0;
    if (mxGetNumberOfElements(prhs[0]) == 1) {
        mdif = mxGetScalar(prhs[0]);

        std::cout << "Release interface " << mdif << std::endl;

        mexReleaseCtpMdif((INTERFACE)mdif);
    } else {
        mexErrMsgTxt("Integer scalar is not of size == [1 1].\n");
    }

    //mexPrintf("%lu\n",scalar);



    return;
}

