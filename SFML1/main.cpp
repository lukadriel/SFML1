#include "Application.h"
#include <iostream>
#include <stdexcept>



int main()
{
	try
	{
		Application game;
		game.run();
	}
	catch (const std::exception& e)
	{
		std::cout << "\nEXCEPTION: " << e.what() << std::endl;
	}
	return 0;
}

