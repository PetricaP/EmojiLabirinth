#pragma once

#include "Event.h"
#include "DirectXMath.h"

class EventHandler {
  public:
	EventHandler() = default;
	virtual ~EventHandler() = default;

	virtual void OnKeyDown(KeyboardEvent::Key key, uint32_t isRepeated) = 0;
	virtual void OnKeyUp(KeyboardEvent::Key key, uint32_t isRepeated) = 0;
	virtual void OnMouseDown(MouseButtonEvent::Button button,
							 unsigned char numClicks) = 0;
	virtual void OnMouseUp(MouseButtonEvent::Button button,
						   unsigned char numClicks) = 0;
	virtual void OnMouseMove(DirectX::XMINT2 position, DirectX::XMINT2 deltaPos) = 0;

  private:
	EventHandler(const EventHandler &other) = delete;
	EventHandler &operator=(const EventHandler &other) = delete;
};
