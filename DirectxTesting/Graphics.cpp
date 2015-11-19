#include "Graphics.h"

Graphics::Graphics()
{
	mp_D3DManager = nullptr;
	m_hwnd = nullptr;
}

Graphics::~Graphics()
{
	if (mp_D3DManager != nullptr)
	{
		delete mp_D3DManager;
		mp_D3DManager = nullptr;
	}
}

bool Graphics::InitialzieDX(HWND hwnd)
{

	if (hwnd == nullptr)
	{
		// TODO: Informar del error,
		return false;
	}

	m_hwnd = hwnd;

	mp_D3DManager = new D3DManager();
	if (!mp_D3DManager->Initialize((int)SCREEN_WIDHT, (int)SCREEN_HEIGHT, VSYNC_ENABLED, hwnd, FULL_SCREEN))
	{
		// TODO: Informar del Error
		return false;
	}

	return true;
}

void Graphics::Initialize()
{

}

void Graphics::BeginScene(float r, float g, float b, float a)
{
	mp_D3DManager->BeginScene(r, g, b, a);
}

void Graphics::EndScene()
{
	mp_D3DManager->EndScene();
}

void Graphics::EnableAlphaBlending(bool enable)
{
	mp_D3DManager->EnableAlphaBlending(enable);
}

void Graphics::EnableZBuffer(bool enable)
{
	mp_D3DManager->EnableZBuffer(enable);
}

D3DManager * Graphics::GetD3DManager()
{
	return mp_D3DManager;
}

HWND Graphics::GetHwnd()
{
	return m_hwnd;
}

ID3D11Device * Graphics::GetDevice()
{
	if (mp_D3DManager == nullptr)
		return nullptr;

	return mp_D3DManager->GetDevice();
}

ID3D11DeviceContext * Graphics::GetDeviceContext()
{
	if (mp_D3DManager == nullptr)
		return nullptr;

	return mp_D3DManager->GetDeviceContext();
}

void Graphics::SetHwnd(HWND hwnd)
{
	m_hwnd = hwnd;
}
