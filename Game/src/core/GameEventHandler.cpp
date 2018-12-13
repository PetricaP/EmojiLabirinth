#include "GameEventHandler.h"

GameEventHandler::GameEventHandler() : m_MouseInputs(3) {
	m_MouseInputs.resize(3);
}

void GameEventHandler::OnKeyDown(KeyboardEvent::Key key, uint32_t repeat) {
	UpdateKeyboardInput(key, 1, repeat);
}

void GameEventHandler::OnKeyUp(KeyboardEvent::Key key, uint32_t repeat) {
	UpdateKeyboardInput(key, -1, repeat);
}

void GameEventHandler::OnMouseDown(MouseButtonEvent::Button button,
								   unsigned char numClicks) {
	UNUSED(button);
	UNUSED(numClicks);
}

void GameEventHandler::OnMouseUp(MouseButtonEvent::Button button,
								 unsigned char numClicks) {
	UNUSED(button);
	UNUSED(numClicks);
}

void GameEventHandler::OnMouseMove(DirectX::XMINT2 position, DirectX::XMINT2 deltaPos) {
	UNUSED(position);
	UNUSED(deltaPos);
}

void GameEventHandler::AddKeyControl(KeyboardEvent::Key key,
									 InputControl &inputControl, float weight) {
	m_KeyboardInputs[key].push_back({weight, &inputControl});
}

void GameEventHandler::AddMouseButtonControl(MouseButtonEvent::Button button,
											 InputControl &inputControl,
											 float weight) {
	m_MouseInputs[static_cast<uint32_t>(button)].push_back(
		{weight, &inputControl});
}

void GameEventHandler::UpdateKeyboardInput(KeyboardEvent::Key key,
										   float direction, uint32_t repeat) {
	if (repeat > 0) {
		return;
	}
	for (auto &keyboardInput : m_KeyboardInputs[key]) {
		keyboardInput.inputControl->AddAmount(keyboardInput.weight * direction);
	}
}
