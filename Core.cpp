
#include "Window.hpp"

namespace System {

	__STD uint64_t Core::GetScreenWidth()noexcept {

		return GetSystemMetrics(SM_CXSCREEN);

	}

	__STD uint64_t Core::GetScreenHeight()noexcept {

		return GetSystemMetrics(SM_CYSCREEN);

	}

	void Core::StartMainLoop()noexcept {

		MSG message{ 0 };
		BOOL no_error = 0;

		while ((no_error = GetMessage(&message, NULL, NULL, NULL)) != 0) {

			TranslateMessage(&message);

			//Call WinProc function and get MSG msg message
			DispatchMessage(&message);

		}

	};

	void Core::SetSystemDPI() {

		if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE)) {

			throw Exception{ GetLastError() };

		}

	}

	//Sends WM_QUIT to the message queue and GetMessage returns zero
	void Core::QuitFromMainLoop(int exit_code)noexcept {

		PostQuitMessage(exit_code);

	}

}