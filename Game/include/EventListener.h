#pragma once

#include "Event.h"

struct EventListener {
	virtual void OnEvent(const Event &e) = 0;
	virtual ~EventListener();
};
