#include "Core.hpp"

using namespace System;

Core::Exception::Exception(ErrorCode code)noexcept :
	code_{ code } { }

__forceinline Core::Exception::ErrorCode Core::Exception::GetCode()const noexcept {

	return code_;
}

Core::Size::Size(__STD uint64_t width, __STD uint64_t height)noexcept :
	width_{ width }, height_{ height }{  }

__forceinline __STD uint64_t Core::Size::Width()const noexcept { return width_; }
__forceinline __STD uint64_t Core::Size::Height()const noexcept { return height_; }

Core::Position::Position(__STD uint64_t x, __STD uint64_t y)noexcept :
	x_{ x }, y_{ y }{  }

__forceinline __STD uint64_t  Core::Position::X()const noexcept { return x_; }
__forceinline __STD uint64_t  Core::Position::Y()const noexcept { return y_; }