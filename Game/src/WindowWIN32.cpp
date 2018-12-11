#include "WindowWIN32.h"
#include <Windowsx.h>
#include <unordered_map>

namespace win32 {

static std::unordered_map<HWND, Window *> _Windows;

Event *create_event(UINT msg, LPARAM lParam, WPARAM wParam) {
	Event *e = nullptr;
	SYSTEMTIME time;
	GetSystemTime(&time);
	switch(msg) {
		case WM_LBUTTONDOWN:
			e = new MouseButtonEvent(time.wMilliseconds, 
				0, MouseButtonEvent::Button::LEFT, true, 1,
				GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_KEYDOWN:
			e = new KeyboardEvent(time.wMilliseconds, 0, true, 1, static_cast<KeyboardEvent::Key>(wParam));
			break;
		case WM_KEYUP:
			e = new KeyboardEvent(time.wMilliseconds, 0, false, 1, static_cast<KeyboardEvent::Key>(wParam));
			break;
		case WM_SIZE:
			e = new WindowResizeEvent(time.wMilliseconds, 0, LOWORD(lParam), HIWORD(lParam));
			break;
		default:
			break;
	}
	return e;
}

constexpr int DEFAULT_SHOW = 10;

static LRESULT CALLBACK wnd_proc(HWND hWnd, UINT msg, WPARAM wParam,
								 LPARAM lParam) {
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
	default:
		break;
	}

	Event *e = create_event(msg, lParam, wParam);
	if (e) {
		_Windows[hWnd]->UpdateListeners(e);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window::Window(const std::string title, uint32_t width, uint32_t height)
	: ::Window(title, width, height), m_MSG{nullptr} {
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = wnd_proc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = nullptr;
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = "BasicWndClass";

	if (!RegisterClass(&wc)) {
		MessageBox(nullptr, "RegisterClass FAILED", nullptr, 0);
	}

	m_Window = CreateWindow("BasicWNDClass", m_Title.c_str(),
							WS_OVERLAPPEDWINDOW, 100, 100, width, height,
							nullptr, nullptr, nullptr, nullptr);
	if (m_Window == nullptr) {
		MessageBox(nullptr, "Failed to create Window", nullptr, 0);
	}

	HDC m_DeviceContext = GetDC(m_Window);

	_Windows[m_Window] = this;

	ShowWindow(m_Window, DEFAULT_SHOW);
	UpdateWindow(m_Window);
}

void Window::PollEvents() {
	if (PeekMessage(&m_MSG, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&m_MSG);
		DispatchMessage(&m_MSG);
	}
}

bool Window::ShouldClose() const {
	return (m_MSG.message == WM_QUIT);
}

void *Window::GetAPIHandle() const {
	return m_Window;
}

/* TODO */
void Window::Close() {
}

void Window::ToggleFullscreen() {
}

void Window::UpdateListeners(const Event *e) {
	for(auto listener : m_EventListeners) {
		listener->OnEvent(*e);
	}
}

}


