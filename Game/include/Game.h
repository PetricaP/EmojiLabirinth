#pragma once

#include "Application.h"
#include "Timer.h"
#include "Transform.h"
#include "WindowWIN32.h"
#include "GameEventHandler.h"
#include "ECS.h"
#include "Sprite.h"
#include "Camera.h"
#include "RenderContext.h"
#include <memory>

class Game : public Application {
  private:
	static constexpr uint32_t INITIAL_WIDTH{800u};
	static constexpr uint32_t INITIAL_HEIGHT{600u};
	static constexpr const char *TITLE{"Hello DirectX!"};

	win32::Window m_Window;
	win32::Timer m_Timer;
	d3d11::Renderer m_Renderer;

	GameRenderContext m_RenderContext;

	Camera m_Camera;

	d3d11::Font m_Font;

	d3d11::Texture m_EmojiTexture;
	d3d11::Texture m_BricksTexture;

	GameEventHandler m_EventHandler;
	InputControl m_Horizontal;
	InputControl m_Vertical;

	ecs::Manager m_ECS;
	ecs::Entity *bricks;
	ecs::Entity *emoji;

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
