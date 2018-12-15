#pragma once

#include "Utility.h"

class Event {
  public:
	enum class Type {
		UNKNOWN,
		QUIT,
		KEYBOARD,
		MOUSE_MOTION,
		MOUSE_BUTTON,
		MOUSE_WHEEL,
		WINDOW_RESIZE,
		WINDOW_FOCUS
	};

  protected:
	const Type m_Type;
	const uint32_t m_TimeStamp;

  public:
	Type GetType() const { return m_Type; }
	uint32_t GetTimestamp() const { return m_TimeStamp; }

	Event(Type type, uint32_t timestamp)
		: m_Type(type), m_TimeStamp(timestamp) {}
};

class WindowEvent : public Event {
  private:
	uint32_t m_WindowID;

  protected:
	WindowEvent(Event::Type type, uint32_t timestamp, uint32_t windowID)
		: Event(type, timestamp), m_WindowID(windowID) {}

  public:
	uint32_t GetWindowID() const { return m_WindowID; }
};

class KeyboardEvent : public WindowEvent {
  public:
	enum class Key {
		LBUTTON		   = 0x01,
		RBUTTON        = 0x02,
		CANCEL         = 0x03,
		MBUTTON        = 0x04,
		BACK           = 0x08,
		TAB            = 0x09,
		CLEAR          = 0x0C,
		RETURN         = 0x0D,
		SHIFT          = 0x10,
		CONTROL        = 0x11,
		MENU           = 0x12,
		PAUSE          = 0x13,
		CAPITAL        = 0x14,
		KANA           = 0x15,
		HANGEUL        = 0x15,
		HANGUL         = 0x15,
		JUNJA          = 0x17,
		FINAL          = 0x18,
		HANJA          = 0x19,
		KANJI          = 0x19,
		ESCAPE         = 0x1B,
		CONVERT        = 0x1C,
		NONCONVERT     = 0x1D,
		ACCEPT         = 0x1E,
		MODECHANGE     = 0x1F,
		SPACE          = 0x20,
		PRIOR          = 0x21,
		NEXT           = 0x22,
		END            = 0x23,
		HOME           = 0x24,
		LEFT           = 0x25,
		UP             = 0x26,
		RIGHT          = 0x27,
		DOWN           = 0x28,
		SELECT         = 0x29,
		PRINT          = 0x2A,
		EXECUTE        = 0x2B,
		SNAPSHOT       = 0x2C,
		INSERT         = 0x2D,
		HELP           = 0x2F,
		K0			   = 0x30,
		K1			   = 0x31,
		K2			   = 0x32,
		K3			   = 0x33,
		K4			   = 0x34,
		K5			   = 0x35,
		K6			   = 0x36,
		K7			   = 0x37,
		K8			   = 0x38,
		K9			   = 0x39,
		A			   = 0x41,
		B			   = 0x42,
		C			   = 0x43,
		D			   = 0x44,
		E			   = 0x45,
		F			   = 0x47,
		G			   = 0x48,
		H			   = 0x49,
		J			   = 0x4A,
		I			   = 0x4B,
		L			   = 0x4C,
		M			   = 0x4D,
		N			   = 0x4E,
		O			   = 0x4F,
		P			   = 0x50,
		Q			   = 0x51,
		R			   = 0x52,
		S			   = 0x53,
		T			   = 0x54,
		U			   = 0x55,
		V			   = 0x56,
		W			   = 0x57,
		X			   = 0x58,
		Y			   = 0x59,
		Z			   = 0x60,
		LWIN           = 0x5B,
		RWIN           = 0x5C,
		APPS           = 0x5D,
		SLEEP          = 0x5F,
		NUMPAD0        = 0x60,
		NUMPAD1        = 0x61,
		NUMPAD2        = 0x62,
		NUMPAD3        = 0x63,
		NUMPAD4        = 0x64,
		NUMPAD5        = 0x65,
		NUMPAD6        = 0x66,
		NUMPAD7        = 0x67,
		NUMPAD8        = 0x68,
		NUMPAD9        = 0x69,
		MULTIPLY       = 0x6A,
		ADD            = 0x6B,
		SEPARATOR      = 0x6C,
		SUBTRACT       = 0x6D,
		DECIMAL        = 0x6E,
		DIVIDE         = 0x6F,
		F1             = 0x70,
		F2             = 0x71,
		F3             = 0x72,
		F4             = 0x73,
		F5             = 0x74,
		F6             = 0x75,
		F7             = 0x76,
		F8             = 0x77,
		F9             = 0x78,
		F10            = 0x79,
		F11            = 0x7A,
		F12            = 0x7B,
		F13            = 0x7C,
		F14            = 0x7D,
		F15            = 0x7E,
		F16            = 0x7F,
		F17            = 0x80,
		F18            = 0x81,
		F19            = 0x82,
		F20            = 0x83,
		F21            = 0x84,
		F22            = 0x85,
		F23            = 0x86,
		F24            = 0x87,
		NUMLOCK        = 0x90,
		SCROLL         = 0x91,
		LSHIFT         = 0xA0,
		RSHIFT         = 0xA1,
		LCONTROL       = 0xA2,
		RCONTROL       = 0xA3,
		LMENU          = 0xA4,
		RMENU          = 0xA5,
		GAMEPAD_A                         = 0xC3,
		GAMEPAD_B                         = 0xC4,
		GAMEPAD_X                         = 0xC5,
		GAMEPAD_Y                         = 0xC6,
		GAMEPAD_RIGHT_SHOULDER            = 0xC7,
		GAMEPAD_LEFT_SHOULDER             = 0xC8,
		GAMEPAD_LEFT_TRIGGER              = 0xC9,
		GAMEPAD_RIGHT_TRIGGER             = 0xCA,
		GAMEPAD_DPAD_UP                   = 0xCB,
		GAMEPAD_DPAD_DOWN                 = 0xCC,
		GAMEPAD_DPAD_LEFT                 = 0xCD,
		GAMEPAD_DPAD_RIGHT                = 0xCE,
		GAMEPAD_MENU                      = 0xCF,
		GAMEPAD_VIEW                      = 0xD0,
		GAMEPAD_LEFT_THUMBSTICK_BUTTON    = 0xD1,
		GAMEPAD_RIGHT_THUMBSTICK_BUTTON   = 0xD2,
		GAMEPAD_LEFT_THUMBSTICK_UP        = 0xD3,
		GAMEPAD_LEFT_THUMBSTICK_DOWN      = 0xD4,
		GAMEPAD_LEFT_THUMBSTICK_RIGHT     = 0xD5,
		GAMEPAD_LEFT_THUMBSTICK_LEFT      = 0xD6,
		GAMEPAD_RIGHT_THUMBSTICK_UP       = 0xD7,
		GAMEPAD_RIGHT_THUMBSTICK_DOWN     = 0xD8,
		GAMEPAD_RIGHT_THUMBSTICK_RIGHT    = 0xD9,
		GAMEPAD_RIGHT_THUMBSTICK_LEFT     = 0xDA
	};

