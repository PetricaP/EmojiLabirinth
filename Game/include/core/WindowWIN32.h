#pragma once

#include "Window.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace win32 {

class Window : public ::Window {
  private:
	HWND m_Window;
	MSG m_MSG;

  public:
	Window(const std::string title, uint32_t width, uint32_t height);
	~Window() override = default;

	void PollEvents() override;
	bool ShouldClose() const override;
	void *GetAPIHandle() const override;
	void Close() override;
	void ToggleFullscreen() override;

	friend LRESULT CALLBACK wnd_proc(HWND hWnd, UINT msg, WPARAM wParam,
									 LPARAM lParam);
	void SetWidth(uint32_t width) {
		m_Width = width;
	}

	void SetHeight(uint32_t height) {
		m_Height = height;
	}

	float GetAspectRatio() const {
		return static_cast<float>(m_Height) / m_Width;
	}

  private:
	void UpdateListeners(const Event *e);
};

} // namespace win32
