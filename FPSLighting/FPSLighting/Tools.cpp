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

void Tools::ErrorBox(const char *ErrorTitle, const char *ErrorMessage, int ErrorType)
{
	MessageBoxA(NULL, ErrorMessage, ErrorTitle, ErrorType);
}


Tools::~Tools()
{
	
}
