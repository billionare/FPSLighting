#pragma once

#include "common.h"

#include <d3dx11core.h>
#include <d3dx11.h>
#include <d3d11.h>
#include <d3d.h>
#include <d3d9.h>  
#include <Wincodec.h>             // we use WIC for saving images
#pragma comment(lib, "d3d9.lib")  // link to DirectX 9 library

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)
#define HRCHECK(__expr) {hr=(__expr);if(FAILED(hr)){wprintf(L"FAILURE 0x%08X (%i)\n\tline: %u file: '%s'\n\texpr: '" WIDEN(#__expr) L"'\n",hr, hr, __LINE__,__WFILE__);goto cleanup;}}
#define RELEASE(__p) {if(__p!=nullptr){__p->Release();__p=nullptr;}}


class Tools
{
public:

	Tools();

	// Animation that plays at the run of the program (Shorter version of another shitty animation I made)
	void StartingAnimation(); 

	// Just incase someone wants to use this to output an error. Probably useless. 
	void Message(const char *ErrorTitle, const char *ErrorMessage, int ErrorType); 

	// Creates a process (quicker CreateProcess)
	void CreateProc(const char *CreateProcIn);

	// Takes a screenshot with DirectX
	HRESULT Direct3D9TakeScreenshots(UINT adapter, UINT count);

	~Tools();
};


