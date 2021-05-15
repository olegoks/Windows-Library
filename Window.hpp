#pragma once

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <functional>
#include <string>
#include <string_view>
#include <exception>

#include <Windows.h>
#include <wininet.h>
#include <Commctrl.h>
#include <windef.h>
#include <WinUser.h>

#define __STD ::std::

#define COMPILING_DLL

#ifdef COMPILING_DLL
#define EXPIMP __declspec(dllexport)
#else
#define EXPIMP __declspec(dllimport)
#endif

#pragma warning(disable:4251)
#pragma warning(disable:4275)
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "Event.hpp"
#include "Pixel.hpp"

namespace System {

	//Convert enum type to basic type
	template<class EnumClass>
	constexpr __STD underlying_type_t<EnumClass> to_basic(EnumClass e)noexcept {

		return static_cast<__STD underlying_type_t<EnumClass>>(e);

	}

	//Convert enum type to basic type
	template<class BasicType, class EnumClass>
	constexpr BasicType to_basic(EnumClass e)noexcept {

		return static_cast<BasicType>(e);

	}

	template<class EnumClass, class BasicType>
	constexpr EnumClass to_enum_type(BasicType value)noexcept {

		return static_cast<EnumClass>(value);

	}

	class EXPIMP Exception final : public __STD exception {
	public:

		using ErrorCode = DWORD;

	private:

		ErrorCode code_;

	public:

		explicit Exception() = default;

		explicit Exception(ErrorCode code)noexcept;
		ErrorCode GetErrorCode()const noexcept;

		~Exception()noexcept = default;

	};

	class EXPIMP Core final {
	public:

		[[nodiscard]] static __STD uint64_t GetScreenWidth()noexcept;
		[[nodiscard]] static __STD uint64_t GetScreenHeight()noexcept;
		[[noreturn]] static void StartMainLoop()noexcept;

		//Sends WM_QUIT to the message queue and GetMessage returns zero
		static void QuitFromMainLoop(int exit_code)noexcept;

		//This window does not scale for DPI changes. It will query for the DPI once and use that value for the lifetime of the process. 
		//If the DPI changes, the process will not adjust to the new DPI value. 
		//It will be automatically scaled up or down by the system when the DPI changes from the system value.
		static void SetSystemDPI();

	};

	class EXPIMP Position final {
	private:

		__STD uint64_t x_;
		__STD uint64_t y_;

	public:

		constexpr explicit Position()noexcept :
			x_{ 0 }, y_{ 0 }{}

		constexpr explicit Position(__STD uint64_t x, __STD uint64_t y)noexcept :
			x_{ x }, y_{ y }{}

		[[nodiscard]] __forceinline __STD uint64_t GetX()const noexcept { return x_; }
		[[nodiscard]] __forceinline __STD uint64_t GetY()const noexcept { return y_; }

	};

	class EXPIMP Size final {
	private:

		__STD uint64_t width_;
		__STD uint64_t height_;

	public:

		constexpr explicit Size(__STD uint64_t width, __STD uint64_t height)noexcept :
			width_{ width }, height_{ height }{}

		[[nodiscard]] constexpr __forceinline __STD uint64_t GetWidth()const noexcept { return width_; }
		[[nodiscard]] constexpr __forceinline __STD uint64_t GetHeight()const noexcept { return height_; }

	};

	class EXPIMP Window {
	public:

		using Id = HWND;
		using StyleBasicType = DWORD;

		enum class Style : StyleBasicType {

			//This style includes border style
			Caption = WS_CAPTION,
			BorderForSizing = WS_THICKFRAME,
			MinimizeBox = WS_MINIMIZEBOX,
			MaximizeBox = WS_MAXIMIZEBOX,
			Overlapped = WS_OVERLAPPED,
			SystemMenu = WS_SYSMENU,
			NoRedrawChilds = WS_CLIPCHILDREN,
			Disabled = WS_DISABLED,
			Border = WS_BORDER,
			//Standart window with border for sizing max., min., close, boxes, caption and icon
			Standart = 0x00000000L,//WS_OVERLAPPEDWINDOW,
			Child = 0x40000000L,//WS_CHILDWINDOW,

			Empty = 0

		};

		enum class ExStyle : StyleBasicType {

			//Can be with caption
			DoubleFrame = 0x00000001L,//WS_EX_DLGMODALFRAME
			AlwaysVisible = 0x00000008L,//WS_EX_TOPMOST,
			Transparent = 0x00000020L,//WS_EX_TRANSPARENT,
			ConvexFrame = 0x00000100L,//WS_EX_WINDOWEDGE,
			ToolWindow = 0x00000080L,//WS_EX_TOOLWINDOW,
			RecessedEdgeFrame = 0x00000200L,//WS_EX_CLIENTEDGE,
			Frame3D = 0x00020000L,//WS_EX_STATICEDGE,
			Empty = 0

		};

		class Event : public IEvent {
		public:

			enum class Action : unsigned int {

