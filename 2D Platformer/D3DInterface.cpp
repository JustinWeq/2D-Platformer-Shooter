#include "D3DInterface.h"

D3DInterface::D3DInterface()
{
	m_depthStencilBuffer = NULL;
	m_depthStencilState = NULL;
	m_depthStencilView = NULL;
	m_device = NULL;
	m_immediateContext = NULL;
	m_rasterizerState = NULL;
	m_renderTarget = NULL;
	m_swapChain = NULL;
	m_errorMessage = NULL;
	m_verticalSync = false;
}

void D3DInterface::InitializeD3D(bool vsyncEnabled, Form* form)
{
	//store the vertical sync state
	m_verticalSync = vsyncEnabled;

	//definitions of temporary variables to be used for D3DInitializtion
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, numerator, denominator, stringlength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adpaterDescription;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDescription;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBuffer;
	D3D11_TEXTURE2D_DESC depthBufferDescription;
	D3D11_DEPTH_STENCIL_DESC depthStencilDescription;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDescription;
	D3D11_RASTERIZER_DESC rasterizerDescription;
	D3D11_VIEWPORT viewPort;
	float fieldOfView, screenAspectRatio;

	//create the dxgi factory
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		//set the error string
		m_errorMessage = "There was an error creating the dxgi factory";
		return;
	}

	//use the factory to create the DXGI adpater
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		m_errorMessage = "There was an error enumerating the adapter";
		return;
	}

	//enumerate the primary adaptor outputs
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		m_errorMessage = "The was an error enumerating the adapter output";
		return;
	}

	//Get the number of modes that support the DXGI_FORMAT_R8G8B8A8_UNORM output
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		m_errorMessage = "The was an error getting the number of display modes that support DXGI_ENUM_MODES_INTERLACED";
		return;
	}

	//create a list to hold all of the possible display modes
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		m_errorMessage = "Failed to intitalize the lists of display modes";
		return;
	}

	//now fill the display modes
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		m_errorMessage = "Failed to fill the display mode list";
		return;
	}

	//Now go through all the display modes and find the one that matches the display mode width and height
	for (int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)(form->GetWidth()) && displayModeList[i].Height == (unsigned int)(form->GetHeight()))
		{
			//Select this display mode
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
			break;
		}
	}

	//delete the display mode list
	delete[] displayModeList;
	displayModeList = NULL;

	//release the adaptor output
	adapterOutput->Release();
	adapterOutput = NULL;

	//release the adaptor
	adapter->Release();
	adapter = NULL;

	//release the factory
	factory->Release();
	factory = NULL;

	//init the swap chain description
	ZeroMemory(&swapChainDescription, sizeof(swapChainDescription));

	//set to a single back buffer
	swapChainDescription.BufferCount = 1;

	//set the width and height of the back buffer
	swapChainDescription.BufferDesc.Width = form->GetWidth();
	swapChainDescription.BufferDesc.Height = form->GetHeight();

	//set the surface of the back buffer to a regular 32 bit format
	swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//set the refresh rate of the back buffer
	if(m_verticalSync)
	{
		swapChainDescription.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDescription.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDescription.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	//set the window for the swap chain to render to
	swapChainDescription.OutputWindow = form->GetHwnd();

	//turn MSAA off
	swapChainDescription.SampleDesc.Count = 1;
	swapChainDescription.SampleDesc.Quality = 0;

	//set fullscreen mode
	swapChainDescription.Windowed = !form->GetFullScreen();

	//set the scanline ordering and spacing to unspecified
	swapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//discard the back buffer contents after presenting
	swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//set the flags to 0
	swapChainDescription.Flags = 0;

	//set the feature level to directx 11
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	//create the swap chain, device and the immediate context
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDescription,
		&m_swapChain, &m_device, NULL, &m_immediateContext);
	if (FAILED(result))
	{
		m_errorMessage = "Failed to create the device, swapchain and immediate context";
		return;
	}

	//Get the pointer to the back buffer
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(result))
	{
		m_errorMessage = "Failed to get a pointer to the back buffer";
		return;
	}

	//create the render target view with the back buffer pointer
	result = m_device->CreateRenderTargetView(backBuffer, NULL, &m_renderTarget);
	if (FAILED(result))
	{
		m_errorMessage = "Failed to create the render target view";
		return;
	}

	//release the back buffer
	backBuffer->Release();
	backBuffer = NULL;

	//init the depth buffer description
	ZeroMemory(&depthBufferDescription, sizeof(depthBufferDescription));

	//set up the description of the depth buffer
	depthBufferDescription.Width = form->GetWidth();
	depthBufferDescription.Height = form->GetHeight();
	depthBufferDescription.MipLevels = 1;
	depthBufferDescription.ArraySize = 1;
	depthBufferDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDescription.SampleDesc.Count = 1;
	depthBufferDescription.SampleDesc.Quality = 0;
	depthBufferDescription.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDescription.CPUAccessFlags = 0;
	depthBufferDescription.MiscFlags = 0;

	//create the texture for the depth buffer
	result = m_device->CreateTexture2D(&depthBufferDescription, NULL, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		m_errorMessage = "The was an error creating the texture for the depth buffer";
		return;
	}

	//inititialize the depscription of the depth stencil state
	ZeroMemory(&depthStencilDescription, sizeof(depthStencilDescription));

	//set up the description of the depth stencil state
	depthStencilDescription.DepthEnable = false;
	depthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDescription.StencilEnable = true;
	depthStencilDescription.StencilReadMask = 0xFF;
	depthStencilDescription.StencilWriteMask = 0xFF;

	//stencil operations if triangle is front facing
	depthStencilDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

	//stencil operations if triangle is back facing
	depthStencilDescription.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescription.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

	//create the depth stencil state
	result = m_device->CreateDepthStencilState(&depthStencilDescription, &m_depthStencilState);
	if (FAILED(result))
	{
		m_errorMessage = "Failed to create the depth stencil state";
		return;
	}

	//set the depth stencil state
	m_immediateContext->OMSetDepthStencilState(m_depthStencilState, 0);

	//initialize the depth stencil view
	ZeroMemory(&depthStencilViewDescription, sizeof(depthStencilViewDescription));

	//Set up the depth stencil view description
	depthStencilViewDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDescription.Texture2D.MipSlice = 0;

	//create the depth stencil view
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDescription, &m_depthStencilView);
	if (FAILED(result))
	{
		m_errorMessage = "Failed to create the depth stencil view";
		return;
	}

	//bind the depth stencil view and render target
	m_immediateContext->OMSetRenderTargets(1, &m_renderTarget, m_depthStencilView);

	//set up the rasterizer description
	rasterizerDescription.AntialiasedLineEnable = false;
	rasterizerDescription.CullMode = D3D11_CULL_BACK;
	rasterizerDescription.DepthBias = 0;
	rasterizerDescription.DepthBiasClamp = 0;
	rasterizerDescription.DepthClipEnable = true;
	rasterizerDescription.FillMode = D3D11_FILL_SOLID;
	rasterizerDescription.FrontCounterClockwise = false;
	rasterizerDescription.MultisampleEnable = false;
	rasterizerDescription.ScissorEnable = false;
	rasterizerDescription.ScissorEnable = 0;

	//create the rasterizer state
	result = m_device->CreateRasterizerState(&rasterizerDescription, &m_rasterizerState);
	if (FAILED(result))
	{
		m_errorMessage = "Failed to create rasterizer state";
		return;
	}

	//bind the rasterizer state
	m_immediateContext->RSSetState(m_rasterizerState);

	//set up the view port for rendering
	viewPort.Height = form->GetHeight();
	viewPort.Width = form->GetWidth();
	viewPort.MaxDepth = 1;
	viewPort.MinDepth = 0;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;

	//create the viewport
	m_immediateContext->RSSetViewports(1, &viewPort);

	D3D11_BLEND_DESC blendStateDescription;

	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	//create the blens state and then set it
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;


	//create the blend state
	result = m_device->CreateBlendState(&blendStateDescription, &m_blendState);


	//release the blend state since it is no longer needed


}

