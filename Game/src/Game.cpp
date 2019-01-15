#include "Game.h"
#include "UtilComponents.h"
#include "GameComponents.h"
#include "AABB2D.h"
#include <iomanip>
#include <fstream>
#include <ctime>

static constexpr ecs::Group GROUP_WALL = 0;
static constexpr ecs::Group GROUP_HEARTS = 1;
static constexpr ecs::Group GROUP_ENEMY = 2;
static constexpr ecs::Group GROUP_LIKES = 3;

/* Entry point */
std::unique_ptr<Application> create_application() {
	return std::make_unique<Game>();
}

Game::Game() : m_Window(TITLE, INITIAL_WIDTH, INITIAL_HEIGHT),
	m_Renderer(m_Window), m_RenderParams(m_Renderer),
	m_Camera(m_Window.GetAspectRatio()), m_InteractionSystem(m_ECS),
	m_Hearts(0), m_State(State::MENU), m_Player(nullptr), map(MAX_TILES),
	m_CurrentMap(Map::MAP_FOUR), m_ShouldClose(false) {}

int Game::Run() {
	Init();
	m_Timer.Reset();
	while (!m_ShouldClose) {
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
		motionComponent.SetVelocity( {motionComponent.GetVelocity().x, 0});
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

static ecs::Entity *create_collectable(ecs::Group group, ecs::Manager &ecs, RenderParams &params,
	const d3d11::Texture &texture, math::vec2f position, math::vec2f extents) {

	auto wall = &ecs.AddEntity();

	wall->AddComponent<CTransform2D>();
	wall->GetComponent<CTransform2D>().SetScale(extents);
	wall->GetComponent<CTransform2D>().SetTranslation(position);
	wall->AddGroup(group);

	wall->AddComponent<CBoxCollider2D>(
		math::vec2f{-extents.x, -extents.y},
		math::vec2f{extents.x, extents.y});

	wall->AddComponent<CSprite>(params, texture);
	return wall;
}

ecs::Entity *Game::CreatePlayer(const d3d11::Texture &texture, math::vec2f position,
								math::vec2f extents) {
	auto player = &m_ECS.AddEntity();

	player->AddComponent<CTransform2D>();
	player->GetComponent<CTransform2D>().SetTranslation(position);
	player->GetComponent<CTransform2D>().SetScale(extents);

	player->AddComponent<CSprite>(m_RenderParams, texture);

	player->AddComponent<CCamera2D>(m_RenderParams, m_Camera,
								  math::vec2f{0.0f, 0.0f});

	player->AddComponent<CMotionComponent2D>();

	player->AddComponent<CBoxCollider2D>(
		math::vec2f{-extents.x, -extents.y},
		math::vec2f{ extents.x,  extents.y},
		[player, this] (const CBoxCollider2D &other, const IntersectionData &data) {
			if (other.entity->HasGroup(GROUP_WALL)) {
				solve_wall_collission(player, other);
			}
			if (other.entity->HasGroup(GROUP_HEARTS)) {
				other.entity->Destroy();
				++m_Hearts;
			}
			if (other.entity->HasGroup(GROUP_LIKES)) {
				other.entity->Destroy();
				++m_Likes;
			}
			if (other.entity->HasGroup(GROUP_ENEMY)) {
				other.entity->Destroy();
				m_State = State::LOST;
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

ecs::Entity *Game::CreateEnemy(const d3d11::Texture &texture, Node &node,
							   math::vec2f extents) {
	auto enemy = &m_ECS.AddEntity();

	enemy->AddComponent<CTransform2D>();
	enemy->GetComponent<CTransform2D>().SetTranslation(node.GetTile()->GetPosition());
	enemy->GetComponent<CTransform2D>().SetScale(extents);

	enemy->AddComponent<CSprite>(m_RenderParams, texture);

	enemy->AddGroup(GROUP_ENEMY);

	enemy->AddComponent<CMotionComponent2D>();

	enemy->AddComponent<CBoxCollider2D>(
		math::vec2f{-extents.x, -extents.y},
		math::vec2f{ extents.x,  extents.y});

	enemy->AddComponent<CEnemyControl>(node);

	return enemy;
}

void Game::Init() {
	AABB2D::Test();
	InitSettings();

	m_PlayerTexture = m_Renderer.CreateTexture("res/textures/emoji.dds");
	m_BricksTexture = m_Renderer.CreateTexture("res/textures/bricks.dds");
	m_HeartTexture = m_Renderer.CreateTexture("res/textures/heart.dds");
	m_LikeTexture = m_Renderer.CreateTexture("res/textures/like.dds");
	m_EnemyTexture = m_Renderer.CreateTexture("res/textures/devil.dds");
}

std::vector<Node> create_nodes(std::vector<std::vector<Tile>> &map, Tile *node_tile);

bool Game::LoadMap(const std::string &path) {
	map.clear();
	nodes.clear();
	map.resize(MAX_TILES);
	for(auto &v : map) {
		v.resize(MAX_TILES);
	}

	std::ifstream mapFile;
	mapFile.open(path.c_str());
	if(!mapFile.is_open()) {
		return false;
	}
	int32_t line = 0;
	int32_t column = 0;
	m_TotalHearts = 0;
	m_TotalLikes = 0;
	int32_t maxColon = column;
	static constexpr float WALL_WIDTH = 0.14f;
	static constexpr float WALL_HEIGHT = 0.02f;
	std::vector<std::pair<uint32_t, uint32_t>> enemyPositions;
	
	while(!mapFile.eof()) {
		char c = mapFile.get();
		map[line][column / 2].row = line;
		map[line][column / 2].column = column / 2;
		map[line][column / 2].tileWidth = WALL_WIDTH * 2;
		switch(c) {
		case '_':
			if(column % 2 == 1) {
				create_wall(m_ECS, m_RenderParams, m_BricksTexture,
							{(column) * WALL_WIDTH, -line * WALL_WIDTH * 2},
							{WALL_WIDTH, WALL_HEIGHT});
				map[line][column / 2].down = true;
				map[line + 1][column / 2].up = true;
			}
			++column;
			break;
		case '|':
			create_wall(m_ECS, m_RenderParams, m_BricksTexture,
						{column * WALL_WIDTH, -(line * WALL_WIDTH * 2 - WALL_WIDTH)},
						{WALL_HEIGHT, WALL_WIDTH});
			map[line][column / 2].left = true;
			if(column > 0) {
				map[line][column / 2 - 1].right = true;
			}
			++column;
			break;
		case 'p':
			m_Player = CreatePlayer(m_PlayerTexture, 
				{(column) * WALL_WIDTH + WALL_WIDTH / 2.0f,
				-(line * WALL_WIDTH + WALL_WIDTH / 2.0f)},
				{0.07f, 0.07f});
			++column;
			break;
		case 'e':
			enemyPositions.push_back(std::make_pair(line, column / 2));
			++column;
			break;
		case 'c':
		case 'h':
			create_collectable(GROUP_HEARTS, m_ECS, m_RenderParams, m_HeartTexture,
				{column * WALL_WIDTH, -(line * WALL_WIDTH * 2 - WALL_WIDTH)},
				{0.07f, 0.07f});
			++m_TotalHearts;
			++column;
			break;
		case 'l':
			create_collectable(GROUP_LIKES, m_ECS, m_RenderParams, m_LikeTexture,
				{column * WALL_WIDTH, -(line * WALL_WIDTH * 2 - WALL_WIDTH)},
				{0.07f, 0.07f});
			++m_TotalLikes;
			++column;
			break;
		case '\n':
			column = 0;
			++line;
			break;
		default:
			++column;
			break;
		}
		if(column > maxColon) {
			maxColon = column;
		}
	}
	m_MaxY = 0;
	m_MinX = 0;
	m_MaxX = WALL_WIDTH * (maxColon + 1);
	m_MinY = -(WALL_WIDTH * 2 * line);

	map.resize(line);
	for(auto &v : map) {
		v.resize(maxColon / 2);
	}

	std::stringstream ss;
	for(uint32_t i = 0; i < map.size(); ++i) {
		for(uint32_t j = 0; j < map[i].size(); ++j) {
			auto &t = map[i][j];
			ss << "{(r: " << t.row << ") (c: " << t.column <<
				") (u: " << t.up << ") (d: " << t.down <<
				") (l: " << t.left << ") (r: " << t.right << ")} ";
			if(i > 0) {
				if(!((t.up && t.down && !(t.left || t.right)) ||
					(t.left && t.right && !(t.up || t.down)))) {
					t.is_node = true;
				} else {
					t.is_node = false;
				}
			}
		}
		ss << std::endl;
	}

	DEBUG_LOG("Loaded map\n");
	DEBUG_LOG(ss.str().c_str());

	Tile *first_node_tile = nullptr;
	for(uint32_t i = 0; i < map.size(); ++i) {
		for(uint32_t j = 0; j < map[i].size(); ++j) {
			if(map[i][j].is_node) {
				first_node_tile = &map[i][j];
				break;
			}
		}
		if(first_node_tile) {
			break;
		}
	}
	ASSERT(first_node_tile);

	nodes = create_nodes(map, first_node_tile);
	for(auto &node : nodes) {
		node.GetTile()->node = &node;
	}

	for(auto &enemyPosition : enemyPositions) {
		for(auto &node : nodes) {
			if(enemyPosition.first == node.GetTile()->row &&
				enemyPosition.second == node.GetTile()->column) {
				CreateEnemy(m_EnemyTexture, node, {0.07f, 0.07f});
			}
		}
	}

	ss.str(std::string());
	ss << "NODES:\n";
	for(auto node : nodes) {
		ss << node << std::endl;
	}
	DEBUG_LOG(ss.str().c_str());
	return true;
}

void Game::Update(float deltaTime) {
	if(!m_ShouldClose) {
		m_ShouldClose = m_Window.ShouldClose();
	}

	if(m_State != State::LOST && m_State != State::MENU) {
		if(m_Player) {
			auto transform = m_Player->GetComponent<CTransform2D>();
			auto x = transform.GetTranslation().x;
			auto y = transform.GetTranslation().y;
			if(m_State != State::WON && (x < m_MinX || x > m_MaxX
									  || y < m_MinY || y > m_MaxY)) {
				m_State = State::WON;
				m_EndTime = static_cast<uint32_t>(m_Timer.TotalTime());
			}
		}
		m_ECS.Refresh();
		m_ECS.Update(deltaTime);
		m_InteractionSystem.Update();
	}
}

void Game::Render() {
	m_Renderer.Clear();

	if(m_State != State::MENU) {
		m_ECS.Render();
	}

	std::stringstream ss;
	switch(m_State) {
	case State::PAUSE:
		ss.str(std::string());
		ss << "R - Resume\n";
		ss << "M - Main menu";
		m_Renderer.RenderText(m_Font, ss.str(), -0.08f, -0.28f, 0.03f, 
							  d3d11::Font::Color::LIGHT_BLUE);
		break;
	case State::MENU:
		ss << "EMOJI LABIRINTH";
		m_Renderer.RenderText(m_Font, ss.str(), -0.18f, -0.28f, 0.04f, 
							  d3d11::Font::Color::LIGHT_BLUE);
		d3d11::Font::Color color;
		for(uint32_t i = 0; i < nr_maps; ++i) {
			ss.str(std::string());
			auto map = m_Maps[i];
			std::string map_string(map);
			map_string = map_string.substr(9, map_string.end() - map_string.begin());
			map_string = map_string.substr(0, map_string.end() - map_string.begin() - 4);
			if(i == m_CurrentMap) {
				color = d3d11::Font::Color::YELLOW;
			} else {
				color = d3d11::Font::Color::GRAY;
			}
			ss << map_string << std::endl;
			m_Renderer.RenderText(m_Font, ss.str(), -0.05f, 0.30f - (nr_maps - i) * 0.07f, 0.03f, 
								  color);
		}
		break;
	case State::PLAY:
	#if defined(DEBUG) || defined(_DEBUG)
		ss << "Frame Time: " << std::setprecision(2) << m_Timer.DeltaTime();
	#endif
		ss << "\nTime: " << static_cast<uint32_t>(m_Timer.TotalTime());
		m_Renderer.RenderText(m_Font, ss.str(), -0.48f, -0.48f, 0.02f, 
							  d3d11::Font::Color::GRAY);
		if(m_TotalHearts) {
			ss.str(std::string());
			ss << "Hearts: " << m_Hearts;
			m_Renderer.RenderText(m_Font, ss.str(), -0.48f, -0.38f, 0.02f, 
								  d3d11::Font::Color::RED);
		}
		if(m_TotalLikes) {
			ss.str(std::string());
			ss << "Likes: " << m_Likes;
			m_Renderer.RenderText(m_Font, ss.str(), -0.48f, -0.32f, 0.02f, 
								  d3d11::Font::Color::LIGHT_BLUE);
		}
		break;
	case State::WON:
		ss << "You finished the game in " << m_EndTime << " seconds";
		m_Renderer.RenderText(m_Font, ss.str(), -0.15f, 0.16f, 0.02f, 
							  d3d11::Font::Color::GRAY);
		if(m_TotalHearts) {
			ss.str(std::string());
			ss << "\nYou collected " << m_Hearts << " out of " << m_TotalHearts << " hearts.";
			m_Renderer.RenderText(m_Font, ss.str(), -0.12f, 0.2f, 0.02f, 
								  d3d11::Font::Color::RED);
		}
		if(m_TotalLikes) {
			ss.str(std::string());
			ss << "\nYou collected " << m_Likes << " out of " << m_TotalLikes << " likes.";
			m_Renderer.RenderText(m_Font, ss.str(), -0.12f, 0.28f, 0.02f, 
								  d3d11::Font::Color::LIGHT_BLUE);
		}
		ss.str(std::string());
		ss << "Press M to go back to main menu.";
		m_Renderer.RenderText(m_Font, ss.str(), -0.14f, 0.40f, 0.02f, 
							  d3d11::Font::Color::GRAY);
		break;
	case State::LOST:
		ss << "You LOST";
		m_Renderer.RenderText(m_Font, ss.str(), -0.05f, 0.16f, 0.02f, 
							  d3d11::Font::Color::GRAY);
		if(m_TotalHearts) {
			ss.str(std::string());
			ss << "\nYou collected " << m_Hearts << " out of " << m_TotalHearts << " hearts.";
			m_Renderer.RenderText(m_Font, ss.str(), -0.12f, 0.2f, 0.02f, 
								  d3d11::Font::Color::RED);
		}
		if(m_TotalLikes) {
			ss.str(std::string());
			ss << "\nYou collected " << m_Likes << " out of " << m_TotalLikes << " likes.";
			m_Renderer.RenderText(m_Font, ss.str(), -0.12f, 0.28f, 0.02f, 
								  d3d11::Font::Color::LIGHT_BLUE);
		}
		ss.str(std::string());
		ss << "Press M to go back to main menu.";
		m_Renderer.RenderText(m_Font, ss.str(), -0.14f, 0.40f, 0.02f, 
							  d3d11::Font::Color::GRAY);
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
		if(m_State == State::MENU) {
			switch(event.GetKey()) {
			case KeyboardEvent::Key::ESCAPE:
				m_ShouldClose = true;
				break;
			case KeyboardEvent::Key::RETURN:
				m_State = State::PLAY;
				m_Timer.Reset();
				m_ECS.Clear();
				m_InteractionSystem.Clear();
				if(LoadMap(m_Maps[m_CurrentMap])) {
					m_Renderer.SetClearColor(color::BLACK);
				} else {
					MessageBox(static_cast<HWND>(m_Window.GetAPIHandle()),
					"The map could not be found!\nThis is a slot reserved for a custom user map.\nYou can find instructions on how to write one in the game directory.", "Error", MB_OK);
					m_State = State::MENU;
				}
				break;
			case KeyboardEvent::Key::UP:
				if(m_CurrentMap > 0) {
					--m_CurrentMap;
				}
				break;
			case KeyboardEvent::Key::DOWN:
				if(m_CurrentMap < nr_maps - 1) {
					++m_CurrentMap;
				}
				break;
			default:
				break;
			}
		} else {
			if(m_State == State::PAUSE) {
				switch(event.GetKey()) {
				case KeyboardEvent::Key::R:
					m_Timer.Start();
					m_State = State::PLAY;
					break;
				case KeyboardEvent::Key::M:
					m_State = State::MENU;
					break;
				}
			}
			if(m_State == State::PLAY) {
				switch(event.GetKey()) {
				case KeyboardEvent::Key::ESCAPE:
					m_Timer.Stop();
					m_State = State::PAUSE;
					break;
				}
			}
			if(m_State != State::LOST) {
				if(event.GetKey() == KeyboardEvent::Key::A || event.GetKey() == KeyboardEvent::Key::LEFT) {
					m_Player->GetComponent<CSprite>().SetRect({1.0f, 0.0f}, {0.0f, 1.0f});
				} else if(event.GetKey() == KeyboardEvent::Key::D || event.GetKey() == KeyboardEvent::Key::RIGHT) {
					m_Player->GetComponent<CSprite>().SetRect({0.0f, 0.0f}, {1.0f, 1.0f});
				}
			}

			if(m_State == State::WON || m_State == State::LOST) {
				switch(event.GetKey()) {
				case KeyboardEvent::Key::M:
					m_State = State::MENU;
					break;
				}
			}
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

	m_Renderer.SetClearColor({color::DARK_GRAY});
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

