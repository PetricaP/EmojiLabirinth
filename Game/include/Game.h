#pragma once

#include "Application.h"
#include "Timer.h"
#include "WindowWIN32.h"
#include "GameEventHandler.h"
#include "Camera2D.h"
#include "RenderParams.h"
#include "InteractionSystem.h"

class Game : public Application {
	enum class State { END, PLAY };
  private:
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
	d3d11::Texture m_CheeseTexture;

	GameEventHandler m_EventHandler;
	InputControl m_Horizontal;
	InputControl m_Vertical;

	ecs::Entity *m_Player;

	ecs::Manager m_ECS;

	InteractionSystem m_InteractionSystem;

	uint32_t m_Cheeses;
	uint32_t m_TotalCheeses;
	float m_MinX;
	float m_MinY;
	float m_MaxX;
	float m_MaxY;

	uint32_t m_EndTime;
	State m_State;

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

	void LoadMap(const std::string &path);

	ecs::Entity *CreatePlayer(const d3d11::Texture &texture,
		math::vec2f position, math::vec2f extents);
};

