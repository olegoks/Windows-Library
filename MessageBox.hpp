#pragma once

#include "Basic.hpp"
#include "Window.hpp"

namespace System::Windows {

		class MsgBox final {
		private:
		public:

			class Exception final : __STD exception {
			public:

				using ErrorCode = DWORD;

			private:

				ErrorCode code_;

			public:

				explicit Exception(ErrorCode code)noexcept :
					code_{ code } {  }

				__forceinline ErrorCode GetCode()const noexcept {

					return code_;

				}

			};

			enum class Style : UINT {};

			enum class Type : UINT {

				//The message box contains three push buttons : Cancel, Try Again, Continue.
				CancelTryAgainContinue = MB_CANCELTRYCONTINUE,

				//Adds a Help button to the message box.When the user clicks the Help button or presses F1, the system sends a WM_HELP message to the owner.
				Help = MB_HELP,

				//The message box contains one push button : OK.This is the default.
				Ok = MB_OK,

				//The message box contains two push buttons : OK and Cancel.
				OkCancel = MB_OKCANCEL,
				RetryCancel = MB_RETRYCANCEL,
				YesNo = MB_YESNO,
				YesNoCancel = MB_YESNOCANCEL

			};

			enum class Icon : UINT {

				//An exclamation - point icon appears in the message box.
				Exclamation = MB_ICONEXCLAMATION,
				Warning = MB_ICONWARNING,
				Information = MB_ICONINFORMATION,
				Asterisk = MB_ICONASTERISK,
				Question = MB_ICONQUESTION

			};

			enum class Result : int {

				//The Abort button was selected.
				AbortSelected = IDABORT,

				//The Cancel button was selected.
				CancelSelected = IDCANCEL,

				//The Continue button was selected.
				ContinueSelected = IDCONTINUE,
				IgnoreSelected = IDIGNORE,
				NoSelected = IDNO,
				YesSelected = IDYES,
				OkSelected = IDOK,
				RetrySelected = IDRETRY,
				TryAgainSelected = IDTRYAGAIN


			};

			static Result Show(Window::Id parent_id, __STD string_view text, __STD string_view caption, Style style)noexcept {

				const int result = MessageBoxA(parent_id, text.data(), caption.data(), to_basic(style));

				if (!result) {

					throw Exception{ GetLastError() };

				}

				return to_enum_type<Result>(result);

			};

			__forceinline static Result Show(Window::Id parent_id, __STD string_view text, __STD string_view caption, Type type)noexcept {
				
				return Show(parent_id, text, caption, to_enum_type<Style>(type));

			}

		};


		constexpr MsgBox::Style operator|(MsgBox::Type style, MsgBox::Icon icon)noexcept {

			return to_enum_type<MsgBox::Style>(to_basic(style) | to_basic(icon));

		}

		constexpr  MsgBox::Style operator|(MsgBox::Icon icon, MsgBox::Type style)noexcept {

			return to_enum_type< MsgBox::Style>(to_basic(style) | to_basic(icon));

		}

}