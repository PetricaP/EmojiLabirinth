#pragma once

#include "Utility.h"
#include "EventHandler.h"
#include "InputControl.h"
#include <vector>
#include <unordered_map>

struct InputPair {
	float weight;
	InputControl *inputControl;

	InputPair() : inputControl(nullptr) { weight = 0.0f; }

	InputPair(float weight, InputControl *inputControl)
		: weight(weight), inputControl(inputControl) {}
};

class GameEventHandler : public EventHandler {
  private:
	std::unordered_map<KeyboardEvent::Key, std::vector<InputPair>> m_KeyboardInputs;
	std::vector<std::vector<InputPair>> m_MouseInputs;

  public:
	GameEventHandler();
	~GameEventHandler() = default;

	void OnKeyDown(KeyboardEvent::Key key, uint32_t repeat) override;
	void OnKeyUp(KeyboardEvent::Key key, uint32_t repeat) override;
	void OnMouseDown(MouseButtonEvent::Button button,
					 unsigned char numClicks) override;
	void OnMouseUp(MouseButtonEvent::Button button,
				   unsigned char numClicks) override;
	void OnMouseMove(DirectX::XMINT2 position, DirectX::XMINT2 deltaPos) override;

	void AddKeyControl(KeyboardEvent::Key key, InputControl &inputControl,
					   float weight = 1.0f);

	void AddMouseButtonControl(MouseButtonEvent::Button key,
							   InputControl &inputControl, float weight = 1.0f);

  private:
	void UpdateKeyboardInput(KeyboardEvent::Key key, float direction,
							 uint32_t repeat);

	GameEventHandler(const GameEventHandler &other) = delete;
	GameEventHandler &operator=(const GameEventHandler &other) = delete;
};
