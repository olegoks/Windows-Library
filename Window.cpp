
#include "Window.hpp"
#include "EventBuilder.hpp"
#include "HelpFunctions.hpp"

namespace System {

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM  wParam, LPARAM lParam);

	Size Window::GetWndSizeWithThisClientSize(const Size& size, Style style, Style ex_style, bool menu){

		RECT rect{ 0, 0, (LONG)size.GetWidth(), (LONG)size.GetHeight() };

		if (AdjustWindowRectEx(&rect, to_basic(style), menu, to_basic(ex_style))) {

			return Size{ (__STD uint64_t)rect.right - rect.left, (__STD uint64_t)rect.bottom - rect.top };

		}

		throw Exception{ GetLastError() };

	}

	bool Window::CallEventHandler(const Event& event) noexcept {

		return handler_(event);

	}

	Window::Window() noexcept :
		self_id_{ NULL },
		parent_id_{ NULL },
		class_name_{ kDefaultClassName },
		handler_{ kDefaultEventHandler }{ }

	void Window::ShowRGBAImage(Pixel* image, Size size){

		BITMAPINFO info{ 0 };
		info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		info.bmiHeader.biWidth = (int)size.GetWidth();
		info.bmiHeader.biHeight = /*-*/(static_cast<int>(size.GetHeight()));
		info.bmiHeader.biPlanes = 1;
		info.bmiHeader.biBitCount = 32;
		info.bmiHeader.biCompression = BI_RGB;
		info.bmiHeader.biSizeImage = (DWORD)(((DWORD)size.GetWidth() * 24 + 31) & ~31) / 8 * (DWORD)size.GetHeight();

		if (!SetDIBitsToDevice(
			(HDC)dc_,
			0,
			0,
			(int)GetWidth(),
			(int)GetHeight(),
			0,
			0,
			0,
			(int)GetHeight(),
			image,
			&info,
			DIB_PAL_COLORS
		)) throw Exception{ GetLastError() };

	}

	bool Window::Enable() noexcept {

		bool previos_state = EnableWindow((HWND)self_id_, true);
		UpdateWindow((HWND)self_id_);

		return previos_state;

	}

	bool Window::Disable() noexcept {

		bool previos_state = EnableWindow((HWND)self_id_, false);
		UpdateWindow((HWND)self_id_);

		return previos_state;

	}

	Size Window::GetSize() const {

		RECT rect{ 0 };

		if (GetWindowRect(self_id_, &rect)) {

			return Size{ (__STD uint64_t)rect.right - rect.left, (__STD uint64_t)rect.bottom - rect.top };

		}

		throw Exception{ GetLastError() };

	}

	Position Window::GetPosition() const {

		RECT rect{ 0 };

		if (GetWindowRect(self_id_, &rect)) {

			return Position{ (__STD uint64_t)rect.left, (__STD uint64_t)rect.top };

		}

		throw Exception{ GetLastError() };

	}

	__STD uint64_t Window::GetX() const {

		return Window::GetPosition().GetX();

	}

	__STD uint64_t Window::GetY() const {

		return Window::GetPosition().GetY();

	}

	void Window::SetEventHandler(EventHandler handler) noexcept{
		
		handler_ = handler;

	}

	void Window::RegisterWndClass(const __STD wstring_view& class_name) {

		WNDCLASSEX wc;

		wc.cbSize = sizeof(wc);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = sizeof(__STD uint64_t);
		wc.hInstance = (HINSTANCE)GetModuleHandle(NULL);
		wc.hIcon = NULL;//LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = class_name.data();
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		if (!RegisterClassEx(&wc))
			throw Exception{ GetLastError() };

	}

	void Window::CreateWnd(__STD wstring_view text, Id parent_wnd_id) {

		CreateWnd(kDefaultStyle, kDefaultExStyle, text, kDefaultPosition, kDefaultSize, parent_wnd_id);

	}

	void Window::CreateWnd(Style style, ExStyle ex_style, Id parent_wnd_id) {

		CreateWnd(style, ex_style, kDefaultText, kDefaultPosition, kDefaultSize, parent_wnd_id);

	}

	void Window::CreateWnd(Style style, __STD wstring_view text, Id parent_wnd_id) {

		CreateWnd(style, ExStyle::Empty, text, kDefaultPosition, kDefaultSize, parent_wnd_id);

	}

	void Window::CreateWnd(Style style, __STD wstring_view text, const Position& pos, const Size& size, Id parent_wnd_id) {

		CreateWnd(style, ExStyle::Empty, text, pos, size, parent_wnd_id);

	}

	void Window::CreateWnd(Style style, ExStyle ex_style, __STD wstring_view text, const Position& pos, const Size& size, Id parent_wnd_id) {

		self_id_ = CreateWindowEx(to_basic(ex_style),
			class_name_.data(),
			text.data(),
			to_basic(style),
			(int)pos.GetX(),
			(int)pos.GetY(),
			(int)size.GetWidth(),
			(int)size.GetHeight(),
			(HWND)parent_wnd_id,
			(HMENU)NULL,
			(HINSTANCE)GetModuleHandle(NULL),
			(LPVOID)this);

		if (!self_id_) { throw Exception{ GetLastError() }; }

		dc_ = GetDC(self_id_);

		if (!dc_) { 
			
			Window::DestroyWnd();

			throw Exception{ GetLastError() }; 
		
		}

		//Saving pointer of Window object to core window memory
		try {

			ChangeWindowAttribute(self_id_, WindowAttribute::UserData, (__STD int64_t)this);

		} catch (const Exception& error) {

			Window::DestroyWnd();

			throw Exception{ error.GetErrorCode() };

		}

		parent_id_ = parent_wnd_id;

	}

	void Window::ShowWnd() {

		ShowWindow(self_id_, SW_SHOW);

	}

	void Window::DestroyWnd() {

		if (!DestroyWindow(self_id_)) {

			throw Exception{ GetLastError() };

		}

		self_id_ = NULL;
		DeleteDC(dc_);

	}

	Window::~Window() noexcept {

		try {

			if (Window::Created()) {

				DestroyWnd();
			
			}

		} catch (Exception) {}

	}

	Size Window::GetClientSize() const {

		RECT rect{ 0 };

		if (GetClientRect(self_id_, &rect)) {

			return Size{ (__STD uint64_t)rect.right - rect.left, (__STD uint64_t)rect.bottom - rect.top };

		}

		throw Exception{ GetLastError() };

	}

	__STD uint64_t Window::GetWidth() const {

		return Window::GetSize().GetWidth();

	}

	__STD uint64_t Window::GetHeight() const {

		return Window::GetSize().GetHeight();

	}

	__STD wstring Window::GetText() const {

		try {

			const __STD uint64_t text_length = GetWindowTextLengthW((HWND)Window::GetId());

			if (!text_length)return __STD wstring{ L"" };

			auto buffer = __STD make_unique<wchar_t[]>(text_length + 1);

			const int read_symbols_number = GetWindowTextW((HWND)GetId(), buffer.get(), (int)text_length + 1);

			if (read_symbols_number) {

				return __STD wstring{ buffer.get() };

			}
			//Error or empty string
			else {

				const DWORD error_code = GetLastError();

				//Empty string -> error_code == NULL
				if (!error_code) {

					return __STD wstring{};

				}

				throw Exception{ error_code };

			}


		} catch (const __STD bad_alloc&) {

			throw Exception{ ERROR_NOT_ENOUGH_MEMORY };

		}

	}

	void Window::SetText(__STD wstring_view text){

		if (!SetWindowTextW((HWND)Window::GetId(), text.data()) ){

			throw Exception{ GetLastError() };

		}
	
	}

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM  wParam, LPARAM lParam) {

		static EventBuilder& builder = EventBuilder::GetInstance();

		const Window::Event& event = builder.ConstructEvent((HWND)hWnd, message, wParam, lParam);

		const Window::Event::Action action = event.GetAction();

		if (action == Window::Event::Action::Created) {

			auto obj_ptr = (Window*)((CREATESTRUCT*)lParam)->lpCreateParams;

			try {

				ChangeWindowAttribute(hWnd, WindowAttribute::UserData, (__STD int64_t)obj_ptr);

			} catch (const System::Exception& error) {

				exit(error.GetErrorCode());

			}

		}

		const IEvent::Source source = event.GetSource();
		Window::Id window_id = NULL;
		
		if (source == IEvent::Source::Window) {

			window_id = hWnd;

		//Control window
		} else {

			window_id = (Window::Id)lParam;

		}

		try {
			
			auto window = (Window*)GetWindowAttribute(hWnd, WindowAttribute::UserData);
		
			const bool event_processed = window->CallEventHandler(event);

			if (event_processed) return 0;

			switch (action) {

			case Window::Event::Action::Close:
				window->Destroy();
				break;

			case Window::Event::Action::CloseAppplication:
				Core::QuitFromMainLoop(0);
				break;

			}

		}
		catch (const System::Exception& error) {

			if (action == Window::Event::Action::Created || action == Window::Event::Action::Close) {

				throw Exception{ error.GetErrorCode() };

			}

		};

		return DefWindowProcW((HWND)hWnd, message, wParam, lParam);

	}

	Exception::Exception(ErrorCode code)noexcept :
		exception::exception{ },
		code_{ code } { }

	Exception::ErrorCode Exception::GetErrorCode()const noexcept { 
		
		return code_; 
	
	}

	__STD uint64_t Window::Event::GetMouseX() const noexcept {

		return LOWORD(lParam_);

	}

	__STD uint64_t Window::Event::GetMouseY() const noexcept{

		return HIWORD(lParam_);
	
	}

	Window& Window::Event::GetWindow() const noexcept {

		return *reinterpret_cast<Window*>(GetWindowAttribute(hWnd_, WindowAttribute::UserData));

	}

	Window::Event::Action Window::Event::GetAction()const noexcept { 
		
		return System::to_enum_type<Action>(message_); 
	
	}

};