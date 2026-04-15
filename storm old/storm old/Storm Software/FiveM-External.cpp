#include "FiveM-External.hpp"
#include "Definations/Cheat.hpp"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Cheat::Initialize();

	while (!g_Options.General.ShutDown)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	Cheat::ShutDown();

	return TRUE;
}