#pragma once

#include "Window.hpp"
#include "Control.hpp"
#include "Event.hpp"

union EventMemory {

	System::Window::Event window_event_;
	System::Control::Event control_event_;

	~EventMemory()noexcept {}
};

class EventBuilder final {
private:

	explicit EventBuilder()noexcept {}

public:

	static EventBuilder& GetInstance() {

		static EventBuilder* builder = nullptr;

		if (builder == nullptr) {

			builder = new EventBuilder{};

		}

		return *builder;

	}

	System::Window::Event& ConstructEvent(HWND hWnd, UINT Message, WPARAM wparam, LPARAM lparam) {

		static char event_memory[sizeof(EventMemory)] = { 0 };

		if ((Message == WM_COMMAND) && lparam) {

			new (event_memory)System::Control::Event{ hWnd, Message, wparam, lparam };

		}
		else if ((Message >= 0 && Message <= 173) || (Message >= 255 && Message <= 911)) {

			new (event_memory) System::Window::Event{ hWnd, Message, wparam, lparam };

		}

		return *(System::Window::Event*)event_memory;

	}

};