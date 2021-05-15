
#include "HelpFunctions.hpp"

namespace System {

	void ChangeWindowAttribute(Window::Id wnd_id, WindowAttribute attribute, __STD int64_t new_value) {

		SetLastError(NULL);

		const LONG_PTR prev_value = SetWindowLongPtrW(wnd_id, to_basic(attribute), new_value);

		//prev_value == zero -> error or last value == zero
		if (!prev_value) {

			const DWORD failure = GetLastError();

			if (failure) {

				throw Exception{ failure };

			}

		}

	}

	__STD int64_t GetWindowAttribute(Window::Id wnd_id, WindowAttribute attribute) {

		auto data = (__STD int64_t)GetWindowLongPtr(wnd_id, GWLP_USERDATA);

		if (!data) {

			throw Exception{ GetLastError() };

		}

		return data;

	}

}