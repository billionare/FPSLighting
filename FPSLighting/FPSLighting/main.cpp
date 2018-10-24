#include "main.h" 
#include "Tools.h"


//this is a fuckin main function
int main() 
{
	//const char *ErrorTitle   = "OwO you have the big error!";
	//const char *ErrorMessage = "This is not very cash money";
	//int	ErrorType = 0;




	// Playing around with stack vs heap allocation
	Tools *tools = new Tools; // Creating object (heap) and calling constructor

	tools->StartingAnimation(); 

	tools->ErrorBox("uwu", "uwu", 69);
	

	delete tools; // Deletes tools obj once main scope ends
	std::cin.get();
}

