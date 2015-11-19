#include "Sprite.h"



Sprite::Sprite(float size)
{
	mp_vertexBuffer = nullptr;
	mp_texture = nullptr;

	m_size = size;
}


Sprite::~Sprite()
{
	if (mp_vertexBuffer != nullptr)
	{
		delete mp_vertexBuffer;
		mp_vertexBuffer = nullptr;
	}
}

void Sprite::Initialize(ID3D11Device * device, Shader * shader, LPCTSTR textureFileName)
{
	mp_shader = shader;
	
	mp_vertexBuffer = new VertexBuffer();
	mp_vertexBuffer->Initialize(device, shader, m_size);

	if (textureFileName != nullptr)
	{
		mp_texture = new Texture();
		mp_texture->Initialize(device, textureFileName);
	}
}

void Sprite::Render(ID3D11DeviceContext * deviceConstext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	if (mp_texture == nullptr)
		return;
	
	mp_shader->SetShaderParameters(deviceConstext, mp_texture->GetTexture());
	mp_shader->SetShaderParameters(deviceConstext, worldMatrix, viewMatrix, projectionMatrix);

	mp_vertexBuffer->Render(deviceConstext);
}
