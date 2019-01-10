#include "Game.h"
#include "UtilComponents.h"
#include "AABB2D.h"
#include <iomanip>
#include <fstream>

static constexpr ecs::Group GROUP_WALL = 0;
static constexpr ecs::Group GROUP_CHEESE = 1;

/* Entry point */
std::unique_ptr<Application> create_application() {
	return std::make_unique<Game>();
}

Game::Game() : m_Window(TITLE, INITIAL_WIDTH, INITIAL_HEIGHT),
	m_Renderer(m_Window), m_RenderParams(m_Renderer),
	m_Camera(m_Window.GetAspectRatio()), m_InteractionSystem(m_ECS), m_Cheeses(0),
	m_MinX(0.0f), m_MinY(0.0f), m_State(State::PLAY), m_Player(nullptr) {}

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

static void solve_wall_collission(ecs::Entity *entity, const CBoxCollider2D &wall) {
	auto &transform = entity->GetComponent<CTransform2D>();

	auto &motionComponent = entity->GetComponent<CMotionComponent2D>();

	const auto &aabb = entity->GetComponent<CBoxCollider2D>().GetAABB();
	const auto &aabbWall = wall.GetAABB();

	auto halfSize = (aabb.GetMaxExtents() - aabb.GetMinExtents()) / 2;
	auto wallHalfSize = (aabbWall.GetMaxExtents() - aabbWall.GetMinExtents()) / 2;

	auto center = aabb.GetTranslation();
	auto centerWall = aabbWall.GetTranslation();

	auto delta = centerWall - center;
	std::stringstream ss;
	ss << delta.x << " " << delta.y << std::endl;
	DEBUG_LOG(ss.str().c_str());

	auto intersect = math::vec2f(abs(delta.x), abs(delta.y)) - wallHalfSize - halfSize;
	if(intersect.x > intersect.y) {
		if(delta.x > 0.0f) {
			transform.Move({intersect.x, 0.0f});
		} else {
			transform.Move({-intersect.x, 0.0f});
		}
		motionComponent.SetVelocity({0, motionComponent.GetVelocity().y});
	} else {
		if(delta.y > 0.0f) {
			transform.Move({0.0f, intersect.y});
		} else {
			transform.Move({0.0f, -intersect.y});
		}
		motionComponent.SetVelocity({motionComponent.GetVelocity().x, 0});
	}
}

static ecs::Entity *create_wall(ecs::Manager &ecs, RenderParams &params,
	const d3d11::Texture &texture, math::vec2f position, math::vec2f extents) {

	auto wall = &ecs.AddEntity();

	wall->AddComponent<CTransform2D>();
	wall->GetComponent<CTransform2D>().SetScale(extents);
	wall->GetComponent<CTransform2D>().SetTranslation(position);
	wall->AddGroup(GROUP_WALL);

	wall->AddComponent<CBoxCollider2D>(
		math::vec2f{-extents.x, -extents.y},
		math::vec2f{extents.x, extents.y});

	wall->AddComponent<CSprite>(params, texture);
	return wall;
}

static ecs::Entity *create_cheese(ecs::Manager &ecs, RenderParams &params,
	const d3d11::Texture &texture, math::vec2f position, math::vec2f extents) {

	auto wall = &ecs.AddEntity();

	wall->AddComponent<CTransform2D>();
	wall->GetComponent<CTransform2D>().SetScale(extents);
	wall->GetComponent<CTransform2D>().SetTranslation(position);
	wall->AddGroup(GROUP_CHEESE);

	wall->AddComponent<CBoxCollider2D>(
		math::vec2f{-extents.x, -extents.y},
		math::vec2f{extents.x, extents.y});

	wall->AddComponent<CSprite>(params, texture);
	return wall;
}

