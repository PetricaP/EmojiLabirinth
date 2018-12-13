#include "Game.h"
#include "UtilComponents.h"
#include "AABB2D.h"
#include <iomanip>

/* Entry point */
std::unique_ptr<Application> create_application() {
	return std::make_unique<Game>();
}

Game::Game() : m_Window(TITLE, INITIAL_WIDTH, INITIAL_HEIGHT),
	m_Renderer(m_Window), m_RenderParams(m_Renderer),
	m_Camera(m_Window.GetAspectRatio()) {}

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

void TestAABB2D() {
	AABB2D a1({0.0f, 0.0f}, {1.0f, 1.0f});
	AABB2D a2({0.5f, 0.5f}, {2.0f, 2.0f});
	AABB2D a3({1.5f, 0.0f}, {2.0f, 1.0f});

	IntersectionData data1 = a1.IntersectAABB2D(a2);
	IntersectionData data2 = a2.IntersectAABB2D(a3);
	IntersectionData data3 = a3.IntersectAABB2D(a1);

	DEBUG_LOG("AABB2D Test:\n");
	std::stringstream ss;
	ss << data1.distance << " " << data1.intersects << std::endl;
	ss << data2.distance << " " << data2.intersects << std::endl;
	ss << data3.distance << " " << data3.intersects << std::endl;
	DEBUG_LOG(ss.str().c_str());
}

void Game::Init() {
	InitSettings();
	TestAABB2D();

	m_EmojiTexture = m_Renderer.CreateTexture("emoji.dds");
	m_BricksTexture = m_Renderer.CreateTexture("bricks.dds");

	ecs::Entity *emoji = &m_ECS.AddEntity();

	emoji->AddComponent<CTransform2D>();
	emoji->GetComponent<CTransform2D>().SetTranslation({0.0f, -0.3f});
	emoji->GetComponent<CTransform2D>().SetScale({0.1f, 0.1f});

	emoji->AddComponent<CSprite>(m_RenderParams, m_EmojiTexture);

	emoji->AddComponent<CMovementControl>();

	emoji->GetComponent<CMovementControl>().controls.push_back(
		std::make_pair(DirectX::XMFLOAT2{1, 0}, &m_Horizontal));

	emoji->GetComponent<CMovementControl>().controls.push_back(
		std::make_pair(DirectX::XMFLOAT2{0, 1}, &m_Vertical));

	emoji->AddComponent<CCamera2D>(m_RenderParams, m_Camera,
								  DirectX::XMFLOAT2{0.0f, 0.5f});

	ecs::Entity *bricks = &m_ECS.AddEntity();

	bricks->AddComponent<CTransform2D>();
	bricks->GetComponent<CTransform2D>().SetScale({0.1f, 0.1f});
	bricks->GetComponent<CTransform2D>().SetTranslation({-0.7f, 0.3f});

	bricks->AddComponent<CSprite>(m_RenderParams, m_BricksTexture);

	ecs::Entity *wall1 = &m_ECS.AddEntity();

	wall1->AddComponent<CTransform2D>();
	wall1->GetComponent<CTransform2D>().SetScale({0.6f, 0.02f});
	wall1->GetComponent<CTransform2D>().SetTranslation({0.3f, 0.5f});

	wall1->AddComponent<CSprite>(m_RenderParams, m_BricksTexture);

	ecs::Entity *wall2 = &m_ECS.AddEntity();

	wall2->AddComponent<CTransform2D>();
	wall2->GetComponent<CTransform2D>().SetScale({0.04f, 0.5f});
	wall2->GetComponent<CTransform2D>().SetTranslation({-0.8f, -0.5f});

	wall2->AddComponent<CSprite>(m_RenderParams, m_BricksTexture);
}

void Game::Update(float deltaTime) {
	m_ECS.Refresh();
	m_ECS.Update(deltaTime);
}

void Game::Render() {
	m_Renderer.Clear();

	m_ECS.Render();

	std::stringstream ss;
	ss << "Frame Time: " << std::setprecision(2) << m_Timer.DeltaTime() << std::endl;
	ss << "Total Time: " << std::setprecision(4) << m_Timer.TotalTime();
	m_Renderer.RenderText(m_Font, ss.str(), -0.45f, -0.45f, 0.02f, 
						  d3d11::Font::Color::BLACK);
	m_Renderer.Flush();
}

void Game::ProcessMouseButtonEvent(const MouseButtonEvent &event) {
	UNUSED(event);
	DEBUG_LOG("The mouse has been pressed.\n");
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
	m_RenderParams.SetProjection(
		DirectX::XMMatrixScaling(m_Window.GetAspectRatio(), 1.0f, 1.0f));
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

	m_RenderParams.SetProjection(m_Camera.GetProjectionMatrix());
	m_RenderParams.SetView(m_Camera.GetViewMatrix());

	m_Font = m_Renderer.CreateFont("Arial");

	m_EventHandler.AddKeyControl(KeyboardEvent::Key::RIGHT, m_Horizontal, 1.0f);
	m_EventHandler.AddKeyControl(KeyboardEvent::Key::LEFT, m_Horizontal, -1.0f);
	m_EventHandler.AddKeyControl(KeyboardEvent::Key::UP, m_Vertical, 1.0f);
	m_EventHandler.AddKeyControl(KeyboardEvent::Key::DOWN, m_Vertical, -1.0f);
}

