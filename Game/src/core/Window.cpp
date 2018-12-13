#include "Window.h"

void Window::SetWidth(uint32_t width) {
	m_Width = width;
}

void Window::SetHeight(uint32_t height) {
	m_Height = height;
}

Window::Window(const std::string title, uint32_t width, uint32_t height)
	: m_Title(title), m_Width(width), m_Height(height) {
}
