#pragma once

#include "common.h"


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



	~Tools();
};


