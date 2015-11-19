#include "Engine.h"

Engine * Engine::mp_instance = nullptr;

Engine * Engine::GetEngine()
{
	if (mp_instance == nullptr) 
	{
		mp_instance = new Engine();
	}

	return mp_instance;
}

Engine::~Engine()
{
	if (mp_graphics != nullptr)
	{
		delete mp_graphics;
		mp_graphics = nullptr;
	}

	mp_instance = nullptr;
}

bool Engine::InitializeGraphics(HWND hwnd)
{
	mp_graphics = new Graphics();

	return mp_graphics->InitialzieDX(hwnd);
}

bool Engine::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	mp_graphics->Initialize();

	return true;
}

void Engine::Run()
{
	Update();
	Render();
}

void Engine::Release()
{
	if(mp_instance != nullptr)
	{
		delete mp_instance;
		mp_instance = nullptr;
	}
}

Graphics * Engine::GetGraphics()
{
	return mp_graphics;
}

void Engine::Update()
{
}

void Engine::Render()
{
	mp_graphics->BeginScene(0.0f,0.0f,0.0f,1.0f);

	// Renderizado

	mp_graphics->EndScene();
}

Engine::Engine()
{
	mp_instance = this;
	mp_graphics = nullptr;
}
