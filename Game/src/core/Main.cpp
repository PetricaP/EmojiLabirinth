#pragma once

#include "Application.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <memory>

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
		   _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	auto app = create_application();
	return app->Run();
}
