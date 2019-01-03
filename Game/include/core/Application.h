#pragma once

#include "EventListener.h"
#include <memory>

struct Application : public EventListener {
	virtual int Run() = 0;
	virtual ~Application() = default;
};

std::unique_ptr<Application> create_application();
