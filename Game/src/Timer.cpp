#include "Timer.h"
#include <windows.h>

namespace win32 {

Timer::Timer()
	: m_SecondsPerCount(0.0), m_DeltaTime(-1.0), m_BaseTime(0), m_PausedTime(0),
	  m_PrevTime(0), m_CurrTime(0), m_Stopped(false) {
	__int64 countsPerSec;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER *>(&countsPerSec));
	m_SecondsPerCount = 1.0 / static_cast<double>(countsPerSec);
}

float Timer::TotalTime() const {
	if (m_Stopped) {
		return static_cast<float>(((m_StopTime - m_PausedTime) - m_BaseTime) *
								  m_SecondsPerCount);
	} else {
		return static_cast<float>(((m_CurrTime - m_PausedTime) - m_BaseTime) *
								  m_SecondsPerCount);
	}
}

float Timer::DeltaTime() const { return static_cast<float>(m_DeltaTime); }

void Timer::Reset() {
	__int64 currTime;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&currTime));

	m_BaseTime = currTime;
	m_PrevTime = currTime;
	m_StopTime = 0;
	m_Stopped = false;
}

void Timer::Start() {
	if (m_Stopped) {
		__int64 startTime;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&startTime));

		m_PausedTime += (startTime - m_StopTime);
		m_PrevTime = startTime;
		m_StopTime = 0;
		m_Stopped = false;
	}
}

void Timer::Stop() {
	if (!m_Stopped) {
		__int64 currTime;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&currTime));

		m_StopTime = currTime;
		m_Stopped = true;
	}
}

void Timer::Tick() {
	if (m_Stopped) {
		m_DeltaTime = 0.0;
		return;
	}
	__int64 currTime;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&currTime));
	m_CurrTime = currTime;

	m_DeltaTime = (m_CurrTime - m_PrevTime) * m_SecondsPerCount;
	m_PrevTime = m_CurrTime;

	if (m_DeltaTime < 0.0) {
		m_DeltaTime = 0.0;
	}
}


void Timer::CapFrameTime(float seconds) {
	float frameTime = DeltaTime();
	if (frameTime < seconds) {
		float delayedTime = (seconds - frameTime) * 1000.f;
		Sleep(static_cast<DWORD>(delayedTime));
	}
}

}
