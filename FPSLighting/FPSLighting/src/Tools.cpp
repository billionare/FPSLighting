#include "Tools.h"


Tools::Tools()
{

}

void Tools::StartingAnimation()
{
	std::cout << "Hey, welcome to FPSLighting, the program that "
		<< "will change your lights when you shoot!" << std::endl;

	Sleep(350);
	std::cout << ".";
	Sleep(350);
	std::cout << ".";
	Sleep(300);
	std::cout << ".";
	Sleep(500);

	system("cls");

}

void Tools::Message(const char *ErrorTitle, const char *ErrorMessage, int ErrorType)
{
	MessageBoxA(NULL, ErrorMessage, ErrorTitle, ErrorType);
}

void Tools::CreateProc(const char *CreateProcIn)
{
	// Not using the below in func params because it would be so hecking long.

	HANDLE hProcess; // Handling hProcess
	HANDLE hThread;  // Handling hThread

	PROCESS_INFORMATION procinfo;

	STARTUPINFO startup;

	DWORD dwProcessId;
	DWORD dwThreadID;

	ZeroMemory(&procinfo, sizeof(procinfo)); // Setting the memory location of procinfo to NULL
	ZeroMemory(&startup , sizeof(startup));  // Setting the memory location of startup  to NULL

	BOOL bCreateProcess = false; 

	// Chances are, we will not need to modify the below (all of the NULLs), but if we do, we can just overload :)
	bCreateProcess = CreateProcess(CreateProcIn, nullptr, nullptr, nullptr, NULL, NULL, nullptr, nullptr, &startup, &procinfo);

		
	if (bCreateProcess)
	{
		std::cout << "Create Process suceeded" << std::endl;

		std::cout << "Proc ID      : " << procinfo.dwThreadId  << std::endl;
		std::cout << "Thread Id    : " << procinfo.dwProcessId << std::endl;
		 
		std::cout << "GetProcessID : " << GetProcessId(procinfo.hProcess) << std::endl;
		std::cout << "GetThreadID  : " << GetProcessId(procinfo.hThread ) << std::endl;

	}

	else
	{
		std::cout << "Create Proc Failed " << GetLastError() << std::endl;
	}
	
	//WaitForSingleObject(procinfo.hProcess, INFINITE);

	CloseHandle(procinfo.hProcess);
	CloseHandle(procinfo.hThread );

}


HRESULT Tools::Direct3D9TakeScreenshots(UINT adapter, UINT count)
{
	HRESULT hr = S_OK;
	IDirect3D9 *d3d = nullptr;
	IDirect3DDevice9 *device = nullptr;
	IDirect3DSurface9 *surface = nullptr;
	D3DPRESENT_PARAMETERS parameters = { 0 };
	D3DDISPLAYMODE mode;
	D3DLOCKED_RECT rc;
	UINT pitch;
	SYSTEMTIME st;
	LPBYTE *shots = nullptr;

	// init D3D and get screen size
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	HRCHECK(d3d->GetAdapterDisplayMode(adapter, &mode));

	parameters.Windowed = TRUE;
	parameters.BackBufferCount = 1;
	parameters.BackBufferHeight = mode.Height;
	parameters.BackBufferWidth = mode.Width;
	parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	parameters.hDeviceWindow = NULL;

	// create device & capture surface
	HRCHECK(d3d->CreateDevice(adapter, D3DDEVTYPE_HAL, NULL, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &parameters, &device));
	HRCHECK(device->CreateOffscreenPlainSurface(mode.Width, mode.Height, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &surface, nullptr));

	// compute the required buffer size
	HRCHECK(surface->LockRect(&rc, NULL, 0));
	pitch = rc.Pitch;
	HRCHECK(surface->UnlockRect());

	// allocate screenshots buffers
	shots = new LPBYTE[count];
	for (UINT i = 0; i < count; i++)
	{
		shots[i] = new BYTE[pitch * mode.Height];
	}

	GetSystemTime(&st); // measure the time we spend doing <count> captures
	wprintf(L"%i:%i:%i.%i\n", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	for (UINT i = 0; i < count; i++)
	{
		// get the data
		HRCHECK(device->GetFrontBufferData(0, surface));

		// copy it into our buffers
		HRCHECK(surface->LockRect(&rc, NULL, 0));
		CopyMemory(shots[i], rc.pBits, rc.Pitch * mode.Height);
		HRCHECK(surface->UnlockRect());
	}
	GetSystemTime(&st);
	wprintf(L"%i:%i:%i.%i\n", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	// save all screenshots
	for (UINT i = 0; i < count; i++)
	{
		//WCHAR file[100];
		LPSTR file;
		wsprintf(file, L"cap%i.png", i);
		HRCHECK(SavePixelsToFile32bppPBGRA(mode.Width, mode.Height, pitch, shots[i], file, GUID_ContainerFormatPng));
	}

cleanup:
	if (shots != nullptr)
	{
		for (UINT i = 0; i < count; i++)
		{
			delete shots[i];
		}
		delete[] shots;
	}
	RELEASE(surface);
	RELEASE(device);
	RELEASE(d3d);
	return hr;
}

HRESULT SavePixelsToFile32bppPBGRA(UINT width, UINT height, UINT stride, LPBYTE pixels, LPWSTR filePath, const GUID &format)
{
	if (!filePath || !pixels)
		return E_INVALIDARG;

	HRESULT hr = S_OK;
	IWICImagingFactory *factory = nullptr;
	IWICBitmapEncoder *encoder = nullptr;
	IWICBitmapFrameEncode *frame = nullptr;
	IWICStream *stream = nullptr;
	GUID pf = GUID_WICPixelFormat32bppPBGRA;
	BOOL coInit = CoInitialize(nullptr);

	HRCHECK(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory)));
	HRCHECK(factory->CreateStream(&stream));
	HRCHECK(stream->InitializeFromFilename(filePath, GENERIC_WRITE));
	HRCHECK(factory->CreateEncoder(format, nullptr, &encoder));
	HRCHECK(encoder->Initialize(stream, WICBitmapEncoderNoCache));
	HRCHECK(encoder->CreateNewFrame(&frame, nullptr)); // we don't use options here
	HRCHECK(frame->Initialize(nullptr)); // we dont' use any options here
	HRCHECK(frame->SetSize(width, height));
	HRCHECK(frame->SetPixelFormat(&pf));
	HRCHECK(frame->WritePixels(height, stride, stride * height, pixels));
	HRCHECK(frame->Commit());
	HRCHECK(encoder->Commit());

cleanup:
	RELEASE(stream);
	RELEASE(frame);
	RELEASE(encoder);
	RELEASE(factory);
	if (coInit) CoUninitialize();
	return hr;
}


Tools::~Tools()
{
	
}
