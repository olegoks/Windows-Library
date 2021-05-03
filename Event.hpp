#pragma once

#ifndef EVENT_HPP
#define EVENT_HPP

#include <cstdint>
#include <type_traits>
#include <memory>

#include "Basic.hpp"
#include "Control.hpp"

class IEvent {
public:

	enum class Source : __STD uint64_t {

		Window,
		Control,
		Empty

	};

	enum class Action : UINT {

		Created = WM_NCCREATE,
		IsActive = WM_ACTIVATE,
		CloseAppplication = WM_DESTROY,
		Moved = WM_MOVE,
		LostKeyboardFocus = WM_KILLFOCUS,
		GetKeyboardFocus = WM_SETFOCUS,
		SizeChanged = WM_SIZE,
		PositionSizeChanged = WM_WINDOWPOSCHANGED,
		PositionSizeChanging = WM_WINDOWPOSCHANGING,
		Close = WM_CLOSE,
		RepaintClientZone = WM_PAINT,

		//Mouse
		MouseMove = WM_MOUSEMOVE,
		LeftButtonDown = WM_LBUTTONDOWN,
		RightButtonDown = WM_RBUTTONDOWN,
		LeftButtonUp = WM_LBUTTONUP,
		RightButtonUp = WM_RBUTTONUP,

	};

private:

	Source source_;

protected:

	HWND hWnd_;
	UINT message_;
	WPARAM wParam_;
	LPARAM lParam_;

public:

	explicit IEvent(Source source, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)noexcept :
		source_{ source },
		hWnd_{ hWnd },
		message_{ message },
		wParam_{ wParam },
		lParam_{ lParam }{ }

	__forceinline Source GetSource()const noexcept { return source_; };

	System::Core::WndCreateData* GetCreateDataPointer()const noexcept { return (System::Core::WndCreateData*)lParam_; }

	Action GetAction()const noexcept { return to_enum_type<Action>(message_); }

	virtual ~IEvent()noexcept = default;

};

class EXPIMP ControlEvent : public IEvent {
public:

	enum class Action : UINT {

		//Button actions
		ButtonMouseLeaveEnter = BCN_HOTITEMCHANGE,
		ButtonClicked = BN_CLICKED,
		ButtonDoubleClicked = BN_DOUBLECLICKED,
		ButtonLostKeyboardFocus = BN_KILLFOCUS,
		ButtonGetKeyboardFocus = BN_SETFOCUS,
		ButtonReleased = BN_UNPUSHED

		//Edit actions
		
	};

	explicit ControlEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)noexcept :
		IEvent::IEvent(Source::Control, hWnd, message, wParam, lParam) {}

	System::Windows::Control* GetControl()const noexcept {

		using namespace System;
		using namespace System::Windows;

		return reinterpret_cast<Control*>(Core::GetWndMemory((Control::Id)lParam_));
	
	}

	__forceinline Action GetAction()const noexcept {

		return to_enum_type<Action>(HIWORD(wParam_));

	}

};

class EXPIMP WindowEvent : public IEvent {
public:

	enum class Action : UINT {

		Created = WM_NCCREATE,
		IsActive = WM_ACTIVATE,
		CloseAppplication = WM_DESTROY,
		Moved = WM_MOVE,
		LostKeyboardFocus = WM_KILLFOCUS,
		GetKeyboardFocus = WM_SETFOCUS,
		SizeChanged = WM_SIZE,
		PositionSizeChanged = WM_WINDOWPOSCHANGED,
		PositionSizeChanging = WM_WINDOWPOSCHANGING,
		Close = WM_CLOSE,
		RepaintClientZone = WM_PAINT,

		//Mouse
		MouseMove = WM_MOUSEMOVE,
		LeftButtonDown = WM_LBUTTONDOWN,
		RightButtonDown = WM_RBUTTONDOWN,
		LeftButtonUp = WM_LBUTTONUP,
		RightButtonUp = WM_RBUTTONUP,

	};

	explicit WindowEvent(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)noexcept :
		IEvent::IEvent{ Source::Window, hWnd, Message, wParam, lParam }{}

	uint64_t GetMouseX()const noexcept { return LOWORD(lParam_); };
	uint64_t GetMouseY()const noexcept { return HIWORD(lParam_); };

	uint64_t GetWindowX()const noexcept { return 0; };
	uint64_t GetWindowY()const noexcept { return 0; };

	uint64_t GetWindowWidth()const noexcept { return 0; };
	uint64_t GetWindowHeight()const noexcept { return 0; };

	System::Windows::Window* GetWindow()const noexcept {

		return reinterpret_cast<System::Windows::Window*>(System::Core::GetWndMemory((System::Windows::Window::Id)hWnd_));

	};

	__forceinline Action GetAction()const noexcept {

		return to_enum_type<Action>(message_);

	};

};

union EventMemory {

	WindowEvent window_event_;
	ControlEvent control_event_;

	~EventMemory() = default;
};

class EventBuilder final {
private:

	explicit EventBuilder()noexcept {}

public:

	static EventBuilder* GetInstance() {

		static EventBuilder* builder = nullptr;

		if (builder == nullptr) {

			builder = new EventBuilder{};

		}

		return builder;

	}

	IEvent* ConstructEvent(HWND hWnd, UINT Message, WPARAM wparam, LPARAM lparam) {

		static char event_memory[sizeof(EventMemory)] = { 0 };

		if ((Message == WM_COMMAND) && lparam) {

			new (event_memory) ControlEvent{ hWnd, Message, wparam, lparam };

		}
		else if ((Message >= 0 && Message <= 173) || (Message >= 255 && Message <= 911)) {

			new (event_memory) WindowEvent{ hWnd, Message, wparam, lparam };

		}

		return (IEvent*)event_memory;

	}

};


#endif //EVENT_HPP