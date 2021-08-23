#include "pch.h"
#include "win_hooks_main.h"


using namespace std;

void find_window(const string_view name)
{

}

#pragma optimize("", off)
BOOL __stdcall enum_windows_proc(HWND h, LPARAM p)
{
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
	wcout << ss << endl;
	return TRUE;
};

namespace amped::win
{

enumerate_windows::enumerate_windows()
{
	enumerate();
}

enumerate_windows::enumerate_windows(const string_view name)
	: m_name_window(name)
{
	enumerate();
}

void enumerate_windows::enumerate()
{
	EnumWindows((WNDENUMPROC)enum_windows_proc, 0);
	if (handle_window_t h = FindWindowEx(NULL, NULL, c_ableton_class_name.c_str(), NULL); h != nullptr)
	{
		EnumChildWindows(h, (WNDENUMPROC)enum_windows_proc, 0);
	}
}

} // namespace amped::win
