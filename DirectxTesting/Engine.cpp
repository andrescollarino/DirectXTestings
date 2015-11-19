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



	//TODO : TEMP BORAR
	if (vertexBuffer != nullptr)
	{
		delete vertexBuffer;
		vertexBuffer = nullptr;
	}

	if (texture != nullptr)
	{
		delete texture;
		texture = nullptr;
	}


	if (textureShader != nullptr)
	{
		delete textureShader;
		textureShader = nullptr;
	}
}

bool Engine::InitializeGraphics(HWND hwnd)
{
	mp_graphics = new Graphics();

	return mp_graphics->InitialzieDX(hwnd);
}

bool Engine::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	mp_graphics->Initialize();

	// TODO: BORRAR SOLO TEMP
	textureShader = new TextureShader(mp_graphics->GetDevice(), hwnd, "Shader/texture", "TextureVertexShader", "TexturePixelShader");

	texture = new Texture();
	texture->Initialize(mp_graphics->GetDevice(), "mario.png");

	vertexBuffer = new VertexBuffer();
	vertexBuffer->Initialize(mp_graphics->GetDevice(), textureShader, 250.0f, false);
	return true;
}

void Engine::Run()
{
	Update();
	Render();
}

void Engine::Release()
{
	if (mp_instance != nullptr)
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
	mp_graphics->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Renderizado
	D3DXMATRIX viewMatrix;
	D3DXMATRIX projectionMatrix;
	D3DXMATRIX worldMatrix;

	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, -100);
	D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);
	D3DXVECTOR3 lookAt = D3DXVECTOR3(0, 0, 1);

	D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);
	D3DXMatrixOrthoLH(&projectionMatrix, SCREEN_WIDHT, SCREEN_HEIGHT, 0.1f, 10000.0f);
	D3DXMatrixIdentity(&worldMatrix);

	mp_graphics->EnableAlphaBlending(true);

	textureShader->SetShaderParameters(mp_graphics->GetDeviceContext(), texture->GetTexture());
	textureShader->SetShaderParameters(mp_graphics->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);

	vertexBuffer->Render(mp_graphics->GetDeviceContext());

	mp_graphics->EndScene();
}

Engine::Engine()
{
	mp_instance = this;
	mp_graphics = nullptr;

	//TODO : TEMP BORAR
	vertexBuffer = nullptr;
	texture = nullptr;
	textureShader = nullptr;
}
