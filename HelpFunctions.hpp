#pragma once

#include "Window.hpp"

namespace System {

	enum class WindowAttribute : int {

		ExStyle = GWL_EXSTYLE,
		Style = GWL_STYLE,
		UserData = GWLP_USERDATA,
		Handler = GWLP_WNDPROC

	};

	void ChangeWindowAttribute(Window::Id wnd_id, WindowAttribute attribute, __STD int64_t value);
	__STD int64_t GetWindowAttribute(Window::Id wnd_id, WindowAttribute attribute);

}