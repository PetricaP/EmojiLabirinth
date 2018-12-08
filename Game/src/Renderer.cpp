#include "Utility.h"
#include "Renderer.h"
#include "Utility.h"
#include "Sprite.h"
#include "FileUtils.h"
#include <DDSTextureLoader.h>

namespace d3d11 {

Renderer::Renderer(const win32::Window &window)
	: m_Window(window), m_Device(nullptr), m_DeviceContext(nullptr), m_SwapChain(nullptr),
	  m_DepthStencilBuffer(nullptr), m_DepthStencilView(nullptr) {
	InitDeviceAndContext();
	InitSwapChain();
	InitRenderTarget();
	InitBlendState();
	InitViewPort();
}

Renderer::~Renderer() {
#if defined(DEBUG) || defined(_DEBUG)
	ID3D11Debug *debugObject = nullptr;
	m_Device.get()->QueryInterface(&debugObject);
	if(debugObject) {
		debugObject->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	}
	DX_SAFE_RELEASE(debugObject);
#endif
}

void Renderer::Clear() {
	auto renderTargetView = m_RenderTargetView.get();
	m_DeviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);

	m_DeviceContext->RSSetViewports(1, &m_ViewPort);

	static constexpr float color[] {0.8f, 0.8f, 0.9f, 1.0f};
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.get(), color);

	static constexpr float blendFactor[4] = { 0.0f };
	static constexpr UINT sampleMask   = 0xffffffff;

	m_DeviceContext->OMSetBlendState(m_BlendState.get(), blendFactor, sampleMask);
}

void Renderer::Flush() {
	m_SwapChain->Present(1, 0);
}

void Renderer::SetShader(const d3d11::Shader &shader) const {
	m_DeviceContext->IASetInputLayout(shader.GetInputLayout().m_Handle.get());

	m_DeviceContext->VSSetShader(shader.GetProgram().GetVertexShader(), nullptr, 0);
	m_DeviceContext->PSSetShader(shader.GetProgram().GetPixelShader(), nullptr, 0);
}

void Renderer::Submit(const Drawable &drawable) const {
	m_DeviceContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT offset = 0;
	auto vertexBuffer = drawable.m_VertexBuffer->m_Handle.get();
	m_DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer,
										&drawable.m_VertexSize, &offset);

	ID3D11Buffer *indexBuffer = drawable.m_IndexBuffer->m_Handle.get();
	m_DeviceContext->IASetIndexBuffer(indexBuffer, 
									  DXGI_FORMAT_R32_UINT, 0);

	m_DeviceContext->DrawIndexed(drawable.m_NumIndices, 0, 0);
}

void Renderer::Submit(const Sprite &sprite) const {
	Submit(static_cast<const Drawable &>(sprite));
	SetTexture(sprite.m_Texture, 0);
}

VertexBuffer Renderer::CreateVertexBuffer(const void *data, UINT size) const {
	VertexBuffer vertexBuffer;
	D3D11_BUFFER_DESC vertexBufferDesc =
		CD3D11_BUFFER_DESC(size, D3D11_BIND_VERTEX_BUFFER);
	D3D11_SUBRESOURCE_DATA vertexData{0};
	vertexData.pSysMem = data;

	DXCall(m_Device->CreateBuffer(&vertexBufferDesc, &vertexData,
												vertexBuffer.m_Handle.getAddressOf()));
	return vertexBuffer;
}

IndexBuffer Renderer::CreateIndexBuffer(const void *data, UINT size) const {
	IndexBuffer indexBuffer;

	D3D11_BUFFER_DESC indexBufferDesc =
		CD3D11_BUFFER_DESC(size, D3D11_BIND_INDEX_BUFFER);
	D3D11_SUBRESOURCE_DATA indexData{0};
	indexData.pSysMem = data;

	DXCall(m_Device->CreateBuffer(&indexBufferDesc, &indexData, 
		indexBuffer.m_Handle.getAddressOf()));

	return indexBuffer;
}