ecs::Entity *Game::CreatePlayer(const d3d11::Texture &texture, math::vec2f position, math::vec2f extents) {
	auto player = &m_ECS.AddEntity();

	player->AddComponent<CTransform2D>();
	player->GetComponent<CTransform2D>().SetTranslation(position);
	player->GetComponent<CTransform2D>().SetScale(extents);

	player->AddComponent<CSprite>(m_RenderParams, texture);

	player->AddComponent<CCamera2D>(m_RenderParams, m_Camera,
								  math::vec2f{0.0f, 0.0f});

	player->AddComponent<CMotionComponent2D>();

	player->AddComponent<CBoxCollider2D>(
		math::vec2f{-0.08f, -0.08f},
		math::vec2f{ 0.08f,  0.08f},
		[player, this] (const CBoxCollider2D &other, const IntersectionData &data) {
			if (other.entity->HasGroup(GROUP_WALL)) {
				solve_wall_collission(player, other);
			}
			if (other.entity->HasGroup(GROUP_CHEESE)) {
				other.entity->Destroy();
				++m_Cheeses;
			}
		}
	);

	player->AddComponent<CMovementControl>();

	const float amount = 5.0f;
	player->GetComponent<CMovementControl>().controls.push_back(
		{math::vec2f{amount, 0}, &m_Horizontal});

	player->GetComponent<CMovementControl>().controls.push_back(
		{math::vec2f{0, amount}, &m_Vertical});


	return player;
}

void Game::Init() {
	AABB2D::Test();
	InitSettings();

	m_PlayerTexture = m_Renderer.CreateTexture("mouse.dds");
	m_BricksTexture = m_Renderer.CreateTexture("bricks.dds");
	m_CheeseTexture = m_Renderer.CreateTexture("cheese.dds");

	LoadMap("map4.txt");
}

void Game::LoadMap(const std::string &path) {
	std::ifstream mapFile;
	mapFile.open(path.c_str());
	ASSERT(mapFile.is_open());
	uint32_t line = 0;
	uint32_t colon = 0;
	m_TotalCheeses = 0;
	uint32_t maxColon = colon;
	static constexpr float WALL_WIDTH = 0.14f;
	static constexpr float WALL_HEIGHT = 0.02f;
	while(!mapFile.eof()) {
		char c = mapFile.get();
		switch(c) {
		case '_':
			if(colon % 2 == 1) {
				create_wall(m_ECS, m_RenderParams, m_BricksTexture,
							{(colon) * WALL_WIDTH, line * WALL_WIDTH * 2}, {WALL_WIDTH, WALL_HEIGHT});
			}
			++colon;
			break;
		case '|':
			create_wall(m_ECS, m_RenderParams, m_BricksTexture,
						{colon * WALL_WIDTH, line * WALL_WIDTH * 2 - WALL_WIDTH}, {WALL_HEIGHT, WALL_WIDTH});
			++colon;
			break;
		case 'p':
			m_Player = CreatePlayer(m_PlayerTexture,{(colon) * WALL_WIDTH + WALL_WIDTH / 2.0f, line * WALL_WIDTH + WALL_WIDTH / 2.0f}, {0.07f, 0.05f});
			++colon;
			break;
		case 'c':
			create_cheese(m_ECS, m_RenderParams, m_CheeseTexture,
				{colon * WALL_WIDTH, line * WALL_WIDTH * 2 - WALL_WIDTH},
				{0.045f, 0.06f});
			++m_TotalCheeses;
			++colon;
			break;
		case '\n':
			colon = 0;
			++line;
			break;
		default:
			++colon;
			break;
		}
		if(colon > maxColon) {
			maxColon = colon;
		}
	}
	m_MaxX = WALL_WIDTH * (maxColon + 1);
	m_MaxY = WALL_WIDTH * 2 * (line + 1);
}

void Game::Update(float deltaTime) {
	if(m_Player) {
		auto transform = m_Player->GetComponent<CTransform2D>();
		auto x = transform.GetTranslation().x;
		auto y = transform.GetTranslation().y;
		if(m_State != State::END && (x < m_MinX || x > m_MaxX || y < m_MinY || y > m_MaxY)) {
			m_State = State::END;
			m_EndTime = static_cast<uint32_t>(m_Timer.TotalTime());
		}
	}
	m_ECS.Refresh();
	m_ECS.Update(deltaTime);
	m_InteractionSystem.Update();
}

