#pragma once

#include "Control.hpp"

namespace System {

	class EXPIMP Button : public Control {
	public:

		enum class Style : StyleBasicType {

			PushButton = BS_PUSHBUTTON,
			CheckBox = BS_CHECKBOX

		};

		explicit Button(const __STD wstring_view& text)noexcept;

		void SetEventHandler(EventHandler handler)noexcept;

	};

	class EXPIMP Edit : public Control {
	public:

		enum class Style : StyleBasicType {

			//Edit styles
			OnlyNumbers = ES_NUMBER,
			LeftTextAlign = ES_LEFT,
			RightTextAlign = ES_RIGHT,
			Lowercase = ES_LOWERCASE,
			Multiline = ES_MULTILINE,
			Password = ES_PASSWORD,
			Uppercase = ES_UPPERCASE,
			ReadOnly = ES_READONLY

		};

		explicit Edit(const __STD wstring_view text)noexcept;

	};

	class EXPIMP ProgressBar : public Control {
	public:

		enum class Style : StyleBasicType {

			Green = PBS_MARQUEE,
			Smooth = PBS_SMOOTH

		};

		explicit ProgressBar()noexcept;
		void SetNewProgressPosition(__STD uint64_t new_pos)noexcept;
		void StartWaittingMode()noexcept;
		void EndWaittingMode()noexcept;

	};

	class EXPIMP Static : public Control {
	public:

		enum class Style : StyleBasicType {

			

		};

		explicit Static(const __STD wstring_view& text)noexcept;

	};



}