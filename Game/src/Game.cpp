#include "Game.h"
#include "UtilComponents.h"
#include <array>

/* Entry point */
Application *create_application() {
	return new Game;
}

Game::Game() : m_Window(TITLE, INITIAL_WIDTH, INITIAL_HEIGHT),
	m_Renderer(m_Window), m_SimpleShader(m_Renderer), emoji(m_Manager.AddEntity()) {
	m_Texture = m_Renderer.CreateTexture("emoji.dds");

	emoji.AddComponent<CTransform>();
	emoji.AddComponent<CSprite>(m_Renderer, m_Texture);
}

int Game::Run() {
	Init();
	m_Timer.Reset();
	while (!m_Window.ShouldClose()) {
		m_Timer.Tick();
		m_Window.PollEvents();
		Game::Update();
		Game::Render();
	}
	return 0;
}

void Game::Init() {
	/* Should this be done by the user? */
	d3d11::Font::Init(m_Window.GetWidth(), m_Window.GetHeight());
	m_Window.AddEventListener(this);
	std::array<float, 4>color{0.5f, 0.2f, 0.2f, 1.0f};
	m_Renderer.SetClearColor(color.data());

	m_MBuffer.projection = DirectX::XMMatrixScaling(m_Window.GetAspectRatio(),
													1.0f, 1.0f);
	m_Font = m_Renderer.CreateFont("Arial");

	emoji.GetComponent<CTransform>().SetScale(DirectX::XMFLOAT3(0.1f, 0.1f, 1.0f));
	emoji.GetComponent<CTransform>().SetRotation(DirectX::XMFLOAT3(0.0f, 0.0f, 180.0f));

	m_Renderer.SetShader(m_SimpleShader);
}

void Game::Update() {
	static auto c{0.0f};

	emoji.GetComponent<CTransform>().SetPosition(DirectX::XMFLOAT3(sinf(c), cosf(c), 0.0f));

	/* Basic user should not care */
	m_MBuffer.model =  emoji.GetComponent<CTransform>().GetModel();
	m_Manager.Update(m_Timer.DeltaTime());

	std::stringstream ss;
	ss << "X: " << emoji.GetComponent<CTransform>().GetPosition().x << '\n';
	DEBUG_LOG(ss.str().c_str());

	c += 2.0f * m_Timer.DeltaTime();
}

void Game::Render() {
	m_Renderer.Clear();

	/* User should not see */
	m_SimpleShader.UpdateBuffer(m_MBuffer);

	m_Renderer.Submit(emoji.GetComponent<CSprite>());

	/*
	const DirectX::XMFLOAT3 &position = m_Transform.GetPosition();
	m_Transform.SetPosition({-position.x, -position.y, position.z});
	m_MBuffer.model  = m_Transform.GetModel();

	m_SimpleShader.UpdateBuffer(m_MBuffer);

	m_Renderer.Submit(*m_EmojiSprite);
	*/


	m_Renderer.RenderText(m_Font, "Hello DirectX!", -0.3f, -0.2f, 0.1f, 
						  d3d11::Font::Color::ORANGE);

	m_Renderer.Flush();
}

void Game::ProcessMouseButtonEvent(const MouseButtonEvent &event) {
	UNUSED(event);
	DEBUG_LOG("The mouse has been pressed.\n");
	std::stringstream ss;
	ss << "Frame Time: " << m_Timer.DeltaTime() << std::endl;
	ss << "Total Time: " << m_Timer.TotalTime() << std::endl;
	DEBUG_LOG(ss.str().c_str());
	DEBUG_LOG("\n");
}

void Game::ProcessKeyboardEvent(const KeyboardEvent &event) {
	UNUSED(event);
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

	/* This should be managed by the camera class */
	m_MBuffer.projection = DirectX::XMMatrixScaling(m_Window.GetAspectRatio(), 1.0f, 1.0f);
}

void Game::OnEvent(const Event &event) {
	switch(event.GetType()) { 
	case Event::Type::MOUSE_BUTTON:
		ProcessMouseButtonEvent(static_cast<const MouseButtonEvent &>(event));
		break;
	case Event::Type::KEYBOARD:
		ProcessKeyboardEvent(static_cast<const KeyboardEvent &>(event));
		break;
	case Event::Type::WINDOW_RESIZE:
		ProcessWindowResizeEvent(static_cast<const WindowResizeEvent &>(event));
		break;
	default: break;
	}
}

