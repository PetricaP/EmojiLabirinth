#pragma once

#include "Application.h"
#include "Utility.h"
#include "Drawable.h"
#include "Renderer.h"
#include "Timer.h"
#include "Transform.h"
#include "WindowWIN32.h"
#include "SimpleShader.h"
#include "Sprite.h"

class Game : public Application {
  private:
	win32::Window m_Window;
	win32::Timer m_Timer;
	d3d11::Renderer m_Renderer;

	/* This should not be visible to the user */
	SimpleShader m_SimpleShader;
	SimpleShader::MatrixBuffer m_MBuffer;

	/* This belongs in an entity */
	xmm::Transform m_Transform;
	std::unique_ptr<Sprite> m_EmojiSprite;

	d3d11::Font m_Font;

	d3d11::Texture m_Texture;

  public:
	Game();
	int Run() override;
	void Update();

	void OnEvent(const Event &e) override;

  private:
	void Draw();
	void ProcessMouseButtonEvent(const MouseButtonEvent &event);
	void ProcessKeyboardEvent(const KeyboardEvent &event);
	void ProcessWindowResizeEvent(const WindowResizeEvent &event);
};

