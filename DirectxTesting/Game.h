#pragma once

#include <Windows.h>
#include <string>
#include <iostream>

#include "Engine.h"


using namespace std;

class Game
{

public:

	Game();
	~Game();

	bool Initialize();
	void Run();

private:
	
	bool CreateDXWindow(string windowTitle, int x, int y, int width, int height);

private:


	HINSTANCE m_hInstance;

};

