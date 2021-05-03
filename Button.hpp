#pragma once

#include "Control.hpp"

namespace System::Windows {

	class EXPIMP Button final : public Control{
	private:

	public:

		using Pos = Window::Pos;
		using Size = Window::Size;

		using Style = Control::Style;

		explicit Button(const __STD wstring_view& text)noexcept :
			Control::Control{ Type::Button, text }{}

	};

}