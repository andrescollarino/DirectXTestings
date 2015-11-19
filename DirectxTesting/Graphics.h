#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "D3DManager.h"
#include "System.h"

class Graphics
{
public:
	Graphics();
	~Graphics();

	bool InitialzieDX(HWND hwnd);
	void Initialize();
	void BeginScene(float r, float g, float b, float a);
	void EndScene();

	void EnableAlphaBlending(bool enable);
	void EnableZBuffer(bool enable);

	D3DManager * GetD3DManager();
	HWND GetHwnd();
	ID3D11Device * GetDevice();
	ID3D11DeviceContext * GetDeviceContext();

	void SetHwnd(HWND hwnd);


private:
	D3DManager * mp_D3DManager;
	HWND m_hwnd;
};

#endif