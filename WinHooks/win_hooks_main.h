#ifndef _WIN_HOOKS_MAIN_H_
#define _WIN_HOOKS_MAIN_H_


#include "pch.h"


using namespace std;

namespace amped::win
{

	const wstring c_ableton_class_name{ L"Ableton Live Window Class" };
	//const wstring c_ableton_class_name{ L"TTOTAL_CMD" };

class enumerate_windows
{
public:
	using handle_window_t = HWND;
	using param_t = LPARAM;

	explicit enumerate_windows();
	explicit enumerate_windows(const string_view name);
protected:
	void enumerate();
private:
	list<handle_window_t> m_list_windows;
	string_view m_name_window;
}; // class enumerate_windows

} // namespace amped::win


#endif // _WIN_HOOKS_MAIN_H_
