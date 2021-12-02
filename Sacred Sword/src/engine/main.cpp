#include "pch.h"
#include "Window.h"

int main()
{
	ss::Window gameWindow;

	try
	{
		gameWindow.Run();
	}
	catch (std::exception e)
	{
		std::cout << "MAIN::ERROR::" << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "MAIN::Successfully exited game loop" << std::endl;

	_CrtDumpMemoryLeaks();

	return EXIT_SUCCESS;
}