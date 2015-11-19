#include "Game.h"

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR  lpCmdLine, int  nCmdShow)
{
	Game * game = new Game();

	if (game->Initialize())
	{
		game->Run();
	}


	delete game;

	return 0 ;
}