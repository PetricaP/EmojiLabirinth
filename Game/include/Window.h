#ifndef WINDOW_H
#define WINDOW_H

#include "Utility.h"
#include "EventListener.h"
#include <string>
#include <windows.h>
#include <vector>

class Window {
  protected:
	std::string m_Title;
	uint32_t m_Width;
	uint32_t m_Height;
	std::vector<EventListener *> m_EventListeners;
	
	virtual void SetWidth(uint32_t width);
	virtual void SetHeight(uint32_t height);

  public:
	Window(const std::string title, uint32_t width, uint32_t height);
	virtual ~Window() = default;

	Window(const Window &window) = delete;
	Window &operator=(const Window &window) = delete;
	Window(Window &&window) = delete;
	Window &operator=(Window &&window) = delete;

	virtual void PollEvents() = 0;
	virtual bool ShouldClose() const = 0;
	virtual void *GetAPIHandle() const = 0;
	virtual void Close() = 0;
	virtual void ToggleFullscreen() = 0;

	void AddEventListener(EventListener *eventListener);

	uint32_t GetWidth() const;
	uint32_t GetHeight() const;
};

inline void Window::AddEventListener(EventListener *eventListener) {
	m_EventListeners.push_back(eventListener);
}

inline uint32_t Window::GetWidth() const {
	return m_Width;
}

inline uint32_t Window::GetHeight() const {
	return m_Height;
}

#endif
