#pragma once
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
#include "Form.h"
#define SAFE_RELEASE(object){if(object) object->Release();object = 0;}
class D3DInterface
{
public:

	//defualt constructor-- creates a new instance of D3DInterface with defualt parameters
	D3DInterface();

	//initializes the d3d interface so that it is ready to draw
	//vsyncEnabled- a bool indicating whther vsync should be eneabled or not
	//form- the form to use as paramters(such as screen width,height or for the HWND handle)
	void InitializeD3D(bool vsyncEnabled, Form* form);

	//returns the error message, if there is none this returns NULL( or 0)
	char* GetErrorMessage();

	//returns the D3DDevice
	ID3D11Device* GetDevice();

	//returns the immeditate context
	ID3D11DeviceContext* GetImmediateContext();

	// cleans up memeory for this instance of D3DInteraface
	void Close();

	//begins drawing by first clearing the screen buffer with the passed in color
	//r- the red color component
	//g-the green color componenet
	//b- the blue color componenet
	void BeginDrawing(float r, float g, float b);

	//presents the drawn screen by calling swap on the swap chain(double buffered by defualt)
	void Present();

	void EnableAlphaBlending();

private:

	// a boo indicating whether vertical sync is enabled or not
	bool m_verticalSync;

	//a pointer to the swap chain
	IDXGISwapChain* m_swapChain;
	
	// a pointer to the d3d device
	ID3D11Device* m_device;

	//a pointer to the immediate context
	ID3D11DeviceContext* m_immediateContext;

	//a pointer to the render target
	ID3D11RenderTargetView* m_renderTarget;

	//a pointer to the depth stencil buffer
	ID3D11Texture2D* m_depthStencilBuffer;

	//a pointer to the depth stencil state
	ID3D11DepthStencilState* m_depthStencilState;

	//a pointer to the depth stencil view
	ID3D11DepthStencilView* m_depthStencilView;

	// ap ointer to the rasterizer state
	ID3D11RasterizerState* m_rasterizerState;

	//a pointer to the error message( this is a null pointer by defualt;
	char* m_errorMessage;

	//the blend state
	ID3D11BlendState* m_blendState;
};