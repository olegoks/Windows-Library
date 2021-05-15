#include "Control.hpp"
#include "HelpFunctions.hpp"

namespace System {

	Control* Control::Event::GetControl() const noexcept {

		return reinterpret_cast<Control*>(GetWindowAttribute((Window::Id)lParam_, WindowAttribute::UserData));

	}

	void Control::SetEventHandler(EventHandler handler) noexcept {

		handler_ = handler;

		Window::SetEventHandler([this](const Window::Event& event)noexcept->bool {



			return Control::handler_(*reinterpret_cast<const Event*>(&event));

			});

	}

	Control::Control(Type type, const __STD wstring_view& text) noexcept :
		type_{ type }, text_{ text } { }

	void Control::Create(Id parent_id) {

		Control::Create(parent_id, Style::Empty, Position{ Window::kDefaultPosition }, Size{ kDefaultSize });

	}

	void Control::Show() {

		Window::ShowWnd();

	}

	void Control::Destroy() {

		Window::DestroyWnd();

	}

	void Control::Create(Id parent_id, Style style, const Position& pos, const Size& size) {

		Window::RegisterWndClass(controls_classes_[System::to_basic(type_)]);

		Window::CreateWnd(style | Style::Child, text_.data(), pos, size, parent_id);

	}

}