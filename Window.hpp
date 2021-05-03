#pragma once

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <functional>
#include <string>
#include <string_view>

#include "Core.hpp"
#include "Pixel.hpp"

class IEvent;

extern class EXPIMP Pixel;

namespace System::Windows {

	class EXPIMP Window {
	public:

		class EXPIMP Exception final : public __STD exception{
		public:

			using ErrorCode = DWORD;

		public:

			enum class Type : __STD uint64_t {

				WindowCreating = 0,
				ClassRegister,
				GettingWidth,
				GettingHeight,
				GettingClientSize,
				GettingClientPosition,
				NotEnougthMemory,
				Empty
			};

		private:

			Type type_;
			ErrorCode code_;

		public:

			static const inline __STD string_view messages_[] = {

				u8"Creating window error.",
				u8"Register class error.",
				u8"Getting window width.",
				u8"Getting window height.",
				u8"Getting client size.",
				u8"Getting client position.",
				u8"Not enougth memory."

			};

			explicit Exception(Type type, ErrorCode code)noexcept;
			explicit Exception(Type type)noexcept;
			explicit Exception(ErrorCode code)noexcept;

			ErrorCode GetErrorCode()const noexcept;
			const __STD string_view& What()const noexcept;

		};

		//If your handler process event: return true else false
		using EventHandler = __STD function<bool(const IEvent* event)>;

		using Style = Core::Style;
		using ExStyle = Core::ExStyle;
		using Position = Core::Position;
		using Pos = Core::Pos;
		using Size = Core::Size;
		using Id = Core::WindowId;

		static inline const __STD wstring_view	kDefaultText = L"";
		static inline const Size				kDefaultSize{ Core::ScreenWidth() / 2, Core::ScreenHeight() / 2 };
		static inline const Position			kDefaultPosition{ 0, 0 };
		static inline const Style				kDefaultStyle = Style::Empty;
		static inline const ExStyle				kDefaultExStyle = ExStyle::Empty;
		static inline const __STD wstring_view	kDefaultClassName = L"Component";
		static inline const Color				kDefaultBackgroundColor{ 255, 255, 255 };
		static inline const EventHandler		kDefaultEventHandler = [](const IEvent* event)noexcept->bool { return false; };

	private:

		Id self_id_;
		Id parent_id_;
		__STD wstring_view class_name_;
		EventHandler handler_;

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM  wParam, LPARAM lParam);
		bool CallEventHandler(const IEvent* event)noexcept;

	protected:

		void SetEventHandler(EventHandler handler)noexcept;

		void RegisterWndClass(const __STD wstring_view& class_name);

		void CreateWnd(Style style, ExStyle ex_style, __STD wstring_view text, const Position& pos, const Size& size, Id parent_wnd_id = NULL);
		void CreateWnd(__STD wstring_view text, Id parent_wnd_id = NULL);
		void CreateWnd(Style style, ExStyle ex_style, Id parent_wnd_id = NULL);
		void CreateWnd(Style style, __STD wstring_view text, Id parent_wnd_id = NULL);
		void CreateWnd(Style style, __STD wstring_view text, const Position& pos, const Size& size, Id parent_wnd_id);

		void ShowWnd();
		void DestroyWnd();

	public:

		explicit Window()noexcept;

		Size GetSize()const;
		Pos GetPosition()const;
		__STD uint64_t GetX()const;
		__STD uint64_t GetY()const;
		Size GetClientSize()const;
		__STD uint64_t GetWidth()const;
		__STD uint64_t GetHeight()const;

		Id GetId()const noexcept;
		bool Created()const noexcept;

		virtual void Create(Id parent_id = NULL) = 0;
		virtual void Show() = 0;
		virtual void Destroy() = 0;

		virtual ~Window()noexcept;

	};

	constexpr Window::Style operator|(Window::Style first, Window::Style second)noexcept {

		return to_enum_type<Window::Style>(to_basic(first) | to_basic(second));

	};

};

#endif //WINDOW_HPP