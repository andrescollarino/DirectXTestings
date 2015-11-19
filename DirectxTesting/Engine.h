#ifndef _ENGINE_H
#define _ENGINE_H

#include "Graphics.h"
#include "TextureShader.h"
#include "Texture.h"
#include "VertexBuffer.h"

class Engine
{

public:

	static Engine * GetEngine();

public:
	
	~Engine();

	bool InitializeGraphics(HWND hwnd);
	bool Initialize(HINSTANCE hInstance, HWND hwnd);
	void Run();

	
	void Release();

	Graphics * GetGraphics();
private:

	void Update();
	void Render();

	Engine(); // TODO: Haveriguar sobre contrusctores rpviados en c++

private:

	static Engine* mp_instance;
	Graphics * mp_graphics;

	// TODO:TEMP SOLO APRA PRUEBAS
	VertexBuffer * vertexBuffer;
	TextureShader * textureShader;
	Texture * texture;
};

#endif