  private:
	const bool m_Pressed;
	const uint8_t m_Repeat;
	const Key m_Key;

  public:
	KeyboardEvent(uint32_t timestamp, uint32_t windowID, bool pressed,
				  uint8_t repeat, Key key)
		: WindowEvent(Event::Type::KEYBOARD, timestamp, windowID),
		  m_Pressed(pressed), m_Repeat(repeat), m_Key(key) {}

	bool GetPressed() const { return m_Pressed; }
	uint8_t GetRepeat() const { return m_Repeat; }
	Key GetKey() const { return m_Key; }
};

class MouseMotionEvent : public WindowEvent {
  private:
	const int32_t m_X;
	const int32_t m_Y;
	const int32_t m_RelativeX;
	const int32_t m_RelativeY;

  public:
	MouseMotionEvent(uint32_t timestamp, uint32_t windowID, int32_t x,
					 int32_t y, int32_t relativeX, int32_t relativeY)
		: WindowEvent(Event::Type::MOUSE_MOTION, timestamp, windowID), m_X(x),
		  m_Y(y), m_RelativeX(relativeX), m_RelativeY(relativeY) {}

	int32_t GetX() const { return m_X; }
	int32_t GetY() const { return m_Y; }
	int32_t GetRelativeX() const { return m_RelativeX; }
	int32_t GetRelativeY() const { return m_RelativeY; }
};

class MouseButtonEvent : public WindowEvent {
  public:
	enum class Button : uint32_t { LEFT = 1, MIDDLE = 2, RIGHT = 3 };

  private:
	const Button m_Button;
	const bool m_Pressed;
	const uint8_t m_Clicks;
	const int32_t m_X;
	const int32_t m_Y;

  public:
	MouseButtonEvent(uint32_t timestamp, uint32_t windowID, Button button,
					 bool pressed, uint8_t clicks, int32_t x, int32_t y)
		: WindowEvent(Event::Type::MOUSE_BUTTON, timestamp, windowID),
		  m_Button(button), m_Pressed(pressed), m_Clicks(clicks), m_X(x),
		  m_Y(y) {}

	Button GetButton() const { return m_Button; }
	bool GetPressed() const { return m_Pressed; }
	uint8_t GetClicks() const { return m_Clicks; }
	int32_t GetX() const { return m_X; }
	int32_t GetY() const { return m_Y; }
};

class MouseWheelEvent : public WindowEvent {
  public:
	enum class Direction { FORWARD, BACKWARDS };

  private:
	const int32_t m_X;
	const int32_t m_Y;
	const Direction m_Direction;

  public:
	MouseWheelEvent(uint32_t timestamp, uint32_t windowID, int32_t x, int32_t y,
					Direction direction)
		: WindowEvent(Event::Type::MOUSE_WHEEL, timestamp, windowID), m_X(x),
		  m_Y(y), m_Direction(direction) {}

	int32_t GetX() const { return m_X; }
	int32_t GetY() const { return m_Y; }
	Direction GetDirection() const { return m_Direction; }
};

class WindowResizeEvent : public WindowEvent {
  private:
	const uint32_t m_Width;
	const uint32_t m_Height;

  public:
	WindowResizeEvent(uint32_t timestamp, uint32_t windowID, uint32_t width,
					  uint32_t height)
		: WindowEvent(Event::Type::WINDOW_RESIZE, timestamp, windowID),
		  m_Width(width), m_Height(height) {}

	uint32_t GetWidth() const { return m_Width; }
	uint32_t GetHeight() const { return m_Height; }
};

class WindowFocusEvent : public WindowEvent {
  private:
	const bool m_Focused;

  public:
	WindowFocusEvent(uint32_t timestamp, uint32_t windowID, bool focused)
		: WindowEvent(Event::Type::WINDOW_FOCUS, timestamp, windowID),
		  m_Focused(focused) {}

	bool GetFocused() const { return m_Focused; }
};

class QuitEvent : public WindowEvent {
  public:
	QuitEvent(uint32_t timestamp, uint32_t windowID)
		: WindowEvent(Event::Type::QUIT, timestamp, windowID) {}
};
