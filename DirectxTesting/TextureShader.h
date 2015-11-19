#ifndef _TEXTURE_SHADER_H
#define _TEXTURE_SHADER_H

#include "Shader.h"

class TextureShader : public Shader 
{
public:

	TextureShader(ID3D11Device * device, HWND hwnd, LPCSTR shaderFileName, LPCSTR vertexFucName, LPCSTR pixelFunName);
	virtual ~TextureShader();

	void Begin(ID3D11DeviceContext * deviceContext, int indexCount);
	void End(ID3D11DeviceContext * deviceContext);

protected:

	bool Initialize(ID3D11Device * device, HWND hwnd, LPCSTR shaderFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName);

private :

	ID3D11SamplerState * mp_samplerState;
};

#endif