void Game::Render() {
	m_Renderer.Clear();

	m_ECS.Render();

	std::stringstream ss;
	if(m_State == State::PLAY) {
	#if defined(DEBUG) || defined(_DEBUG)
		ss << "Frame Time: " << std::setprecision(2) << m_Timer.DeltaTime() << std::endl;
	#endif
		ss << "Time: " << static_cast<uint32_t>(m_Timer.TotalTime());
		m_Renderer.RenderText(m_Font, ss.str(), -0.48f, -0.48f, 0.02f, 
							  d3d11::Font::Color::GRAY);
		ss.str(std::string());
		ss << "Cheese: " << m_Cheeses;
		m_Renderer.RenderText(m_Font, ss.str(), -0.48f, -0.38f, 0.02f, 
							  d3d11::Font::Color::YELLOW);
	} else if(m_State == State::END) {
		ss << "You finished the game in " << m_EndTime << " seconds";
		m_Renderer.RenderText(m_Font, ss.str(), -0.15f, 0.16f, 0.02f, 
							  d3d11::Font::Color::GRAY);
		ss.str(std::string());
		ss << "\nYou collected " << m_Cheeses << " out of " << m_TotalCheeses << " pieces of cheese.";
		m_Renderer.RenderText(m_Font, ss.str(), -0.17f, 0.2f, 0.02f, 
							  d3d11::Font::Color::YELLOW);
	}
	m_Renderer.Flush();
}

void Game::ProcessMouseButtonEvent(const MouseButtonEvent &event) {
	UNUSED(event);
	DEBUG_LOG("The mouse has been pressed.\n");
}

void Game::ProcessKeyboardEvent(const KeyboardEvent &event) {
	if(event.GetPressed() == true) {
		m_EventHandler.OnKeyDown(event.GetKey(), 0);
		if(event.GetKey() == KeyboardEvent::Key::A || event.GetKey() == KeyboardEvent::Key::LEFT) {
			m_Player->GetComponent<CSprite>().SetRect({0.0f, 0.0f}, {1.0f, 1.0f});
		} else if(event.GetKey() == KeyboardEvent::Key::D || event.GetKey() == KeyboardEvent::Key::RIGHT) {
			m_Player->GetComponent<CSprite>().SetRect({1.0f, 0.0f}, {0.0f, 1.0f});
		}
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

	m_Camera.SetAspectRatio(m_Window.GetAspectRatio());
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
	srand(static_cast<uint32_t>(time(nullptr)));
	d3d11::Font::Init(m_Window.GetWidth(), m_Window.GetHeight());
	m_Window.AddEventListener(this);

	m_Renderer.SetClearColor({color::BLACK});
	m_Renderer.EnableVSync(true);

	m_RenderParams.SetProjection(m_Camera.GetProjectionMatrix());
	m_RenderParams.SetView(m_Camera.GetViewMatrix());

	m_Font = m_Renderer.CreateFont("Arial");

	m_EventHandler.AddKeyControl(KeyboardEvent::Key::D, m_Horizontal, 1.0f);
	m_EventHandler.AddKeyControl(KeyboardEvent::Key::A, m_Horizontal, -1.0f);
	m_EventHandler.AddKeyControl(KeyboardEvent::Key::W, m_Vertical, 1.0f);
	m_EventHandler.AddKeyControl(KeyboardEvent::Key::S, m_Vertical, -1.0f);
	m_EventHandler.AddKeyControl(KeyboardEvent::Key::RIGHT, m_Horizontal, 1.0f);
	m_EventHandler.AddKeyControl(KeyboardEvent::Key::LEFT, m_Horizontal, -1.0f);
	m_EventHandler.AddKeyControl(KeyboardEvent::Key::UP, m_Vertical, 1.0f);
	m_EventHandler.AddKeyControl(KeyboardEvent::Key::DOWN, m_Vertical, -1.0f);
}

