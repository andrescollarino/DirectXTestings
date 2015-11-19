#include "Shader.h"

Shader::Shader(ID3D11Device * device, HWND hwnd, LPCSTR shaderFileName, LPCSTR vertexFucName, LPCSTR pixelFunName)
{
	mp_vertexShader = nullptr;
	mp_pixelShader = nullptr;
	mp_layaout = nullptr;
	mp_matrixBuffer = nullptr;
	mp_name = nullptr;

	m_initialized = Initialize(device, hwnd, shaderFileName, vertexFucName, pixelFunName);
}

Shader::~Shader()
{
	if (mp_vertexShader != nullptr)
	{
		mp_vertexShader->Release();
		mp_vertexShader = nullptr;
	}

	if (mp_pixelShader != nullptr)
	{
		mp_pixelShader->Release();
		mp_pixelShader = nullptr;
	}

	if (mp_layaout != nullptr)
	{
		mp_layaout->Release();
		mp_layaout = nullptr;
	}

	if (mp_matrixBuffer != nullptr)
	{
		mp_matrixBuffer->Release();
		mp_matrixBuffer = nullptr;
	}

	if (mp_name != nullptr)
	{
		delete[] mp_name;
		mp_name = nullptr;
	}
}

void Shader::Begin(ID3D11DeviceContext * deviceContext, int indexCount)
{
	// Cambio el layout
	deviceContext->IASetInputLayout(mp_layaout);

	// Cambio el vertex an pixel shader
	deviceContext->VSSetShader(mp_vertexShader, NULL, 0);
	deviceContext->PSSetShader(mp_pixelShader, NULL, 0);

	//Renderisado
	deviceContext->DrawIndexed(indexCount, 0, 0);
}

void Shader::End(ID3D11DeviceContext * deviceContext)
{
	deviceContext->IASetInputLayout(NULL);

	deviceContext->VSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(NULL, NULL, 0);
}

bool Shader::SetShaderParameters(ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture)
{
	deviceContext->PSSetShaderResources(0, 1, &texture);
	return true;
}

bool Shader::SetShaderParameters(ID3D11DeviceContext * deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectioNMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType * dataPtr;

	// Transpose todas las matrises 
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectioNMatrix, &projectioNMatrix);

	// Lock
	result = deviceContext->Map(mp_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		//TODO: Informar del Error
		return false;
	}

	// Edit
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->worldMatrix = worldMatrix;
	dataPtr->viewMatrix = viewMatrix;
	dataPtr->projectionMatrix = projectioNMatrix;

	// unlock
	deviceContext->Unmap(mp_matrixBuffer, 0);

	// actualizo los valores en el shader
	deviceContext->VSSetConstantBuffers(0, 1, &mp_matrixBuffer);

	return true;
}

char * Shader::GetName()
{
	return mp_name;
}

bool Shader::IsInitialized()
{
	return m_initialized;
}

bool Shader::Initialize(ID3D11Device * device, HWND hwnd, LPCSTR shaderFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName)
{

	bool result;
	int lenghFileName = strlen(shaderFileName) + 1;

	mp_name = new char[lenghFileName];

	memcpy(mp_name, shaderFileName, lenghFileName);

	char * psFileName = new char[lenghFileName + 5];

	strcpy_s(psFileName, lenghFileName + 5, shaderFileName);
	strcat_s(psFileName, lenghFileName + 5, ".ps\0");


	char * vsFileName = new char[lenghFileName + 5];

	strcpy_s(vsFileName, lenghFileName + 5, shaderFileName);
	strcat_s(vsFileName, lenghFileName + 5, ".vs\0");

	result = initializeShader(device, hwnd, vsFileName, psFileName, vertexFuncName, pixelFuncName);

	delete[] psFileName;
	delete[] vsFileName;

	return result;
}

bool Shader::initializeShader(ID3D11Device * device, HWND hwnd, LPCSTR vsFileName, LPCSTR psFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName)
{
	HRESULT result;
	ID3D10Blob * errorMessage = nullptr;
	ID3D10Blob * vertexShaderBuffer = nullptr;
	ID3D10Blob * pixelShaderBuffer = nullptr;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC maxtrixBufferDesc;

	// Compilo el vertex shader
	result = D3DX11CompileFromFile(
		vsFileName,
		NULL,
		NULL,
		vertexFuncName,
		"vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		NULL,
		&vertexShaderBuffer,
		&errorMessage,
		NULL
		);

	if (FAILED(result))
	{
		if (errorMessage != nullptr)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFileName);
		}
		else
		{
			MessageBox(hwnd, vsFileName, "Error in Shader File", MB_OK);
		}

		return false;
	}

	// Compilo el pixel shader
	result = D3DX11CompileFromFile(
		psFileName,
		NULL,
		NULL,
		pixelFuncName,
		"vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		NULL,
		&pixelShaderBuffer,
		&errorMessage,
		NULL
		);

	if (FAILED(result))
	{
		if (errorMessage != nullptr)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFileName);
		}
		else
		{
			MessageBox(hwnd, vsFileName, "Error in Shader File", MB_OK);
		}

		return false;
	}


	// creo el vertex shader buffer
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &mp_vertexShader);
	if (FAILED(result))
	{
		//TODO: Informar del Error
		return false;
	}

	// Creo el pixel shader
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &mp_pixelShader);
	if (FAILED(result))
	{
		//TODO: Informar del Error
		return false;
	}


	// Configuro el layaut 
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	result = device->CreateInputLayout(
		polygonLayout,
		numElements,
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		&mp_layaout
		);

	if (FAILED(result))
	{
		//TODO: Informar del Error
		return false;
	}

	// Released Buffers
	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	// Configuro la matrix Buffer Description
	maxtrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	maxtrixBufferDesc.ByteWidth = sizeof(D3D11_BUFFER_DESC);
	maxtrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	maxtrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	maxtrixBufferDesc.MiscFlags = 0;
	maxtrixBufferDesc.StructureByteStride = 0;

	// Creo el puntero al buffer constante
	result = device->CreateBuffer(&maxtrixBufferDesc, NULL, &mp_matrixBuffer);
	if (FAILED(result))
	{
		//TODO: Informar del Error
		return false;
	}

	return true;

}

void Shader::OutputShaderErrorMessage(ID3D10Blob * errorMessage, HWND hwnd, LPCSTR shaderFileName)
{
	ofstream fout;
	char* compileErrors = (char*)errorMessage->GetBufferPointer();
	unsigned long bufferSize = errorMessage->GetBufferSize();

	fout.open("shader-errors");

	for (unsigned int i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	fout.close();

	errorMessage->Release();
	errorMessage = nullptr;

	MessageBox(hwnd, "Error compiling shader. Check shader-errors" , shaderFileName , MB_OK);

}
