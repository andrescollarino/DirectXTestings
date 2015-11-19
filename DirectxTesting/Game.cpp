#include "Game.h"
//#include "System.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

Game::Game()
{

}


Game::~Game()
{
	if (FULL_SCREEN)
	{
		// Quita el full Screen
		ChangeDisplaySettings(NULL, 0);
	}

	Engine::GetEngine()->Release();

	UnregisterClass(WINDOW_CLASS_NAME, m_hInstance);
	m_hInstance = NULL;
}


bool Game::Initialize()
{
	if (!CreateDXWindow("Game Test", WINDOW_POS_X, WINDOW_POS_Y, SCREEN_WIDHT, SCREEN_HEIGHT))
	{
		// TODO: Informar del problema
		return false;
	}

	if (!Engine::GetEngine()->Initialize(m_hInstance, Engine::GetEngine()->GetGraphics()->GetHwnd()))
	{
		// TODO: Informar del problema
		return false;
	}
	return true;
}

void Game::Run()
{
	MSG msg;

	ZeroMemory(&msg, sizeof(MSG));

	// Bucle principal
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// Control de eventos de windows
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Updates y Render
			Engine::GetEngine()->Run();
		}
	}
}


bool Game::CreateDXWindow(string windowTitle, int x, int y, int width, int height)
{
	HWND hwnd; // Ventana
	WNDCLASSEX wc;

	m_hInstance = GetModuleHandle(NULL);
	
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= m_hInstance;
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm			= wc.hIcon;
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= WINDOW_CLASS_NAME;
	wc.cbSize			= sizeof(WNDCLASSEX);

	// Registro la clase 
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "RegisterClassEx Failed", "Error", MB_OK);
		return false; 
	}


	// Full Screen
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN) 
	{
		DEVMODE dmScreenSettings;
		
		ZeroMemory(&dmScreenSettings, sizeof(DEVMODE));

		dmScreenSettings.dmSize			= sizeof(DEVMODE);
		dmScreenSettings.dmPelsWidth	= (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight	= (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel	= 32;
		dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	}
	else
	{
		screenHeight = height;
		screenWidth = width;
	}

	// Creo la ventana
	int nStyle = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX;
	
	hwnd = CreateWindowEx(
		WS_EX_APPWINDOW, 
		WINDOW_CLASS_NAME, 
		windowTitle.c_str(), 
		nStyle, 
		x, 
		y, 
		screenWidth, 
		screenHeight, 
		NULL, 
		NULL, 
		m_hInstance, 
		NULL
		);


	if (hwnd == NULL)
	{
		MessageBox(NULL, "CreateWindowEx Failed", "Error", MB_OK);
		Engine::GetEngine()->Release();
		PostQuitMessage(0);

		return false;
	}

	if (!Engine::GetEngine()->InitializeGraphics(hwnd))
	{
		MessageBox(hwnd, "Could not initialize DirectX 11", "Error", MB_OK);
		Engine::GetEngine()->Release();
		PostQuitMessage(0);
		UnregisterClass(WINDOW_CLASS_NAME, m_hInstance);
		m_hInstance = NULL;
		DestroyWindow(hwnd);

		return false;
	}

	Engine::GetEngine()->GetGraphics()->SetHwnd(hwnd);

	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	return true;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
			DestroyWindow(hwnd);
		}
		break;
	case WM_PAINT:

		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);

		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		DestroyWindow(hwnd);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
		break;
	}

	return 0;
}