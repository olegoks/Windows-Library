#pragma once

#include <Windows.h>
#include <Commctrl.h>
#include <wininet.h>
#define __STD ::std::

#define COMPILING_DLL

#ifdef COMPILING_DLL
#define EXPIMP __declspec(dllexport)
#else
#define EXPIMP __declspec(dllimport)
#endif

#pragma warning(disable:4251)
#pragma warning(disable:4275)
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <type_traits>

//Convert enum type to basic type
template<class EnumClass>
constexpr __STD underlying_type_t<EnumClass> to_basic(EnumClass e)noexcept {

	return static_cast<__STD underlying_type_t<EnumClass>>(e);

}

template<class EnumClass, class BasicType>
constexpr EnumClass to_enum_type(BasicType value)noexcept {

	return static_cast<EnumClass>(value);

}