				Created = 0x0081,//WM_NCCREATE,
				IsActive = 0x0006,//WM_ACTIVATE,
				CloseAppplication = 0x0002,//WM_DESTROY,
				Moved = 0x0003,//WM_MOVE,
				LostKeyboardFocus = 0x0008,//WM_KILLFOCUS,
				GetKeyboardFocus = 0x0007,//WM_SETFOCUS,
				SizeChanged = 0x0005,//WM_SIZE,
				PositionSizeChanged = 0x0047,//WM_WINDOWPOSCHANGED,
				PositionSizeChanging = 0x0046,//WM_WINDOWPOSCHANGING,
				Close = 0x0010,//WM_CLOSE,
				RepaintClientZone = 0x000F,//WM_PAINT,

				//Mouse
				MouseMove = 0x0200,//WM_MOUSEMOVE,
				LeftButtonDown = 0x0201,//WM_LBUTTONDOWN,
				RightButtonDown = 0x0204,//WM_RBUTTONDOWN,
				LeftButtonUp = 0x0202,//WM_LBUTTONUP,
				RightButtonUp = 0x0205,//WM_RBUTTONUP,

			};

		public:

			explicit Event(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)noexcept :
				IEvent::IEvent{ Source::Window, hWnd, message, wParam, lParam }{}

			explicit Event(Source source, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)noexcept :
				IEvent::IEvent{ source, hWnd, message, wParam, lParam }{}

			__STD uint64_t GetMouseX()const noexcept;
			__forceinline __STD uint64_t GetMouseY()const noexcept;

			__forceinline __STD uint64_t GetWindowX()const noexcept { return 0; };
			__forceinline __STD uint64_t GetWindowY()const noexcept { return 0; };

			__forceinline __STD uint64_t GetWindowWidth()const noexcept { return 0; };
			__forceinline __STD uint64_t GetWindowHeight()const noexcept { return 0; };

			Window& GetWindow()const noexcept;

			Action GetAction()const noexcept;

		};

		using EventHandler = __STD function<bool(const Event& event)>;

		static inline const __STD wstring_view	kDefaultText = L"";
		static inline const Size				kDefaultSize{ Core::GetScreenWidth() / 2, Core::GetScreenHeight() / 2 };
		static inline const Position			kDefaultPosition{ 0, 0 };
		static inline const Style				kDefaultStyle = Style::Empty;
		static inline const ExStyle				kDefaultExStyle = ExStyle::Empty;
		static inline const __STD wstring_view	kDefaultClassName = L"Component";
		static inline const Color				kDefaultBackgroundColor{ 255, 255, 255 };
		static inline const EventHandler		kDefaultEventHandler = [](const IEvent& event)noexcept->bool { return false; };

	private:

		HDC dc_;
		Id self_id_;
		Id parent_id_;
		__STD wstring_view class_name_;
		EventHandler handler_;

	protected:


		void SetEventHandler(EventHandler handler)noexcept;
		void RegisterWndClass(const __STD wstring_view& class_name);
		void CreateWnd(Style style, ExStyle ex_style, __STD wstring_view text, const Position& pos, const Size& size, Id parent_wnd_id = NULL);
		void CreateWnd(__STD wstring_view text, Id parent_wnd_id = NULL);
		void CreateWnd(Style style, ExStyle ex_style, Id parent_wnd_id = NULL);
		void CreateWnd(Style style, __STD wstring_view text, Id parent_wnd_id = NULL);
		void CreateWnd(Style style, __STD wstring_view text, const Position& pos, const Size& size, Id parent_wnd_id = NULL);
		void ShowWnd();
		void DestroyWnd();

	public:

		static Size GetWndSizeWithThisClientSize(const Size& size, Style style, Style ex_style, bool menu);

		virtual bool CallEventHandler(const Event& event)noexcept;

		explicit Window()noexcept;

		void ShowRGBAImage(Pixel* image, Size size);

		bool Enable()noexcept;
		bool Disable()noexcept;

		Size GetSize()const;
		Position GetPosition()const;

		__STD uint64_t GetX()const;
		__STD uint64_t GetY()const;

		Size GetClientSize()const;

		__STD uint64_t GetWidth()const;
		__STD uint64_t GetHeight()const;

		Id GetParentId()const noexcept { return parent_id_; }
		Id GetId()const noexcept { return self_id_; }
		bool Created()const noexcept { return self_id_; }

		__STD wstring GetText()const;
		void SetText(__STD wstring_view text);

		virtual void Create(Id parent_id = NULL) = 0;
		virtual void Show() = 0;
		virtual void Destroy() = 0;

		virtual ~Window()noexcept;

	};

	template<class FirstStyle, class SecondStyle>
	constexpr Window::Style operator|(FirstStyle first, SecondStyle second)noexcept {

		return to_basic(first) | to_basic(second);

	}

	//template<class SomeStyle>
	//constexpr __forceinline Window::Style operator|(Window::Style first, SomeStyle second)noexcept {

	//	return (__STD uint32_t)first | to_basic<Window::StyleBasicType>(second);

	//}

	template<class SomeStyle>
	constexpr Window::Style operator|(SomeStyle second, Window::Style first)noexcept {

		return to_enum_type<Window::Style>((Window::StyleBasicType)first | to_basic<Window::StyleBasicType>(second));

	}

};

#endif //WINDOW_HPP