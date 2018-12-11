#include "Game.h"
#include "UtilComponents.h"
#include <array>

/* Entry point */
Application *create_application() {
	return new Game;
}

Game::Game() : m_Window(TITLE, INITIAL_WIDTH, INITIAL_HEIGHT),
	m_Renderer(m_Window), m_RenderContext(m_Renderer) {}

int Game::Run() {
	Init();
	m_Timer.Reset();
	while (!m_Window.ShouldClose()) {
		m_Timer.Tick();
		m_Window.PollEvents();
		Game::Update(m_Timer.DeltaTime());
		Game::Render();
	}
	return 0;
}

void Game::Init() {
	InitSettings();

	m_EmojiTexture = m_Renderer.CreateTexture("emoji.dds");
	m_BricksTexture = m_Renderer.CreateTexture("bricks.dds");

	bricks = &m_ECS.AddEntity();

	bricks->AddComponent<CTransform>();
	bricks->GetComponent<CTransform>().SetTranslation({0.0f, -0.3f, 1.0f});
	bricks->GetComponent<CTransform>().SetScale({0.1f, 0.1f, 1.0f});
	bricks->GetComponent<CTransform>().SetRotation({0.0f, 0.0f, 180.0f});

	bricks->AddComponent<CSprite>(m_RenderContext, m_BricksTexture);

	bricks->AddComponent<CMovementControl>();

	bricks->GetComponent<CMovementControl>().controls.push_back(
		std::make_pair(DirectX::XMFLOAT3{1, 0, 0}, &m_Horizontal));

	bricks->GetComponent<CMovementControl>().controls.push_back(
		std::make_pair(DirectX::XMFLOAT3{0, 1, 0}, &m_Vertical));

	emoji = &m_ECS.AddEntity();

	emoji->AddComponent<CTransform>();
	emoji->GetComponent<CTransform>().SetScale({0.1f, 0.1f, 1.0f});
	emoji->GetComponent<CTransform>().SetRotation({0.0f, 0.0f, 180.0f});

	emoji->AddComponent<CSprite>(m_RenderContext, m_EmojiTexture);
}

void Game::Update(float deltaTime) {
	static auto c{0.0f};

	emoji->GetComponent<CTransform>().SetTranslation({sinf(c), cosf(c), 0.0f});

	m_ECS.Refresh();
	m_ECS.Update(deltaTime);

	c += 2.0f * deltaTime;
}

void Game::Render() {
	m_Renderer.Clear();

	m_ECS.Render();

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
	if(event.GetPressed() == true) {
		m_EventHandler.OnKeyDown(event.GetKey(), 0);
	} else {
		m_EventHandler.OnKeyUp(event.GetKey(), 0);
	}
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
	m_RenderContext.SetProjection(DirectX::XMMatrixScaling(m_Window.GetAspectRatio(), 1.0f, 1.0f));
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

void Game::InitSettings() {
	d3d11::Font::Init(m_Window.GetWidth(), m_Window.GetHeight());
	m_Window.AddEventListener(this);

	m_Renderer.SetClearColor(color::CHERRY);
	m_Renderer.EnableVSync(true);

	m_RenderContext.SetProjection(DirectX::XMMatrixScaling(m_Window.GetAspectRatio(), 1.0f, 1.0f));

	m_Font = m_Renderer.CreateFont("Arial");

	m_EventHandler.AddKeyControl(KeyboardEvent::Key::RIGHT, m_Horizontal, 1.0f);
	m_EventHandler.AddKeyControl(KeyboardEvent::Key::LEFT, m_Horizontal, -1.0f);
	m_EventHandler.AddKeyControl(KeyboardEvent::Key::UP, m_Vertical, 1.0f);
	m_EventHandler.AddKeyControl(KeyboardEvent::Key::DOWN, m_Vertical, -1.0f);
}

