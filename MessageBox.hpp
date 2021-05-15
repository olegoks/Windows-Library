#pragma once

#include "Window.hpp"

#undef MessageBox
namespace System {

	class EXPIMP MessageBox final {
	public:

		using Style = UINT;

		enum class Type : Style {

			//The message box contains three push buttons : Cancel, Try Again, Continue.
			CancelTryAgainContinue = 0x00000006L,//MB_CANCELTRYCONTINUE,

			//Adds a Help button to the message box.When the user clicks the Help button or presses F1, the system sends a WM_HELP message to the owner.
			Help = 0x00004000L,//MB_HELP,

			//The message box contains one push button : OK.This is the default.
			Ok = 0x00000000L,//MB_OK,

			//The message box contains two push buttons : OK and Cancel.
			OkCancel = 0x00000001L,//MB_OKCANCEL,
			RetryCancel = 0x00000005L,//MB_RETRYCANCEL,
			YesNo = 0x00000004L,//MB_YESNO,
			YesNoCancel = 0x00000003L,//MB_YESNOCANCEL

		};

		enum class Icon : Style {

			//An exclamation - point icon appears in the message box.
			Exclamation = 0x00000030L,//MB_ICONEXCLAMATION,
			Warning = 0x00000030L,//MB_ICONWARNING,
			Information = 0x00000040L,//MB_ICONINFORMATION,
			Asterisk = 0x00000040L,//MB_ICONASTERISK,
			Question = 0x00000020L//MB_ICONQUESTION

		};

		enum class Result : int {

			//The Abort button was selected.
			AbortSelected = 3,//IDABORT,

			//The Cancel button was selected.
			CancelSelected = 2,//IDCANCEL,

			//The Continue button was selected.
			ContinueSelected = 11,//IDCONTINUE,
			IgnoreSelected = 5, //IDIGNORE,
			NoSelected = 7,//IDNO,
			YesSelected = 6,//IDYES,
			OkSelected = 1,//IDOK,
			RetrySelected = 4,//IDRETRY,
			TryAgainSelected = 10//IDTRYAGAIN

		};

		static Result Show(Window::Id parent_id, __STD string_view text, __STD string_view caption, Style style);
		static Result Show(Window::Id parent_id, __STD string_view text, __STD string_view caption, Type type);

	};

	using MsgBox = MessageBox;

	constexpr EXPIMP MsgBox::Style operator|(MsgBox::Type style, MsgBox::Icon icon)noexcept;
	constexpr EXPIMP MsgBox::Style operator|(MsgBox::Icon icon, MsgBox::Type style)noexcept;

}