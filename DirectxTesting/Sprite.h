#ifndef _SPRITE_H
#define _SPRITE_H

#include "VertexBuffer.h"
#include "Texture.h"

class Sprite
{
public:
	Sprite(float size);
	virtual ~Sprite();

	virtual void Initialize(ID3D11Device * device , Shader * shader , LPCTSTR textureFileName);
	virtual void Update(){}
	virtual void Render(ID3D11DeviceContext * deviceConstext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);

protected:

	VertexBuffer * mp_vertexBuffer;
	Texture * mp_texture;
	Shader * mp_shader;
	int m_size;
};

#endif
