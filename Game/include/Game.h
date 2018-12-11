#pragma once

#include "Application.h"
#include "Timer.h"
#include "Transform.h"
#include "WindowWIN32.h"
#include "GameEventHandler.h"
#include "ECS.h"
#include "SimpleShader.h"
#include "Sprite.h"
#include <memory>

class Game : public Application {
  private:
	static constexpr uint32_t INITIAL_WIDTH{800u};
	static constexpr uint32_t INITIAL_HEIGHT{600u};
	static constexpr const char *TITLE{"Hello DirectX!"};

	win32::Window m_Window;
	win32::Timer m_Timer;
	d3d11::Renderer m_Renderer;

	d3d11::Font m_Font;

	d3d11::Texture m_Texture;

	/* This should not be visible to the user */
	/* Shader in the Application? */
	SimpleShader m_SimpleShader;
	SimpleShader::MatrixBuffer m_MBuffer;

	GameEventHandler m_EventHandler;
	InputControl m_Horizontal;
	InputControl m_Vertical;

	ecs::Manager m_Manager;
	ecs::Entity *emoji;
	ecs::Entity *emoji2;

  public:
	Game();
	int Run() override;

	void Init();
	void Update();

	void OnEvent(const Event &event) override;

  private:
	void Render();
	void ProcessMouseButtonEvent(const MouseButtonEvent &event);
	void ProcessKeyboardEvent(const KeyboardEvent &event);
	void ProcessWindowResizeEvent(const WindowResizeEvent &event);
};
