//winmain.cpp - entry point to application
#include "Engine.h"
using namespace std;

//declare global engine object
std::auto_ptr<Batman::Engine> g_engine(new Batman::Engine);

/**
WndProc - message handler (req'd when user closes window with 'X' button)
**/
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg == WM_DESTROY) PostQuitMessage(0);
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

/**
WinMain - entry point of program
**/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
					LPSTR lpCmdLine, int nCmdShow)
{
	//output log
	debug << "WinMain running" << endl;
	//seed random number
	srand((unsigned int)time(0));

	//check command line
	debug << "Checking Parameters" << endl;
	if(strlen(lpCmdLine) > 0)
	{
		g_engine->setCommandLineParams(lpCmdLine);
		debug << "Params: " << g_engine->getCommandLineParams() << std::endl;
	}

	//let main program set screen dimensions
	debug << "Calling game_preload" << endl;
	if(!game_preload())
	{
		debug << "Error in game_preload" << endl;
		return 0;
	}

	//initialise the engine
	debug << "initialising engine" << endl;
	bool res = g_engine->Init(
		hInstance,
		g_engine->getScreenWidth(),
		g_engine->getScreenHeight(),
		g_engine->getColorDepth(),
		g_engine->getFullScreen()
		);

	if(!res)
	{
		debug << "error initialising the engine" << endl;
		return 0;
	}

	//set windows message var
	MSG msg;
	memset(&msg, 0, sizeof(MSG));

	//set timer
	Batman::Timer timer;
	double startTime = timer.getElapsed();

	debug << "Core timer started: " << timer.getElapsed() << endl;
	debug << "entering while loop" << endl;

	//main message loop
	while(msg.message!=WM_QUIT)
	{
		//peek at message queue and distribute if necessary
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//get time elapsed
			double t = timer.getElapsed();
			//calculate time difference
			float deltaTime = (t - startTime) * 0.001f;
			//call engine update
			g_engine->Update(deltaTime);
			//reset time
			startTime = t;
		}
	}

	//output log and shutdown operations
	debug << "Exiting while loop" << endl;
	debug << "Total run time: " << timer.getElapsed() << endl;
	debug << "Freeing game resources" << endl;
	game_end();

	//shutdown engine
	debug << "shutting down engine" << endl;
	g_engine->Shutdown();
	ShowCursor(true);
	return 1;
}