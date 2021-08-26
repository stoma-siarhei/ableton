#ifndef _ENUMERATE_WINDOWS_HPP_
#define _ENUMERATE_WINDOWS_HPP_


#define _LOG_ENUM_WINDOW
#undef _LOG_ENUM_WINDOW


#include "pch.h"


using namespace std;

namespace amped::win
{

const wstring c_ableton_class_name{ L"Ableton Live Window Class" };

class enumerate_windows
{
public:
	using handle_window_t = HWND;
	using param_t = LPARAM;
	using class_name_t = tuple<wstring, wstring>;
	using handle_map_t = map<handle_t, class_name_t>;

	explicit enumerate_windows();
	explicit enumerate_windows(const string_view name);
	~enumerate_windows();
protected:
	void enumerate();
	friend bool insert_handle(const handle_window_t h);
	friend handle_map_t& get_map(enumerate_windows& en);
private:
	handle_map_t m_map_handle;
	string_view m_name_window;

	static enumerate_windows* m_this;
}; // class enumerate_windows

} // namespace amped::win


#endif // _ENUMERATE_WINDOWS_HPP_
