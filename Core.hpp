#pragma once

#include <cstdint>
#include <string_view>
#include <exception>

#include "Basic.hpp"

namespace System {

	class EXPIMP Core final {
	public:

		using WindowId = HWND;
		using ApplicationId = HINSTANCE;

		class EXPIMP Exception final : public __STD exception {
		public:

			using ErrorCode = DWORD;

		private:

			ErrorCode code_;

		public:

			explicit Exception(ErrorCode code)noexcept;
			ErrorCode GetCode()const noexcept;

		};

		enum class Style : DWORD {

			//Progress bar
			Green  = PBS_MARQUEE,
			Smooth = PBS_SMOOTH,

			//Edit styles
			OnlyNumbers = ES_NUMBER,
			LeftTextAlign = ES_LEFT,
			RightTextAlign = ES_RIGHT,
			Lowercase = ES_LOWERCASE,
			Multiline = ES_MULTILINE,
			Password = ES_PASSWORD,
			Uppercase = ES_UPPERCASE,
			ReadOnly = ES_READONLY,

			//Buttons styles
			PushButton = BS_PUSHBUTTON,
			CheckBox = BS_CHECKBOX,

			Border = WS_BORDER,

			//This style includes border style
			Caption = WS_CAPTION,
			BorderForSizing = WS_THICKFRAME,
			MinimizeBox = WS_MINIMIZEBOX,
			MaximizeBox = WS_MAXIMIZEBOX,
			Overlapped = WS_OVERLAPPED,
			SystemMenu = WS_SYSMENU,
			NoRedrawChilds = WS_CLIPCHILDREN,
			Disabled = WS_DISABLED,


			//Standart window with border for sizing max., min., close, boxes, caption and icon
			Standart = WS_OVERLAPPEDWINDOW,
			Child = WS_CHILDWINDOW,


			Empty = NULL

		};

		enum class ExStyle : DWORD {

			AlwaysVisible = WS_EX_TOPMOST,
			Transparent = WS_EX_TRANSPARENT,
			Empty = NULL

		};

		class EXPIMP Position final {
		private:

			__STD uint64_t x_;
			__STD uint64_t y_;

		public:

			explicit Position(__STD uint64_t x, __STD uint64_t y)noexcept;

			__STD uint64_t X()const noexcept;
			__STD uint64_t Y()const noexcept;

		};

		using Pos = Position;

		class EXPIMP Size final {
		private:

			__STD uint64_t width_;
			__STD uint64_t height_;

		public:

			explicit Size(__STD uint64_t width, __STD uint64_t height)noexcept;

			__STD uint64_t Width()const noexcept;
			__STD uint64_t Height()const noexcept;

		};

		class WndCreateData final {
		private:

			LPVOID lpCreateParams;
			HINSTANCE hInstance;
			HMENU hMenu;
			HWND hwndParent;
			int cy;
			int cx;
			int y;
			int x;
			LONG style;
			LPCTSTR lpszName;
			LPCTSTR lpszClass;
			DWORD dwExStyle;

		public:
			
			__STD uint64_t GetAdditionalData()const noexcept { return (__STD uint64_t)lpCreateParams; }
			//...
		};

		static __STD uint64_t WindowTitleBarHeight(WindowId wnd_id) {
			
			TITLEBARINFO title_info{ 0 };
			title_info.cbSize = sizeof(TITLEBARINFO);

			if (GetTitleBarInfo(wnd_id, &title_info)) {

				return (__STD uint64_t)title_info.rcTitleBar.bottom - title_info.rcTitleBar.top;

			}

			throw Exception{ GetLastError() };

		}

		static Size GetWndSizeWithThisClientSize(const Size& size, Style style, ExStyle ex_style, bool menu = false) {

			RECT rect{ 0, 0, size.Width(), size.Height() };

			if (AdjustWindowRectEx(&rect, to_basic(style), menu, to_basic(ex_style))) {

				return Size{ (__STD uint64_t)rect.right - rect.left, (__STD uint64_t)rect.bottom - rect.top };

			}

			throw Exception{ GetLastError() };

		}

