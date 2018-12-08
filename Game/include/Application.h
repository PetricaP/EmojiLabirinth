#pragma once

#include "EventListener.h"

struct Application : public EventListener {
	virtual int Run() = 0;
	virtual ~Application() = default;
};

Application *create_application();
