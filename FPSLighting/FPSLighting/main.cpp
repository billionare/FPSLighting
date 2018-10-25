#include "main.h" 
#include "Tools.h"


//this is a fuckin main function
int main() 
{
	// Playing around with stack vs heap allocation
	Tools *tools = new Tools; // Creating object (heap) and calling constructor

	tools->StartingAnimation(); 

	tools->Message("OwO you have the big error!", "This is not very cash money", 6);

	delete tools; // Deletes tools obj once main scope ends

	std::cin.get();
}

