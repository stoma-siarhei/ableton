#ifndef _EXECUTING_CONTROL_HPP_
#define _EXECUTING_CONTROL_HPP_


#include "pch.h"

#include "hooks_headers.h"
#include "enumerate_windows.hpp"
#include "capture_bitmap.hpp"
#include "buffers_manager.hpp"
#include "system_messages.hpp"


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
	bool capture_window();
	void set_search();
private:
	void check_windows();

	wstring m_path;
	wstring m_search;
	atomic<en::handle_window_t> m_handle{ nullptr };

	thread m_th{ &execute_manager::check_windows, this };
	mutex m_lock_th;
	atomic_bool m_execute{ false };

	mem::buffer_image m_buffer;
}; // class execute_manager

} // namespace amped::win


#endif // _EXECUTING_CONTROL_HPP_