char * D3DInterface::GetErrorMessage()
{
	return m_errorMessage;
}

ID3D11Device * D3DInterface::GetDevice()
{
	return m_device;
}

ID3D11DeviceContext * D3DInterface::GetImmediateContext()
{
	return m_immediateContext;
}

void D3DInterface::Close()
{
	//release all of the data
	SAFE_RELEASE(m_depthStencilBuffer);

	SAFE_RELEASE(m_depthStencilState);

	SAFE_RELEASE(m_depthStencilView);

	SAFE_RELEASE(m_device);

	SAFE_RELEASE(m_immediateContext);

	SAFE_RELEASE(m_rasterizerState);

	SAFE_RELEASE(m_renderTarget);

	SAFE_RELEASE(m_swapChain);

	delete[]m_errorMessage;
	m_errorMessage = NULL;
}

void D3DInterface::BeginDrawing(float r, float g, float b)
{

	//make a color array

	float color[] = { r,g,b,1 };

	//clear the render target
	m_immediateContext->ClearRenderTargetView(m_renderTarget,color);

	//clear the depth buffer
	m_immediateContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1, 0);

}

void D3DInterface::Present()
{
	//present the swap chain
	if (m_verticalSync)
	{
		//lock the screen to the refresh rate
		m_swapChain->Present(1, 0);
	}
	else
	{
		//present as fast as possible
		m_swapChain->Present(0, 0);
	}
}

void D3DInterface::EnableAlphaBlending()
{
	float blendFactor[4];

	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	//set the blend state
	m_immediateContext->OMSetBlendState(m_blendState, blendFactor, 0xffffffff);
}
