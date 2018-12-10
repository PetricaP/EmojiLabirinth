#pragma once

#include "ConstantBuffer.h"
#include "Drawable.h"
#include "Font.h"
#include "InputLayout.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "WindowWIN32.h"
#include <d3d11.h>
#include <string>

class Sprite;

struct Color {
	float r, g, b, a;

	Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {
	}
};

namespace color {

static const Color RED{1.0f, 0.0f, 0.0f, 1.0f};
static const Color BLUE{0.0f, 0.0f, 1.0f, 1.0f};
static const Color GREEN{0.0f, 0.0f, 0.0f, 1.0f};
static const Color GRAY{0.2f, 0.2f, 0.2f, 1.0f};
static const Color CHERRY{0.5f, 0.2f, 0.2f, 1.0f};

}

namespace d3d11 {

class Renderer {
  private:
	const win32::Window &m_Window;

	unique_handle<ID3D11Device> m_Device;
	unique_handle<ID3D11DeviceContext> m_DeviceContext;
	unique_handle<IDXGISwapChain> m_SwapChain;
	unique_handle<ID3D11Texture2D> m_DepthStencilBuffer;
	unique_handle<ID3D11DepthStencilView> m_DepthStencilView;
	unique_handle<ID3D11RenderTargetView> m_RenderTargetView;
	unique_handle<ID3D11BlendState> m_BlendState;

	D3D11_VIEWPORT m_ViewPort;

	float m_ClearColor[4];

  public:
	Renderer(const win32::Window &window);
	~Renderer();

	void Clear();
	void Flush();

	void Submit(const Drawable &drawable) const;

	void Submit(const Sprite &sprite) const;

	VertexBuffer CreateVertexBuffer(const void *data, UINT size) const;
	IndexBuffer CreateIndexBuffer(const void *data, UINT size) const;
	ConstantBuffer CreateConstantBuffer(UINT size, D3D11_USAGE usage) const;
	Font CreateFont(const std::string name);

	void RenderText(const Font &font, const std::string &text, float x, float y,
					float width, uint32_t color) const;

	ShaderProgram CreateShaderProgram(const std::string &vsFilePath,
									  const std::string &psFilePath) const;

	InputLayout
	CreateInputLayout(const std::vector<InputElement> &inputElements,
					  const ShaderProgram &shader) const;

	Texture CreateTexture(const std::string &filePath) const;

	void MapResource(const ConstantBuffer &buffer, const void *data) const;
	void PSSetConstantBuffer(const ConstantBuffer &buffer, UINT slot) const;
	void VSSetConstantBuffer(const ConstantBuffer &buffer, UINT slot) const;

	void SetTexture(const Texture &texture, UINT slot) const;
	void SetShader(const d3d11::Shader &shader) const;

	void SetClearColor(const Color &color);

	Renderer(const Renderer &other) = delete;
	Renderer &operator=(const Renderer &other) = delete;

	/*
	ID3D11Device *GetDevice() const { return m_Device.get(); }
	ID3D11DeviceContext *GetContext() const { return m_DeviceContext.get(); }
	*/

  private:
	void InitRenderTarget();
	void InitSwapChain();
	void InitDeviceAndContext();
	void InitDepthStencilBuffer();
	void InitBlendState();
};

} // namespace d3d11
