#ifndef _EXECUTING_CONTROL_HPP_
#define _EXECUTING_CONTROL_HPP_


#include "pch.h"

#include "hooks_headers.h"
#include "enumerate_windows.hpp"
#include "capture_bitmap.hpp"
#include "buffers_manager.hpp"
#include "system_messages.hpp"
#include "hooks_resources.hpp"


#define _NO_CAPTURE_BITMAP_MAIN_FORM
#undef _NO_CAPTURE_BITMAP_MAIN_FORM_


using namespace std;

namespace amped::win
{

namespace en = amped::win::enumerate;

namespace mem = amped::memory;

class execute_manager
{
	const wstring c_verb{ L"open" };
	const uint64_t c_pause_th{ 10 };
public:
	execute_manager();

	execute_manager(const wstring_view path);

	~execute_manager();

	void operator()();

	void operator()(const wstring_view path);
protected:
	bool execute_process();
	bool save_project();
#ifndef _NO_CAPTURE_BITMAP_MAIN_FORM
	bool capture_window();
	bool capture_window(const bm::rect_t& rect);
#endif // _NO_CAPTURE_BITMAP_MAIN_FORM
	bool send_mouse_click(const int x, const int y) const;
#ifndef _NO_CAPTURE_BITMAP_MAIN_FORM
	bool send_mouse_scroll() const;
#endif // _NO_CAPTURE_BITMAP_MAIN_FORM
	void set_search();
private:
	void check_windows();

	wstring m_path;
	wstring m_search;
	atomic<en::handle_window_t> m_handle{ nullptr };

	thread m_th{ &execute_manager::check_windows, this };
	// mutex m_lock_th;
	atomic_bool m_execute{ false };

	tuple<uint32_t, uint32_t> m_coord;
#ifdef _SLICING_POLYGON
	mem::manager m_manager;
#else
	mem::buffer_image m_buffer;
#endif // _SLICING_POLYGON
}; // class execute_manager

} // namespace amped::win


#endif // _EXECUTING_CONTROL_HPP_
