#ifndef _VERTEX_BUFFER_H
#define _VERTEX_BUFFER_H

#include "Shader.h"
#include <D3D11.h>
#include <D3DX10math.h>

class VertexBuffer
{
public:
	
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 uv;
	};

	VertexBuffer();
	~VertexBuffer();

	bool Initialize(ID3D11Device * device, Shader * shader, float size, bool writeable = false);
	void Render(ID3D11DeviceContext * deviceContext);

	VertexType *GetVertices();
	ID3D11Buffer * GetVertexBuffer();
	int GetIndexCount();

private:

	ID3D11Buffer * mp_vertexBuffer;
	ID3D11Buffer * mp_indexBuffer;
	VertexType * mp_vertices;
	int m_vertexCount;
	int m_indexCount;
	Shader * mp_shader;
};

#endif