ConstantBuffer Renderer::CreateConstantBuffer(UINT size,
											  D3D11_USAGE usage) const {
	ConstantBuffer constantBuffer;

	constantBuffer.m_Size = size;

	D3D11_BUFFER_DESC constantBufferDesc{};
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.ByteWidth = size;
	constantBufferDesc.Usage = usage;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	DXCall(m_Device->CreateBuffer(&constantBufferDesc, nullptr,
										 constantBuffer.m_Handle.getAddressOf()));

	return constantBuffer;
}

Font Renderer::CreateFont(const std::string name) {
	Font font;
	d3d11::unique_handle<IFW1Factory> pFW1Factory;
	DXCall(FW1CreateFactory(FW1_VERSION, pFW1Factory.getAddressOf()));

	std::wstring wname(name.begin(), name.end());
	DXCall(pFW1Factory->CreateFontWrapper(m_Device.get(), wname.c_str(),
		font.m_Handle.getAddressOf()));
	return font;
}

void Renderer::RenderText(const Font &font, const std::string &text, float x, float y, float width,
						uint32_t color) const {
	font.m_Handle->DrawString( m_DeviceContext.get(), L"Hello DirectX", width * Font::m_ScreenX,
		(x + 0.5f) * Font::m_ScreenX, (y + 0.5f) * Font::m_ScreenY, color, FW1_RESTORESTATE );
}

ShaderProgram Renderer::CreateShaderProgram(const std::string &vsFilePath,
											const std::string &psFilePath) const {
	ShaderProgram program;
	
	program.m_VSData = load_file(vsFilePath);
	program.m_PSData = load_file(psFilePath);

	DXCall(m_Device->CreateVertexShader(
		program.m_VSData.data(), program.m_VSData.size(), nullptr, 
		program.m_VertexShader.getAddressOf()));

	DXCall(m_Device->CreatePixelShader(
		program.m_PSData.data(), program.m_PSData.size(), nullptr,
		program.m_PixelShader.getAddressOf()));

	return program;
}

InputLayout Renderer::CreateInputLayout(
	const std::vector<InputElement> &inputElements, const ShaderProgram &shader) const {

	std::vector<D3D11_INPUT_ELEMENT_DESC> layout(inputElements.size());

	layout[0] = {inputElements[0].name.c_str(), 0,
						 inputElements[0].format, 0, 0,
						 D3D11_INPUT_PER_VERTEX_DATA, 0};
	for (unsigned int i = 1; i < inputElements.size(); ++i) {
		layout[i] = {inputElements[i].name.c_str(), 0,
			inputElements[i].format, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0};
	}

	InputLayout inputLayout;

	DXCall(m_Device->CreateInputLayout(layout.data(), static_cast<UINT>(layout.size()),
		shader.GetVertexShaderData().data(), static_cast<UINT>(shader.GetVertexShaderData().size()),
		inputLayout.m_Handle.getAddressOf()));

	return inputLayout;
}

Texture Renderer::CreateTexture(const std::string &filePath) const {
	Texture texture;
	
	std::wstring wFilePath;
	wFilePath.assign(filePath.begin(), filePath.end());

	DXCall(DirectX::CreateDDSTextureFromFile(m_Device.get(), wFilePath.c_str(),
		nullptr, texture.m_Handle.getAddressOf()));

	D3D11_SAMPLER_DESC samplerDesc;

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	DXCall(m_Device->CreateSamplerState(&samplerDesc, texture.m_SampleState.getAddressOf()));

	return texture;
}

void Renderer::PSSetConstantBuffer(const ConstantBuffer &buffer, UINT slot) const {
	ID3D11Buffer *handle = buffer.m_Handle.get();
	m_DeviceContext->PSSetConstantBuffers(0, 1, &handle);
}

