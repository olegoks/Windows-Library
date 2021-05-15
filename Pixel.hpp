
#pragma once
#ifndef PIXEL_HPP
#define PIXEL_HPP

#include <cstdint>

#define COMPILEDLL

#ifdef COMPILEDLL
#define EXPIMP __declspec(dllexport)
#else 
#define EXPIMP __declspec(dllimport)
#endif

#define __STD ::std::

class EXPIMP Pixel  {
public:

	union rgba {

		__STD uint8_t bgra[4];

		struct {

			__STD uint8_t b;
			__STD uint8_t g;
			__STD uint8_t r;
			__STD uint8_t a;

		};

	};

	rgba rgba_ = { 0 };

	constexpr explicit Pixel(__STD uint8_t r, __STD uint8_t g, __STD uint8_t b)noexcept;

	explicit Pixel()noexcept;

	explicit Pixel(const Pixel& copy_pixel) = default;
	explicit Pixel(Pixel&& move_pixel)noexcept;

	Pixel& operator=(Pixel&& move_pixel)noexcept;
	Pixel& operator=(const Pixel& copy_pixel) = default;

	~Pixel()noexcept = default;

};

using Color = Pixel;
using RGBA = Color;
using RGB = RGBA;

#endif //PIXEL_HPP