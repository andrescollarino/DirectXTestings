#include "D3DManager.h"



D3DManager::D3DManager()
{
	mp_swapChain = nullptr;
	mp_device = nullptr;
	mp_deviceContext = nullptr;
	mp_renderTargetView = nullptr;
	mp_depthStencilBuffer = nullptr;
	mp_depthStencilState = nullptr;
	mp_depthStencilView = nullptr;
	mp_alphaEnableBlendingState = nullptr;
	mp_alphaDisableBlendingState = nullptr;
	mp_depthDisableStencilState = nullptr;
	mp_depthDisableStencilState = nullptr;
}


D3DManager::~D3DManager()
{

	if (mp_swapChain != nullptr)
	{
		mp_swapChain->SetFullscreenState(false, NULL);
	}

	if (mp_alphaEnableBlendingState != nullptr)
	{
		mp_alphaEnableBlendingState->Release();
		mp_alphaEnableBlendingState = nullptr;
	}

	if (mp_alphaDisableBlendingState != nullptr)
	{
		mp_alphaDisableBlendingState->Release();
		mp_alphaDisableBlendingState = nullptr;
	}

	if (mp_depthDisableStencilState != nullptr)
	{
		mp_depthDisableStencilState->Release();
		mp_depthDisableStencilState = nullptr;
	}

	if (mp_rasterState != nullptr) 
	{
		mp_rasterState->Release();
		mp_rasterState = nullptr;
	}

	if (mp_depthStencilView != nullptr)
	{
		mp_depthStencilView->Release();
		mp_depthStencilView = nullptr;
	}

	if (mp_depthStencilState != nullptr) 
	{
		mp_depthStencilState->Release();
		mp_depthStencilState = nullptr;
	}
}


bool D3DManager::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullScreen)
{
	HRESULT result;
	IDXGIFactory * factory = nullptr;
	IDXGIAdapter * adapter = nullptr;
	IDXGIOutput * adapterOutput = nullptr;
	unsigned int numModes;
	unsigned int numerator = 0;
	unsigned int denominator = 0;
	unsigned int stringLenht;
	DXGI_MODE_DESC * displayModeList = nullptr;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	ID3D11Texture2D * backBuffer = nullptr;

	// Guardo el vsycn
	m_vsync_enable = vsync;

	// Creo una interfas con DirectX (IDXGIFactory)
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		// TODO: Informar del Error
		return false;
	}

	// Uso el IDXGIFactory para crear un adapatador(Placa de Video)
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		// TODO: Informar del Error
		return false;
	}

	// Enumero la cantidad de monitores (Adaptador de salida)
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		// TODO: Informar del Error
		return false;
	}
	
	// Obtengo el numero de modos que se ajustan al formato DXGI_FORMAT_R8G8B8A8 para el monitor
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		// TODO: Informar del Error
		return false;
	}

	// Creo una lista con todos los posbles modos para esta combinacion placa de video monitor.
	displayModeList = new DXGI_MODE_DESC[numModes];

	// Cargo la lista de display Mode
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		// TODO: Informar del Error
		return false;
	}


	for (int i = 0; i < (int)numModes; ++i) 
	{

		if (displayModeList[i].Width == (unsigned int)screenWidth && displayModeList[i].Height == (unsigned int)screenHeight)
		{
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
			//TODO: Loquidar el for al encontrar el valor correcto
		}

	}


	if (numerator == 0 && denominator == 0)
	{
		// Informar Error
		return false;
	}

	// Obtengo la descripcion del adaptador
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		// TODO: Informar del Error
		return false;
	}

	// Guardo la cantidad de memorya de video
	m_videoCardMemory = adapterDesc.DedicatedVideoMemory;

	// Convierto el nombre de la VGA a string
	char tmp_string[128];

	error = wcstombs_s(&stringLenht, tmp_string, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		// TODO: Informar del Error
		return false;
	}

	m_videoCardDescription = tmp_string;

	// Released Memory
	delete [] displayModeList;
	displayModeList = nullptr;

	adapterOutput->Release();
	adapterOutput = nullptr;

	adapter->Release();
	adapter = nullptr;

	factory->Release();
	factory = nullptr;


	if (!InitializeSwapChain(hwnd, fullScreen, screenWidth, screenHeight, numerator, denominator))
	{
		// Todo: Informar El Error
		return false;
	}

	// Obtengo el puntero al BackBuffer
	result = mp_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(result))
	{
		// TODO: Informar del Error
		return false;
	}

	// Creo el render Target View
	result = mp_device->CreateRenderTargetView(backBuffer, NULL, &mp_renderTargetView);
	if (FAILED(result))
	{
		// TODO: Informar del Error
		return false;
	}

	// No te Nesesito MUERE
	backBuffer->Release();
	backBuffer = nullptr;

	if (!InitializeDepthBuffer(screenWidth, screenHeight)) 
	{
		// TODO: Informar del Error
		return false;
	}

	if (!InitializeDepthStencilBuffer())
	{
		// TODO: Informar del Error
		return false;
	}

	if (!InitializeStencilView()) 
	{
		// TODO: Informar del Error
		return false;
	}

	mp_deviceContext->OMSetRenderTargets(1, &mp_renderTargetView, mp_depthStencilView);
	if (!InitializeRasterizerStete())
	{
		// TODO: Informar del Error
		return false;
	}

	InitializeViewport(screenWidth, screenHeight);

	if (!InitializeAlphaBlending())
	{
		// TODO: Informar del Error
		return false;
	}

	if (!InitializeZBuffer()) 
	{
		// TODO: Informar del Error
		return false;
	}

	return true;
}