void Renderer::VSSetConstantBuffer(const ConstantBuffer &buffer, UINT slot) const {
	ID3D11Buffer *handle = buffer.m_Handle.get();
	m_DeviceContext->VSSetConstantBuffers(0, 1, &handle);
}

void Renderer::SetTexture(const Texture &texture, UINT slot) const {
	auto handle = texture.m_Handle.get();
	m_DeviceContext->PSSetShaderResources(slot, 1, &handle);

	auto sampleState = texture.m_SampleState.get();
	m_DeviceContext->PSSetSamplers(slot, 1, &sampleState);
}

void Renderer::InitRenderTarget() {
	ID3D11Texture2D *backBuffer;
	DXCall(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		reinterpret_cast<void **>(&backBuffer)));

	DXCall(m_Device->CreateRenderTargetView(backBuffer, nullptr,
		m_RenderTargetView.getAddressOf()));

	DX_SAFE_RELEASE(backBuffer);
}

void Renderer::InitDepthStencilBuffer() {
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = m_Window.GetWidth();
	depthStencilDesc.Height = m_Window.GetHeight();
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	DXCall(m_Device->CreateTexture2D(&depthStencilDesc, 0,
			m_DepthStencilBuffer.getAddressOf()));

	DXCall(m_Device->CreateDepthStencilView(m_DepthStencilBuffer.get(), 0,
											m_DepthStencilView.getAddressOf()));
}

void Renderer::InitBlendState() {
	D3D11_BLEND_DESC blendStateDesc;

	blendStateDesc.AlphaToCoverageEnable = FALSE;
	blendStateDesc.IndependentBlendEnable = FALSE;

	blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	DXCall(m_Device->CreateBlendState(&blendStateDesc, m_BlendState.getAddressOf()));
}

void Renderer::InitViewPort() {
	m_ViewPort =
		CD3D11_VIEWPORT(0.0f, 0.0f, static_cast<float>(m_Window.GetWidth()),
						static_cast<float>(m_Window.GetHeight()));
}

void Renderer::MapResource(const ConstantBuffer &buffer, const void *data) const {
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	DXCall(m_DeviceContext->Map(
		buffer.m_Handle.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

	memcpy(mappedResource.pData, data, buffer.m_Size);

	m_DeviceContext->Unmap(buffer.m_Handle.get(), 0);
}

void Renderer::InitSwapChain() {
	DXGI_SWAP_CHAIN_DESC swapchainDesc;

	swapchainDesc.BufferDesc.Width = m_Window.GetWidth();
	swapchainDesc.BufferDesc.Height = m_Window.GetHeight();
	swapchainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.BufferDesc.ScanlineOrdering =
		DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.SampleDesc.Quality = 0;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.BufferCount = 1;
	swapchainDesc.OutputWindow = static_cast<HWND>(m_Window.GetAPIHandle());
	swapchainDesc.Windowed = true;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapchainDesc.Flags = 0;

	IDXGIDevice *dxgiDevice = nullptr;
	m_Device->QueryInterface(__uuidof(IDXGIDevice), (void **)&dxgiDevice);
	IDXGIAdapter *dxgiAdapter = nullptr;
	DXCall(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));
	IDXGIFactory *dxgiFactory = nullptr;
	DXCall(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory)); 

	DXCall(dxgiFactory->CreateSwapChain(m_Device.get(), &swapchainDesc,
		m_SwapChain.getAddressOf())); 

	DX_SAFE_RELEASE(dxgiFactory);
	DX_SAFE_RELEASE(dxgiAdapter);
	DX_SAFE_RELEASE(dxgiDevice);
}

void Renderer::InitDeviceAndContext() {
	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevel;

	DXCall(D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, createDeviceFlags, 0, 0,
								   D3D11_SDK_VERSION, m_Device.getAddressOf(),
								   &featureLevel, m_DeviceContext.getAddressOf()));
}

} // namespace d3d11

