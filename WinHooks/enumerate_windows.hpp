#ifndef _ENUMERATE_WINDOWS_HPP_
#define _ENUMERATE_WINDOWS_HPP_


#define _LOG_ENUM_WINDOW
#undef _LOG_ENUM_WINDOW


#include "pch.h"

#include "hooks_headers.h"


using namespace std;

namespace amped::win
{

namespace exec = amped::win::execute;

namespace en = amped::win::enumerate;

const wstring c_ableton_class_name{ L"Ableton Live Window Class" };

class enumerate_windows
{
public:
	explicit enumerate_windows();

	explicit enumerate_windows(const string_view name);

	~enumerate_windows();

	void operator()();

	en::handle_window_t operator[](const string_view str) noexcept;

	en::handle_window_t operator[](const wstring_view str) noexcept;

	en::handle_window_t operator[](const tuple<string_view, string_view>& t_str) noexcept;

	en::handle_window_t operator[](const tuple<wstring_view, wstring_view>& t_str) noexcept;
protected:
	void enumerate();
private:
	string_view m_name_window;
	en::handle_window_t m_handle{ nullptr };
}; // class enumerate_windows

} // namespace amped::win


#endif // _ENUMERATE_WINDOWS_HPP_
