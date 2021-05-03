#include "Window.hpp"
#include "Control.hpp"

#include "Event.hpp"

using namespace System;
using namespace System::Windows;

Window::Window() noexcept :
	self_id_{ NULL },
	parent_id_{ NULL },
	class_name_{ },
	handler_{ kDefaultEventHandler }{ }

Window::Size System::Windows::Window::GetSize() const{

	RECT rect{ 0 };

	if (GetWindowRect(self_id_, &rect)) {

		return Size{ (__STD uint64_t)rect.right - rect.left, (__STD uint64_t)rect.bottom - rect.top };

	}

	throw Exception{ GetLastError() };

}

Window::Pos System::Windows::Window::GetPosition() const{

	RECT rect{ 0 };

	if (GetWindowRect(self_id_, &rect)) {

		return Pos{ (__STD uint64_t)rect.left, (__STD uint64_t)rect.top };

	}

	throw Exception{ GetLastError() };
	
}

__STD uint64_t System::Windows::Window::GetX() const{

	RECT rect{ 0 };

	if (GetWindowRect(self_id_, &rect)) {

		return rect.left;

	}

	throw Exception{ GetLastError() };
}

__STD uint64_t System::Windows::Window::GetY() const{

	RECT rect{ 0 };

	if (GetWindowRect(self_id_, &rect)) {

		return rect.top;

	}

	throw Exception{ GetLastError() };

}

void Window::RegisterWndClass(const __STD wstring_view& class_name) {

	try {

		if (!Core::WndClassRegistered(class_name)) {

			Core::RegisterWndClass(class_name, WndProc);

		}

		class_name_ = class_name;

	}
	catch (const Core::Exception& error) {

		throw Exception{ Exception::Type::ClassRegister, error.GetCode() };

	}

}

void Window::CreateWnd(__STD wstring_view text, Id parent_wnd_id) {

	CreateWnd(kDefaultStyle, kDefaultExStyle, text, kDefaultPosition, kDefaultSize, parent_wnd_id);

}

void Window::CreateWnd(Style style, ExStyle ex_style, Id parent_wnd_id) {

	CreateWnd(style, ex_style, kDefaultText, kDefaultPosition, kDefaultSize, parent_wnd_id);

}

void Window::CreateWnd(Style style, __STD wstring_view text, Id parent_wnd_id) {

	CreateWnd(style, ExStyle::Empty, text, kDefaultPosition, kDefaultSize, parent_wnd_id);

}

void Window::CreateWnd(Style style, __STD wstring_view text, const Position& pos, const Size& size, Id parent_wnd_id) {

	CreateWnd(style, ExStyle::Empty, text, pos, size, parent_wnd_id);

}

void Window::CreateWnd(Style style, ExStyle ex_style, __STD wstring_view text, const Position& pos, const Size& size, Id parent_wnd_id) {

	if (Window::Created())
		throw Exception{ Exception::Type::WindowCreating };

	//Creating new window
	try {

		self_id_ = Core::CreateWnd(
			ex_style,
			class_name_,
			text,
			style,
			size,
			pos,
			parent_wnd_id,
			(__STD uint64_t)this
		);

	}
	catch (const Core::Exception& error) {

		throw Exception{ Exception::Type::WindowCreating, error.GetCode() };

	}

	parent_id_ = parent_wnd_id;

	//Saving pointer of Window object to core window memory
	try {

		Core::SaveToWndMemory(self_id_, (__STD uint64_t)this);

	}
	catch (const Core::Exception& error) {

		Core::DestroyWnd(self_id_);

		self_id_ = NULL;

		throw Exception{ Exception::Type::WindowCreating, error.GetCode() };

	}

}

void Window::ShowWnd() {

	Core::ShowWnd(self_id_);

}

void Window::DestroyWnd() {

	Core::DestroyWnd(self_id_);

}

__forceinline void Window::SetEventHandler(EventHandler handler)noexcept {

	handler_ = handler;

}
Window::~Window() noexcept {

	try {

		Core::DestroyWnd(self_id_);

	}
	catch (Core::Exception) {}

}

Window::Size Window::GetClientSize() const{

	RECT rect{ 0 };

	if (GetClientRect(self_id_, &rect)) {

		return Size{ (__STD uint64_t)rect.right - rect.left, (__STD uint64_t)rect.bottom - rect.top };

	}

	throw Exception{ GetLastError() };
}

__STD uint64_t System::Windows::Window::GetWidth() const{

	RECT rect{ 0 };

	if (GetWindowRect(self_id_, &rect)) {

		return (__STD uint64_t)rect.right - rect.left;

	}

	throw Exception{ Exception::Type::GettingWidth, GetLastError() };
	
}



__STD uint64_t System::Windows::Window::GetHeight() const{
	
	RECT rect{ 0 };

	if (GetWindowRect(self_id_, &rect)) {

		return (__STD uint64_t)rect.bottom - rect.top;

	}

	throw Exception{ Exception::Type::GettingHeight, GetLastError() };
}

__forceinline Window::Id Window::GetId()const noexcept {

	return self_id_;

}

__forceinline bool Window::Created() const noexcept {

	return self_id_;
}

__forceinline bool Window::CallEventHandler(const IEvent* event)noexcept {

	return handler_(event);
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM  wParam, LPARAM lParam) {

	static EventBuilder* builder = nullptr;

	if (builder == nullptr) {

		builder = EventBuilder::GetInstance();

	}

	IEvent* event = builder->ConstructEvent(hWnd, message, wParam, lParam);

	const IEvent::Source source = event->GetSource();
	const IEvent::Action action = event->GetAction();

	if (action == IEvent::Action::Created) {

		auto obj_ptr = (Core::WndCreateData*)event->GetCreateDataPointer();

		try {

			Core::SaveToWndMemory(hWnd, obj_ptr->GetAdditionalData());

		}
		catch (const Core::Exception& error) {

			exit(error.GetCode());

		}

	}

	bool event_processed = false;

	switch (source) {
	case IEvent::Source::Window: {

		try {

			auto window = (Window*)Core::GetWndMemory(hWnd);

			event_processed = window->CallEventHandler(event);

		}
		catch (Core::Exception) {}

		break;
	}

	case IEvent::Source::Control: {

		try {

			auto control = (Control*)Core::GetWndMemory((Control::Id)lParam);

			event_processed = control->CallEventHandler(event);

		}
		catch (Core::Exception) {}

		break;
	}
	};

	if (event_processed) return 0;

	switch (action) {

	case IEvent::Action::Close:
		Core::DestroyWnd(hWnd);
		break;

	case IEvent::Action::CloseAppplication:
		Core::QuitFromMainLoop(0);
		break;

	}

	return DefWindowProc(hWnd, message, wParam, lParam);

}

Window::Exception::Exception(Type type, ErrorCode code)noexcept :
	type_{ type },
	code_{ code } { }

Window::Exception::Exception(Type type)noexcept :
	Exception::Exception{ type, NULL } { }

Window::Exception::Exception(ErrorCode code)noexcept :
	Exception::Exception{ Type::Empty , code } { }

__forceinline Window::Exception::ErrorCode Window::Exception::GetErrorCode()const noexcept {

	return code_;

}

const __STD string_view& Window::Exception::What()const noexcept {

	return messages_[to_basic(type_)];

}