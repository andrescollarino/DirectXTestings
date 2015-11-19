#ifndef _SHADER_H
#define _SHADER_H

#include <D3D11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
//#include <string>
#include <fstream>

using namespace std;


class Shader
{
private:

	struct MatrixBufferType
	{
		D3DXMATRIX worldMatrix;
		D3DXMATRIX viewMatrix;
		D3DXMATRIX projectionMatrix;
	};

public:
	Shader(ID3D11Device * device , HWND hwnd , LPCSTR shaderFileName , LPCSTR vertexFucName , LPCSTR pixelFunName);

	virtual ~Shader();

	virtual void Begin(ID3D11DeviceContext * deviceContext, int indexCount);
	virtual void End(ID3D11DeviceContext * deviceContext);
	
	bool SetShaderParameters(ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture);
	bool SetShaderParameters(ID3D11DeviceContext * deviceContext, D3DXMATRIX worldMatrix , D3DXMATRIX viewMatrix , D3DXMATRIX projectioNMatrix);

	char * GetName();

	bool IsInitialized();

protected:

	virtual bool Initialize(ID3D11Device * device, HWND hwnd, LPCSTR shaderFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName);

private:

	bool initializeShader(ID3D11Device * device, HWND hwnd, LPCSTR vsFileName, LPCSTR psFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName);
	void OutputShaderErrorMessage(ID3D10Blob * errorMessage, HWND hwnd, LPCSTR shaderFileName);

	ID3D11VertexShader * mp_vertexShader;
	ID3D11PixelShader * mp_pixelShader;
	ID3D11InputLayout * mp_layaout;
	ID3D11Buffer * mp_matrixBuffer;
	char * mp_name;
	bool m_initialized;
};

#endif
