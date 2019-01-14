#pragma once

#include "Application.h"
#include "Timer.h"
#include "WindowWIN32.h"
#include "GameEventHandler.h"
#include "Camera2D.h"
#include "RenderParams.h"
#include "InteractionSystem.h"
#include "Tile.h"


class Game : public Application {
	enum class State { MENU, WON, PLAY, LOST, PAUSE };
	enum Map : uint32_t { MAP_ONE, MAP_TWO, MAP_THREE, MAP_FOUR, MAP_FIVE };

	static constexpr const char *m_Maps[]{
		"res/maps/map1.txt",
		"res/maps/map2.txt",
		"res/maps/map3.txt",
		"res/maps/map4.txt",
		"res/maps/map5.txt"
	};

	static constexpr size_t nr_maps = sizeof(m_Maps) / sizeof(m_Maps[0]) ;

	static constexpr uint32_t INITIAL_WIDTH{1280u};
	static constexpr uint32_t INITIAL_HEIGHT{720u};
	static constexpr const char *TITLE{"Hello DirectX!"};

	win32::Window m_Window;
	win32::Timer m_Timer;
	d3d11::Renderer m_Renderer;

	RenderParams m_RenderParams;

	Camera2D m_Camera;

	d3d11::Font m_Font;

	d3d11::Texture m_PlayerTexture;
	d3d11::Texture m_BricksTexture;
	d3d11::Texture m_HeartTexture;
	d3d11::Texture m_LikeTexture;
	d3d11::Texture m_EnemyTexture;

	GameEventHandler m_EventHandler;
	InputControl m_Horizontal;
	InputControl m_Vertical;

	ecs::Entity *m_Player;

	ecs::Manager m_ECS;

	InteractionSystem m_InteractionSystem;

	uint32_t m_Hearts;
	uint32_t m_Likes;
	uint32_t m_TotalHearts;
	uint32_t m_TotalLikes;
	float m_MinX;
	float m_MinY;
	float m_MaxX;
	float m_MaxY;

	uint32_t m_EndTime;
	State m_State;
	uint32_t m_CurrentMap;
	bool m_ShouldClose;

	static constexpr size_t MAX_TILES = 40;
	std::vector<std::vector<Tile>> map;
	std::vector<Node> nodes;


  public:
	Game();
	int Run() override;

  private:
	void Init();
	void Update(float deltaTime);
	void Render();

	void OnEvent(const Event &event) override;
	void ProcessMouseButtonEvent(const MouseButtonEvent &event);
	void ProcessKeyboardEvent(const KeyboardEvent &event);
	void ProcessWindowResizeEvent(const WindowResizeEvent &event);
	void InitSettings();

	bool LoadMap(const std::string &path);

	ecs::Entity *CreatePlayer(const d3d11::Texture &texture,
		math::vec2f position, math::vec2f extents);
	ecs::Entity *CreateEnemy(const d3d11::Texture &texture,
		Node &node, math::vec2f extents);
};

