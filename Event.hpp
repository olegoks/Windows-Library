#pragma once

#ifndef EVENT_HPP
#define EVENT_HPP

#include <cstdint>
#include <type_traits>
#include <memory>

class IEvent {
public:

	enum class Source : __STD uint64_t {

		Window,
		Control,
		Empty

	};

private:

	Source source_;

protected:

	HWND hWnd_;
	unsigned int message_;
	WPARAM wParam_;
	LPARAM lParam_;

public:

	explicit IEvent(Source source, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)noexcept :
		source_{ source },
		hWnd_{ hWnd },
		message_{ message },
		wParam_{ wParam },
		lParam_{ lParam }{ }

	Source GetSource()const noexcept { return source_; };

	virtual ~IEvent()noexcept = default;

};

#endif //EVENT_HPP