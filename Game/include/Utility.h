#pragma once

#include <cassert>
#include <sstream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define ASSERT assert

#if defined(DEBUG) || defined(_DEBUG)
#define DEBUG_LOG(x) OutputDebugString(x)
#else
#define DEBUG_LOG(x)
#endif

#define UNUSED(x) (void)x

#if defined(DEBUG) || defined(_DEBUG)

inline static bool CheckD3DError(HRESULT result, int line, const char *file,
								 const char *function) {
	if (result == S_OK)
		return true;
	DEBUG_LOG("D3D11 Error: ");
	switch (result) {
	case D3D11_ERROR_FILE_NOT_FOUND:
		DEBUG_LOG("D3D11_ERROR_FILE_NOT_FOUND");
		break;
	case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
		DEBUG_LOG("D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS");
		break;
	case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
		DEBUG_LOG("D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS");
		break;
	case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD:
		DEBUG_LOG("D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD");
		break;
	case DXGI_ERROR_INVALID_CALL:
		DEBUG_LOG("DXGI_ERROR_INVALID_CALL");
		break;
	case DXGI_ERROR_WAS_STILL_DRAWING:
		DEBUG_LOG("DXGI_ERROR_WAS_STILL_DRAWING");
		break;
	case E_FAIL:
		DEBUG_LOG("E_FAIL");
		break;
	case E_INVALIDARG:
		DEBUG_LOG("E_INVALIDARG");
		break;
	case E_OUTOFMEMORY:
		DEBUG_LOG("E_OUTOFMEMORY");
		break;
	case E_NOTIMPL:
		DEBUG_LOG("E_NOTIMPL");
		break;
	case S_FALSE:
		DEBUG_LOG("S_FALSE");
		break;
	}
	std::stringstream log;
	log << " File: " << file << ", Line: " << line << ": " << function << '\n';
	DEBUG_LOG(log.str().c_str());
	return false;
}

#define DXCall(X)                                                              \
	do {                                                                       \
		HRESULT __hr = X;                                                      \
		if(!CheckD3DError(__hr, __LINE__, __FILE__, #X)) {					   \
			__debugbreak();													   \
		}                                                                      \
	} while (false)
#else
#define DXCall(X) X
#endif

#define DX_SAFE_RELEASE(x)                                                     \
	if (x) {                                                                   \
		x->Release();                                                          \
		x = nullptr;                                                           \
	}

namespace d3d11 {

template <typename T>
class unique_handle {
	T *m_Ptr;

  public:
	unique_handle() : m_Ptr(nullptr) {
	}
	unique_handle(T *value) : m_Ptr(value) {
	}
	~unique_handle() {
		DX_SAFE_RELEASE(m_Ptr);
	}

	void reset(T *value) {
		DX_SAFE_RELEASE(m_Ptr);
		m_Ptr = value;
	}

	T *Get() const {
		return m_Ptr;
	}

	T **GetAddressOf() {
		return &m_Ptr;
	}

	T **ReleaseAndGetAddressOf() {
		DX_SAFE_RELEASE(m_Ptr);
		return &m_Ptr;
	}

	T *operator->() const {
		return m_Ptr;
	}

	unique_handle(const unique_handle &other) = delete;
	unique_handle &operator=(const unique_handle &other) = delete;

	unique_handle(unique_handle &&other) : m_Ptr(other.m_Ptr) {
		other.m_Ptr = nullptr;
	}

	unique_handle &operator=(unique_handle &&other) {
		m_Ptr = other.m_Ptr;
		other.m_Ptr = nullptr;
		return *this;
	}
};

} // namespace d3d11
