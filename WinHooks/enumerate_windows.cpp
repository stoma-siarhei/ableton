#include "pch.h"
#include "enumerate_windows.hpp"


using namespace std;

bool insert_handle(const amped::win::enumerate_windows::handle_window_t h);

#pragma optimize("", off)
BOOL __stdcall enum_windows_proc(HWND h, LPARAM p)
{
#ifdef _LOG_ENUM_WINDOW
	wstring ss;
	wofstream f(L"enumerate windows.txt", ios::app);
	if (int len = GetWindowTextLength(h); len != 0)
	{
		vector<wchar_t> v(static_cast<size_t>(len) + 2);
		if (int l_str = GetWindowText(h, reinterpret_cast<LPTSTR>(v.data()), static_cast<int>(v.size())); l_str != 0)
		{
			ss = v.data();
		}
		else
		{
			return TRUE;
		}
		ZeroMemory(static_cast<void*>(v.data()), static_cast<int>(v.size() * 2)), ss += L" | ";
		if (int l_str = GetClassName(h, reinterpret_cast<LPTSTR>(v.data()), static_cast<int>(v.size())); l_str != 0)
		{
			ss += v.data();
		}
		else
		{
			return TRUE;
		}
	}
	else
	{
		if (GetLastError() != 0) return TRUE;
	}
	f << ss;
	f << endl;
#endif // _LOG_ENUM_WINDOW
	insert_handle(h);
	return TRUE;
};

namespace amped::win
{

enumerate_windows* enumerate_windows::m_this = nullptr;

enumerate_windows::enumerate_windows()
{
	m_this = this;
	enumerate();
}

enumerate_windows::enumerate_windows(const string_view name)
	: m_name_window(name)
{
	m_this = this;
	enumerate();
}

enumerate_windows::~enumerate_windows()
{
	m_this = nullptr;
}

void enumerate_windows::enumerate()
{
	EnumWindows((WNDENUMPROC)enum_windows_proc, 0);
	if (handle_window_t h = FindWindowEx(NULL, NULL, c_ableton_class_name.c_str(), NULL); h != nullptr)
	{
		EnumChildWindows(h, (WNDENUMPROC)enum_windows_proc, 0);
	}
}

bool insert_handle(const amped::win::enumerate_windows::handle_window_t h)
{
	static vector<wchar_t> v_class{ 255 }, v_name{ 255 };
	for_each(begin(v_class), end(v_class), [](auto&& v) { v = L'\0'; });
	if (int l = GetClassName(h, reinterpret_cast<LPTSTR>(v_class.data()), static_cast<int>(v_class.size())); l == 0) return true;
	if (int l = GetWindowTextLength(h); l != 0)
	{
		if (l > v_name.size() + 2) v_name.resize(l + 2);
		for_each(begin(v_name), end(v_name), [](auto&& v) { v = L'\0'; });
		GetWindowText(h, reinterpret_cast<LPTSTR>(v_name.data()), static_cast<int>(v_name.size()));
		get_map(*amped::win::enumerate_windows::m_this).insert({ h, { wstring{ v_class.data() }, wstring{ v_name.data() } } });
	}
	else
	{
		get_map(*amped::win::enumerate_windows::m_this).insert({ h, { wstring{ v_class.data() }, wstring{ L"" } } });
	}

	return true;
}

amped::win::enumerate_windows::handle_map_t& get_map(enumerate_windows& en)
{
	return add_lvalue_reference_t<amped::win::enumerate_windows::handle_map_t>(en.m_map_handle);
}

} // namespace amped::win
