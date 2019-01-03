#include "Renderer.h"
#include "WindowWIN32.h"

int main() {
	win32::Window window("Test window", 800, 600);
	while(!window.ShouldClose()) {
		window.PollEvents();
	}
}