void D3DManager::BeginScene(float r, float g, float b, float a)
{
	float color[4];

	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;

	mp_deviceContext->ClearRenderTargetView(mp_renderTargetView, color);
	mp_deviceContext->ClearDepthStencilView(mp_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3DManager::EndScene()
{
	if (m_vsync_enable) 
	{
		mp_swapChain->Present(1, 0);
	}
	else
	{
		mp_swapChain->Present(0, 0);
	}
}

void D3DManager::EnableAlphaBlending(bool enable)
{
	float blendFactor[4];

	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	if (enable)
	{
		mp_deviceContext->OMSetBlendState(mp_alphaEnableBlendingState, blendFactor, 0xFFFFFFFF);
	}
	else
	{
		mp_deviceContext->OMSetBlendState(mp_alphaDisableBlendingState, blendFactor, 0xFFFFFFFF);
	}
}

void D3DManager::EnableZBuffer(bool enable)
{
	if (enable)
	{
		mp_deviceContext->OMSetDepthStencilState(mp_depthStencilState, 1);
	}
	else
	{
		mp_deviceContext->OMSetDepthStencilState(mp_depthDisableStencilState, 1);
	}
}

ID3D11Device * D3DManager::GetDevice()
{
	return mp_device;
}

ID3D11DeviceContext * D3DManager::GetDeviceContext()
{
	return mp_deviceContext;
}

bool D3DManager::InitializeSwapChain(HWND hwnd, bool fullscreen, int screenWidth, int screenHeight, unsigned int numerator, unsigned int denominator)
{

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	HRESULT result;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1; // Un solo back buffer
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

	if (m_vsync_enable) 
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;

	// Multisampleo desactivado
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.Windowed = !fullscreen;

	// Escalado no Especificado
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapChainDesc.Flags = 0;
	
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Creo el divace, swapchain y deviceContext
	result = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		&featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&mp_swapChain,
		&mp_device,
		NULL,
		&mp_deviceContext
		);

	if (FAILED(result))
	{
		// TODO: Informar del Error
		return false;
	}
	
	return true;
}

bool D3DManager::InitializeDepthBuffer(int screenWidth, int screenHeight)
{

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	HRESULT result;

	ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = mp_device->CreateTexture2D(&depthBufferDesc, NULL, &mp_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool D3DManager::InitializeDepthStencilBuffer()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	HRESULT result;

	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = mp_device->CreateDepthStencilState(&depthStencilDesc, &mp_depthStencilState);
	if (FAILED(result))
	{
		// TODO: Informar del Error
		return false;
	}

	mp_deviceContext->OMSetDepthStencilState(mp_depthStencilState, 1);

	return true;
}

bool D3DManager::InitializeStencilView()
{
	D3D11_DEPTH_STENCIL_VIEW_DESC  depthStencilViewDesc;
	HRESULT result;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = mp_device->CreateDepthStencilView(mp_depthStencilBuffer, &depthStencilViewDesc, &mp_depthStencilView);
	if (FAILED(result))
	{
		// TODO: Informar del Error
		return false;
	}

	return true;
}

bool D3DManager::InitializeRasterizerStete()
{
	D3D11_RASTERIZER_DESC rasterDesc;
	HRESULT result;

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	result = mp_device->CreateRasterizerState(&rasterDesc, &mp_rasterState);
	if (FAILED(result))
	{
		return false;
	}
	
	mp_deviceContext->RSSetState(mp_rasterState);
	return true;
}

void D3DManager::InitializeViewport(int screenWidth, int screenHeight)
{
	D3D11_VIEWPORT viewport;

	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	mp_deviceContext->RSSetViewports(1, &viewport);
}

bool D3DManager::InitializeAlphaBlending()
{

	D3D11_BLEND_DESC blendStateDesc;
	HRESULT result;

	ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));

	// TODO: Estuadiar los modos de alpha para mallor compresion
	blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

	result = mp_device->CreateBlendState(&blendStateDesc, &mp_alphaEnableBlendingState);
	if (FAILED(result)) 
	{
		// TODO: Informar del ERROR
		return false;
	}

	blendStateDesc.RenderTarget[0].BlendEnable = FALSE;
	result = mp_device->CreateBlendState(&blendStateDesc, &mp_alphaDisableBlendingState);
	if (FAILED(result))
	{
		// TODO: Informar del ERROR
		return false;
	}

	return true;
}

bool D3DManager::InitializeZBuffer()
{

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	HRESULT result;

	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = mp_device->CreateDepthStencilState(&depthStencilDesc, &mp_depthDisableStencilState);
	if (FAILED(result))
	{
		// TODO: Informar del Error
		return false;
	}

	return true;
}