		static __STD uint64_t WindowTitleBarWidth(WindowId wnd_id) {

			TITLEBARINFO title_info{ 0 };
			title_info.cbSize = sizeof(TITLEBARINFO);

			if (GetTitleBarInfo(wnd_id, &title_info)) {

				return (__STD uint64_t)title_info.rcTitleBar.right - title_info.rcTitleBar.left;

			}

			throw Exception{ GetLastError() };

		}

		[[nodiscard]]static __STD uint64_t ScreenWidth()noexcept {

			return GetSystemMetrics(SM_CXSCREEN);

		}

		static __STD uint64_t ScreenHeight()noexcept {

			return GetSystemMetrics(SM_CYSCREEN);

		}

		static bool WndClassRegistered(const __STD wstring_view& class_name)noexcept {

			WNDCLASS wc;

			return GetClassInfo(GetModuleHandle(NULL), class_name.data(), &wc);

		}

		using WindowEventsHandler = WNDPROC;

		static void RegisterWndClass(const __STD wstring_view& class_name, WindowEventsHandler handler) {

			WNDCLASSEX wc;

			wc.cbSize = sizeof(wc);
			wc.style = CS_HREDRAW | CS_VREDRAW;
			wc.lpfnWndProc = handler;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = sizeof(__STD uint64_t);
			wc.hInstance = (HINSTANCE)GetModuleHandle(NULL);
			wc.hIcon = NULL;//LoadIcon(NULL, IDI_APPLICATION);
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
			wc.lpszMenuName = NULL;
			wc.lpszClassName = class_name.data();
			wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

			if (!RegisterClassEx(&wc))
				throw Exception{ GetLastError() };

		}

		[[nodiscard]]static HWND CreateWnd(ExStyle ex_style, const __STD wstring_view& class_name, const __STD wstring_view& text, Style style, const Size& size, const Position& pos, WindowId parent_hWnd, __STD uint64_t additional_data = NULL) {

			const HWND self_id_ = CreateWindowEx(to_basic(ex_style),
				class_name.data(),
				text.data(),
				to_basic(style),
				(int)pos.X(),
				(int)pos.Y(),
				(int)size.Width(),
				(int)size.Height(),
				parent_hWnd,
				(HMENU)NULL,
				(HINSTANCE)GetModuleHandle(NULL),
				(LPVOID)additional_data);
	
			if (!self_id_)
				throw Exception{ GetLastError() };

			return self_id_;

		};

		[[noreturn]] static void StartMainLoop() {

			MSG message{ 0 };
			BOOL no_error = 0;

			while ((no_error = GetMessage(&message, NULL, NULL, NULL)) != 0) {

				TranslateMessage(&message);

				//Call WinProc function and get MSG msg message
				DispatchMessage(&message);

			}

		};

		static void DestroyWnd(WindowId wnd_hWnd) {

			DestroyWindow(wnd_hWnd);

		}

		static void SaveToWndMemory(WindowId wnd_hWnd, __STD uint64_t data) {

			SetLastError(NULL);

			const LONG_PTR last_value = SetWindowLongPtr(wnd_hWnd, GWLP_USERDATA, (LONG_PTR)data);

			if (!last_value) {

				const DWORD failure = GetLastError();

				if (failure) {

					throw Exception{ failure };

				}

			}

		}

		[[nodiscard]]static __STD uint64_t GetWndMemory(WindowId wnd_id) {

			auto data = (__STD uint64_t)GetWindowLongPtr(wnd_id, GWLP_USERDATA);

			if (!data) {

				throw Exception{ GetLastError() };

			}

			return data;

		}

		//This window does not scale for DPI changes. It will query for the DPI once and use that value for the lifetime of the process. 
		//If the DPI changes, the process will not adjust to the new DPI value. 
		//It will be automatically scaled up or down by the system when the DPI changes from the system value.
		static void SetSystemDPI() {

			if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE)) {

				throw Exception{ GetLastError() };

			}

		}

		//Sends WM_QUIT to the message queue and GetMessage returns zero
		static void QuitFromMainLoop(int exit_code) {

			PostQuitMessage(exit_code);

		}

		static void ShowWnd(WindowId wnd_id) {

			ShowWindow(wnd_id, SW_SHOW);

		}

	};

};