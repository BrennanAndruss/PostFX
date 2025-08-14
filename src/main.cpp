#include <iostream>

#include "./Application/Application.h"
#include "./Application/WindowManager.h"

int main(int argc, char* argv[])
{
	Application application;
	application.init(640, 480);

	// Loop until the user closes the window
	while (!application.shouldClose())
	{
		application.run();
	}

	return 0;
}