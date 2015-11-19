#include "Texture.h"



Texture::Texture()
{
	mp_texture = nullptr;
	m_width = 0;
	m_height = 0;
}


Texture::~Texture()
{
	if (mp_texture != nullptr)
	{
		mp_texture->Release();
		mp_texture = nullptr;
	}

	m_width = 0;
	m_height = 0;
}

bool Texture::Initialize(ID3D11Device * device, LPCSTR fileName)
{
	HRESULT result;

	// Cargo la textura
	result = D3DX11CreateShaderResourceViewFromFile(
		device,
		fileName,
		NULL,
		NULL,
		&mp_texture ,
		NULL
		);

	if (FAILED(result))
	{
		// TODO : Informar del error
		return false;
	}

	// obtengo datos de la textura
	ID3D11Resource * resource = nullptr;
	mp_texture->GetResource(&resource);

	ID3D11Texture2D * texture2D = nullptr;
	result = resource->QueryInterface(&texture2D);	

	if (SUCCEEDED(result))
	{ 
		D3D11_TEXTURE2D_DESC desc;
		texture2D->GetDesc(&desc);

		m_width = desc.Width;
		m_height = desc.Height;
	}
	
	texture2D->Release();
	texture2D = nullptr;

	resource->Release();
	resource = nullptr;

	return true;
}

ID3D11ShaderResourceView * Texture::GetTexture()
{
	return mp_texture;
}

int Texture::GetWidth()
{
	return m_width;
}

int Texture::GetHeight()
{
	return m_height;
}
