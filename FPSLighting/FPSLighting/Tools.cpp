#include "Tools.h"


Tools::Tools()
{

}

// Animation that plays at the run of the program (Shorter version of another shitty animation I made)

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


Tools::~Tools()
{

}
