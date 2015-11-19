#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <D3D11.h>
#include <D3DX11tex.h>

class Texture
{
public:
	Texture();
	~Texture();

	bool Initialize(ID3D11Device * device, LPCSTR fileName);

	ID3D11ShaderResourceView * GetTexture();

	int GetWidth();
	int GetHeight();

private:

	ID3D11ShaderResourceView * mp_texture;

	int m_width;
	int m_height;

};

#endif
