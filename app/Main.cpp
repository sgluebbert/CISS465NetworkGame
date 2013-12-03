#include "../Source/Application/Application.h"

int main(int argc, char* argv[]) {

	bool gui = true;

	if (argc >= 2)
	{
		if (strcmp(argv[1], "help") == 0 || strcmp(argv[1], "h") == 0)
		{
			std::cout << "Usage:\n";
			std::cout << "\t" << argv[0] << "\n";
			std::cout << "\t" << argv[0] << " server [port] [lobby name] [map scale]\n";
			return 0;
		}

		if (strcmp(argv[1], "server") == 0)
			gui = false;

		if (argc >= 3)
			lobbySetPort = atoi(argv[2]);
		if (argc >= 4)
			lobbySetName = argv[3];
		if (argc >= 5)
			lobbySetMapScale = atof(argv[4]);
	}

    Application GameApp;

    return GameApp.Execute(gui);
}
