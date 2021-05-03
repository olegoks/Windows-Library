#pragma once

#include "Window.hpp"

class ControlEvent;

namespace System::Windows {

	class EXPIMP Control : public Window {
	public:

		using EventHandler = __STD function<bool(const ControlEvent& event)>;

		enum class Type : __STD uint64_t {

			Button,
			Edit,
			Static,
			ProgressBar

		};

		using Id = Window::Id;

	private:

		Type type_;
		
		__STD wstring_view text_;

		EventHandler handler_;

		static const inline __STD wstring_view controls_classes_[] = {
			
			L"Button",
			L"Edit",
			L"Static",
			L"msctls_progress32"

		};

	public:

		__forceinline void SetEventHandler(EventHandler handler)noexcept {

			handler_ = handler;

			Window::SetEventHandler([this](const IEvent* event)noexcept->bool {
				
				return Control::handler_(*reinterpret_cast<const ControlEvent*>(event));
				
				});

		}

		static const inline __STD uint64_t kDefaultX_ = 0;
		static const inline __STD uint64_t kDefaultY_ = 0;
		static const inline __STD uint64_t kDefaultWidth_ = Core::ScreenWidth() / 5;
		static const inline __STD uint64_t kDefaultHeight_ = Core::ScreenHeight() / 10;

		explicit Control(Type type, const __STD wstring_view& text)noexcept :
			type_{ type }, text_{ text } { }

		void Create(Id parent_id, Style style, const Position& pos, const Size& size) {

			Window::RegisterWndClass(controls_classes_[to_basic(type_)]);

			Window::CreateWnd(Style::Child|style, text_.data(), pos, size, parent_id);

		}
			
		void Create(Id parent_id = NULL)override {

			Control::Create(parent_id, Style::Empty, Position{ kDefaultX_, kDefaultY_ }, Size{  kDefaultWidth_, kDefaultHeight_ });

		}

		void Show()override {

			Window::ShowWnd();

		}

		void Destroy()override {

			Window::DestroyWnd();

		}

	};

}