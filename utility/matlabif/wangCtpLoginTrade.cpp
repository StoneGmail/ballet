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

    uint64_t tdif = 0;
    if (mxGetNumberOfElements(prhs[0]) == 1) {
        tdif = mxGetScalar(prhs[0]);

        std::cout << "Release interface " << tdif << std::endl;
    } else {
        mexErrMsgTxt("Integer scalar is not of size == [1 1].\n");
    }

    INTERFACE tdif = mexGetCtpTdif((CONFIGPTR)config, logpath);
    if (tdif == nullptr) {
        std::cout << "Get Ctp Trade Interface Error\n";
    }

    uint64_t* dynamicData = (uint64_t*)mxCalloc(1, sizeof(uint64_t));
    dynamicData[0] = (uint64_t)tdif;

    /* set C-style string output_buf to MATLAB mexFunction output*/

    /* allocate memory for return config */
    // output_buf=(char*)mxCalloc(buflen, sizeof(char));

    plhs[0] = mxCreateNumericMatrix(1,1,mxINT64_CLASS,mxREAL);

    mxSetData(plhs[0], dynamicData);


    return;
}
