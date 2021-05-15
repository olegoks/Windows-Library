#pragma once

#include "Window.hpp"


namespace System {

	class EXPIMP Control : public Window {
	public:

		class EXPIMP Event : public Window::Event {
		public:

			enum class Action : unsigned int {

				//Button actions
				ButtonMouseLeaveEnter = BCN_HOTITEMCHANGE,
				ButtonClicked = BN_CLICKED,
				ButtonDoubleClicked = BN_DOUBLECLICKED,
				ButtonLostKeyboardFocus = BN_KILLFOCUS,
				ButtonGetKeyboardFocus = BN_SETFOCUS,
				ButtonReleased = BN_UNPUSHED

				//Edit actions

			};

			explicit Event(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)noexcept :
				Window::Event::Event(Source::Control, hWnd, message, wParam, lParam) {}

			Control* GetControl()const noexcept;

			__forceinline Action GetAction()const noexcept {

				return System::to_enum_type<Action>(HIWORD(wParam_));

			}

		};

		using EventHandler = __STD function<bool(const Event& event)>;

		enum class Type : __STD uint64_t {

			Button,
			Edit,
			Static,
			ProgressBar

		};

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

		void SetEventHandler(EventHandler handler)noexcept;

		static const inline Size kDefaultSize{ Core::GetScreenWidth() / 5, Core::GetScreenHeight() / 10 };

		explicit Control(Type type, const __STD wstring_view& text)noexcept;
		void Create(Id parent_id, Style style, const Position& pos, const Size& size);
		void Create(Id parent_id = NULL)override;
		void Show()override;
		void Destroy()override;

	};

};