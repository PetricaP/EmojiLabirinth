#pragma once

#include "Application.h"
#include "Timer.h"
#include "WindowWIN32.h"
#include "GameEventHandler.h"
#include "Camera2D.h"
#include "RenderParams.h"
#include "InteractionSystem.h"

class Game : public Application {
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

	d3d11::Texture m_EmojiTexture;
	d3d11::Texture m_BricksTexture;

	GameEventHandler m_EventHandler;
	InputControl m_Horizontal;
	InputControl m_Vertical;

	ecs::Manager m_ECS;

	InteractionSystem m_InteractionSystem;
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
};

