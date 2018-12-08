#include "Game.h"
#include "Utility.h"
#include "FileUtils.h"
#include <fstream>
#include <DDSTextureLoader.h>
#include <sstream>
#include <vector>

static constexpr SimpleShader::Vertex vertices[] = {
	{-1.0f, -1.0f,  0.0f,  0.0f},
	{-1.0f,  1.0f,  0.0f,  1.0f},
	{ 1.0f,  1.0f,  1.0f,  1.0f},
	{ 1.0f, -1.0f,  1.0f,  0.0f}
};

static constexpr unsigned int indices[] = {0, 1, 2, 0, 2, 3};

/* Entry point */
Application *create_application() {
	return new Game;
}

Game::Game() : m_Window("Hello DirectX!", 800, 600), m_Renderer(m_Window),
	m_SimpleShader(m_Renderer) {

	d3d11::Font::Init(m_Window.GetWidth(), m_Window.GetHeight());
	m_Window.AddEventListener(this);

	m_Texture = m_Renderer.CreateTexture("emoji.dds");
	m_MBuffer.projection = DirectX::XMMatrixScaling(m_Window.GetAspectRatio(),
													1.0f, 1.0f);
	m_Font = m_Renderer.CreateFont("Arial");

	m_Transform.SetScale(DirectX::XMFLOAT3(0.1f, 0.1f, 1.0f));
	m_Transform.SetRotation(DirectX::XMFLOAT3(0.0f, 0.0f, 180.0f));

	m_EmojiSprite = std::make_unique<Sprite>(m_Renderer, m_Texture);
}

int Game::Run() {
	m_Timer.Reset();
	while (!m_Window.ShouldClose()) {
		m_Window.PollEvents();
		m_Timer.Tick();
		Game::Update();
		Game::Draw();
		m_Timer.CapFrameTime(0.006f);
	}
	return 0;
}

void Game::Update() {
	static float c = 0.0f;

	m_Transform.SetPosition(DirectX::XMFLOAT3(sinf(c), cosf(c), 0.0f));
	m_MBuffer.model =  m_Transform.GetModel();

	c += 0.04f;
}

void Game::Draw() {
	m_Renderer.Clear();

	m_SimpleShader.UpdateBuffer(m_MBuffer);
	m_Renderer.SetShader(m_SimpleShader);

	m_Renderer.Submit(*m_EmojiSprite);

	m_Renderer.RenderText(m_Font, "Hello DirectX!", -0.3f, -0.2f, 0.1f, 0xff5599ff);

	m_Renderer.Flush();
}

void Game::ProcessMouseButtonEvent(const MouseButtonEvent &event) {
	DEBUG_LOG("The mouse has been pressed.\n");
}

void Game::ProcessKeyboardEvent(const KeyboardEvent &event) {
	DEBUG_LOG("A key has been pressed.\n");
}

void Game::ProcessWindowResizeEvent(const WindowResizeEvent &event) {
	DEBUG_LOG("The size has changed to: ");

	std::stringstream ss;
	ss << event.GetWidth() << " " << event.GetHeight() << std::endl;
	DEBUG_LOG(ss.str().c_str());
	DEBUG_LOG("\n");

	m_Window.SetHeight(event.GetHeight());
	m_Window.SetWidth(event.GetWidth());

	m_MBuffer.projection = DirectX::XMMatrixScaling(m_Window.GetAspectRatio(), 1.0f, 1.0f);
}

void Game::OnEvent(const Event &e) {
	switch(e.GetType()) { 
	case Event::Type::MOUSE_BUTTON:
		ProcessMouseButtonEvent(static_cast<const MouseButtonEvent &>(e));
		break;
	case Event::Type::KEYBOARD:
		ProcessKeyboardEvent(static_cast<const KeyboardEvent &>(e));
		break;
	case Event::Type::WINDOW_RESIZE:
		ProcessWindowResizeEvent(static_cast<const WindowResizeEvent &>(e));
		break;
	default: break;
	}
}

