#include "../Source/Application/Application.h"

int main(int argc, char* argv[]) {

	bool gui = true;
	if (argc == 2) {
		if (strcmp(argv[1], "server") == 0)
			gui = false;
	}

    Application GameApp;

    return GameApp.Execute(gui);
}
