#pragma once

#include "Control.hpp"


namespace System::Windows {

	class ProgressBar final : public Control {
	public:

		using Pos = Window::Pos;
		using Size = Window::Size;

		using Style = Control::Style;

		explicit ProgressBar()noexcept :
			Control::Control{ Type::ProgressBar, L"" }{}

		void SetNewPosition(__STD uint64_t new_pos)noexcept {

			SendMessage(Control::GetId(), PBM_SETPOS, new_pos,0);

		}

		void StartWaittingMode()noexcept {

			SendMessage(Control::GetId(), PBM_SETMARQUEE, true, 0);

		}

		void EndWaittingMode()noexcept {

			SendMessage(Control::GetId(), PBM_SETMARQUEE, false, 0);

		}

	};

}