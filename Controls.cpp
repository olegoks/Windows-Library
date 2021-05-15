
#include "Controls.hpp"

System::Button::Button(const __STD wstring_view& text) noexcept :
	Control::Control{ Type::Button, text }{}

void System::Button::SetEventHandler(EventHandler handler) noexcept {

	Control::SetEventHandler([handler](const Control::Event& event)noexcept->bool {

		return handler(event);

		});

}

System::Edit::Edit(const __STD wstring_view text) noexcept :
	Control::Control{ Control::Type::Edit, text }{}

System::ProgressBar::ProgressBar() noexcept :
	Control::Control{ Type::ProgressBar, L"" }{}

void System::ProgressBar::SetNewProgressPosition(__STD uint64_t new_pos) noexcept {

	SendMessage((HWND)Control::GetId(), PBM_SETPOS, new_pos, 0);

}

void System::ProgressBar::StartWaittingMode() noexcept {

	SendMessage((HWND)Control::GetId(), PBM_SETMARQUEE, true, 0);

}

void System::ProgressBar::EndWaittingMode() noexcept {

	SendMessage((HWND)Control::GetId(), PBM_SETMARQUEE, false, 0);

}

System::Static::Static(const __STD wstring_view& text) noexcept :
	Control::Control{ Type::Static, text }{}
