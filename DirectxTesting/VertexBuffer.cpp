#include "VertexBuffer.h"



VertexBuffer::VertexBuffer()
{
	mp_indexBuffer = nullptr;
	mp_shader = nullptr;
	mp_vertexBuffer = nullptr;
	mp_vertices = nullptr;
	m_vertexCount = 0;
	m_indexCount = 0;
}


VertexBuffer::~VertexBuffer()
{
	if (mp_vertices != nullptr)
	{
		delete[] mp_vertices;
		mp_vertices = nullptr;
	}

	if (mp_vertexBuffer != nullptr)
	{
		mp_vertexBuffer->Release();
		mp_vertexBuffer = nullptr;
	}

	if (mp_indexBuffer != nullptr)
	{
		mp_indexBuffer->Release();
		mp_indexBuffer = nullptr;
	}
}

bool VertexBuffer::Initialize(ID3D11Device * device, Shader * shader, float size, bool writeable)
{
	unsigned long * indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	m_vertexCount = 4;
	m_indexCount = 6;

	mp_vertices = new VertexType[m_vertexCount];
	indices = new unsigned long[m_indexCount];

	float halfSize = size / 2.0f;

	// Vertices de mi cuadrado
	mp_vertices[0].position = D3DXVECTOR3(-halfSize, -halfSize, 0.0f);
	mp_vertices[0].uv = D3DXVECTOR2(0.0f, 1.0f);

	mp_vertices[1].position = D3DXVECTOR3(-halfSize, halfSize, 0.0f);
	mp_vertices[1].uv = D3DXVECTOR2(0.0f, 0.0f);

	mp_vertices[2].position = D3DXVECTOR3(halfSize, halfSize, 0.0f);
	mp_vertices[2].uv = D3DXVECTOR2(1.0f, 0.0f);

	mp_vertices[3].position = D3DXVECTOR3(halfSize, -halfSize, 0.0f);
	mp_vertices[3].uv = D3DXVECTOR2(1.0f, 1.0f);

	// Creo mis triangulos con los indises
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	// Set VertexBuffer
	vertexBufferDesc.Usage = (writeable) ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = (writeable) ? D3D11_CPU_ACCESS_WRITE : D3D11_CPU_ACCESS_READ;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Set up Vetex Data
	vertexData.pSysMem = mp_vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Creo el Vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mp_vertexBuffer);
	if (FAILED(result))
	{
		//TODO: Informar del Error
		return false;
	}

	//Set Index Buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;


	result = device->CreateBuffer(&indexBufferDesc, &indexData, &mp_indexBuffer);
	if (FAILED(result))
	{
		//TODO: Informar del Error
		return false;
	}

	delete[] indices;
	indices = 0;

	mp_shader = shader;
	return true;
}

void VertexBuffer::Render(ID3D11DeviceContext * deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	mp_shader->Begin(deviceContext, m_indexCount);
	deviceContext->IASetVertexBuffers(0, 1, &mp_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(mp_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

VertexBuffer::VertexType * VertexBuffer::GetVertices()
{
	return mp_vertices;
}

ID3D11Buffer * VertexBuffer::GetVertexBuffer()
{
	return mp_vertexBuffer;
}

int VertexBuffer::GetIndexCount()
{
	return m_indexCount;
}
