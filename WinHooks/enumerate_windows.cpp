#include "pch.h"
#include "enumerate_windows.hpp"


using namespace std;

namespace en = amped::win::enumerate;

BOOL __stdcall enum_windows_proc(HWND h, LPARAM p);

static map<wstring, en::handle_window_t> s_map_windows_name;

static map<wstring, en::handle_window_t> s_map_windows_class;

namespace amped::win
{

#pragma optimize("", off)
enumerate_windows::enumerate_windows()
{
	enumerate();
}

enumerate_windows::enumerate_windows(const string_view name)
	: m_name_window(name)
{
	enumerate();
}

enumerate_windows::~enumerate_windows()
{
}

void enumerate_windows::operator()()
{
	m_handle = nullptr;
	enumerate();
}

en::handle_window_t enumerate_windows::operator[](const string_view str) noexcept
{
	return operator[](wstring(begin(str), end(str)));
}

en::handle_window_t enumerate_windows::operator[](const wstring_view str) noexcept
{
	if (m_handle != nullptr) return m_handle;
	for (auto&& [k, v] : s_map_windows_name)
	{
		if (k.find(str) != wstring::npos) return v;
	}
	return nullptr;
}

void enumerate_windows::enumerate()
{
	s_map_windows_name.clear();
	s_map_windows_class.clear();
	EnumWindows((WNDENUMPROC)enum_windows_proc, 0);
}

} // namespace amped::win

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
#else
	static vector<wchar_t> v_class{ 255 }, v_name{ 255 };
	transform(begin(v_class), end(v_class), begin(v_class), [](auto _) { return '\0'; });
	transform(begin(v_name), end(v_name), begin(v_name), [](auto _) { return '\0'; });

	if (int l = GetClassName(h, reinterpret_cast<LPTSTR>(v_class.data()), static_cast<int>(v_class.size())); l != 0)
	{
		if (l > v_class.size() + 2)
		{
			v_class.resize(static_cast<size_t>(l) + 2);
			GetClassName(h, reinterpret_cast<LPTSTR>(v_class.data()), static_cast<int>(v_class.size()));
			s_map_windows_class.insert(make_pair(wstring{ v_class.data() }, h));
		}
	}

	if (int l = GetWindowTextLength(h); l != 0)
	{
		if (l > v_name.size() + 2) v_name.resize(static_cast<size_t>(l) + 2);
		GetWindowText(h, reinterpret_cast<LPTSTR>(v_name.data()), static_cast<int>(v_name.size()));
		s_map_windows_name.insert(make_pair(wstring{ v_name.data() }, h));
	}
#endif // _LOG_ENUM_WINDOW
	return TRUE;
};
