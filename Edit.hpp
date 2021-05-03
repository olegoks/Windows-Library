#pragma once

#include "Control.hpp"

using namespace System::Windows;

class Edit final : public Control {
private:
public:

	explicit Edit(const __STD wstring_view text)noexcept:
		Control::Control{ Control::Type::Edit, text }{}

	//Symbols number includes 0
	__STD wstring GetText(__STD uint64_t max_symbols_number)const {

		try {
		
			auto buffer = __STD make_unique<wchar_t[]>(max_symbols_number);
			
			const int read_symbols_number = GetWindowTextW(Control::GetId(), buffer.get(), max_symbols_number);

			if (read_symbols_number) {

				return __STD wstring{ buffer.get(), (__STD uint64_t )read_symbols_number };

			}
			//Error or empty string
			else {

				const DWORD error_code = GetLastError();

				//Empty string -> error_code == NULL
				if(!error_code){
					
					return __STD wstring{};

				}

				throw Exception{ error_code };

			}


		} catch (const __STD bad_alloc& error) {

			throw Exception{ Exception::Type::NotEnougthMemory};

		}

	}

};