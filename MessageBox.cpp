
#include "MessageBox.hpp"
#include "Window.hpp"

namespace System {

	MessageBox::Result MessageBox::Show(Window::Id parent_id, __STD string_view text, __STD string_view caption, Style style) {

		const int result = MessageBoxA((HWND)parent_id, text.data(), caption.data(), style);

		if (!result) {
		
			throw Exception{ GetLastError() };

		}

		return to_enum_type<Result>(result);

	};

	MessageBox::Result MessageBox::Show(Window::Id parent_id, __STD string_view text, __STD string_view caption, Type type) {

		return Show(parent_id, text, caption, to_enum_type<Style>(type));

	}

	constexpr MsgBox::Style operator|(MsgBox::Type style, MsgBox::Icon icon)noexcept {

		return System::to_enum_type<MsgBox::Style>(to_basic(style) | to_basic(icon));

	}

	constexpr  MsgBox::Style operator|(MsgBox::Icon icon, MsgBox::Type style)noexcept {

		return System::to_enum_type<MsgBox::Style>(to_basic(style) | to_basic(icon));

	}

}
