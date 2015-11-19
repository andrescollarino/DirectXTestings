#ifndef _D3D_MANAGER_H
#define _D3D_MANAGER_H

#include <dxgi.h>
#include <d3d11.h>
#include <D3Dcommon.h>
#include <Windows.h>

#include <string>

using namespace std;

class D3DManager
{
public:
	D3DManager();
	~D3DManager();

	bool Initialize(int screenWidth, int screenHeight , bool vsync , HWND hwnd , bool fullScreen);
	void BeginScene(float r , float g , float b , float a);
	void EndScene();

	void EnableAlphaBlending(bool enable);
	void EnableZBuffer(bool enable);

	ID3D11Device * GetDevice();
	ID3D11DeviceContext * GetDeviceContext();

private:

	bool InitializeSwapChain(HWND hwnd , bool fullscreen , int screenWidth , int screenHeight , unsigned int numerator , unsigned int  denominator);
	bool InitializeDepthBuffer(int screenWidth, int screenHeight);
	bool InitializeDepthStencilBuffer();
	bool InitializeStencilView();
	bool InitializeRasterizerStete();
	void InitializeViewport(int screenWidth, int screenHeight);
	bool InitializeAlphaBlending();
	bool InitializeZBuffer();


private:

	bool m_vsync_enable;
	
	int m_videoCardMemory;
	string m_videoCardDescription;

	IDXGISwapChain * mp_swapChain;
	ID3D11Device * mp_device;
	ID3D11DeviceContext * mp_deviceContext;
	ID3D11RenderTargetView * mp_renderTargetView;
	ID3D11Texture2D *	mp_depthStencilBuffer;
	ID3D11DepthStencilState * mp_depthStencilState;
	ID3D11DepthStencilView * mp_depthStencilView;
	ID3D11BlendState * mp_alphaEnableBlendingState;
	ID3D11BlendState * mp_alphaDisableBlendingState;
	ID3D11DepthStencilState * mp_depthDisableStencilState;
	ID3D11RasterizerState * mp_rasterState;
};

#endif

