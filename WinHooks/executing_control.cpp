#include "pch.h"
#include "executing_control.hpp"


using namespace std;

namespace amped::win
{

namespace exec = amped::win::execute;

namespace res = amped::resources;

namespace mem = amped::memory;

namespace fs = filesystem;

execute_manager::execute_manager()
{
	// m_lock_th.lock();
}

execute_manager::execute_manager(const wstring_view path) 
	: m_path(path)
{
	// m_lock_th.lock();
	set_search();
}

execute_manager::~execute_manager()
{
	if (m_execute)
	{
		m_execute = false;
		m_th.join();
	}
}

void execute_manager::operator()()
{
	auto un_lock = [&] {
		this_thread::sleep_for(chrono::seconds(c_pause_th));
		// m_lock_th.unlock();
		while (m_handle == nullptr) this_thread::sleep_for(chrono::seconds(c_pause_th));
	};

	m_execute = execute_process();
	
	thread th{ un_lock };
	th.join();

	// m_lock_th.unlock();

	save_project();

	//send_message<type_send_message::keyboard> send(m_handle, { system_keys::Return_key });
	//send();
	send_message<type_send_message::mouse> send(m_handle, { 909, 123 });
	send();
}

void execute_manager::operator()(const wstring_view path)
{
	m_path = path;
	set_search();
	this->operator()();
}

bool execute_manager::execute_process()
{
	exec::shell_execute_info_t sh{ sizeof(exec::shell_execute_info_t), 
		NULL, NULL, c_verb.c_str(), m_path.c_str(), NULL, NULL, SW_MAXIMIZE, NULL };

	wcout << L"Shell execute - " << m_path << endl;
	
	return ShellExecuteEx(&sh);
}

bool execute_manager::save_project()
{
#ifdef _NO_CAPTURE_BITMAP_MAIN_FORM
	// send_mouse_click(15, -10);
	HMENU n_menu = GetMenu(m_handle);
	HMENU n_sub_menu = GetSubMenu(n_menu, 0);
	// 17
	cout << GetMenuItemCount(n_sub_menu) << endl;
	m_search = L"Export Audio/Video";
	enumerate_windows _en;
	_en(), m_handle = _en[m_search];
	cout << "Check window - " << hex << m_handle << endl;

	vector<tuple<int, int>> cord{ {234, 45}, {210, 225}, {210, 333}, {210, 446}, {85, 590} };

	if (m_handle != nullptr)
	{
		for (auto&& it : cord)
		{
			auto [_x, _y] = it;
			bool _b = send_mouse_click(_x, _y);
			cout << "Send mouse click {" << dec << _x << ", " << _y << "} - " << _b << endl;
		}
		/*
		send_mouse_click(234, 45);
		send_mouse_click(210, 225);
		send_mouse_click(210, 333);
		send_mouse_click(210, 446);
		send_mouse_click(85, 590);
		*/
	}
	/*
	* Rendered Track 
	* X - 234 Y - 45
	* Create Analysis File
	* X - 210 Y- 225
	* Encode PCM
	* X - 210 Y - 333
	* Encode MP3
	* X - 210 Y - 446
	* 
	* X- 85 Y - 585
	* 
	* 
	* 
	*/
#else
	capture_window();

#ifdef _SLICING_POLYGON
	bool _is_button{ false };
	size_t _index{ 0 };
	for (size_t it{ 0 }; it < m_manager.get(); it++)
	{
		mem::buffer_t&& _buffer = m_manager[it].get();
		res::vector_pixels _pixel(_buffer);
		if (_is_button = _pixel({ m_manager[it].get_width(), m_manager[it].get_height() }); _is_button)
		{
			_index = it;
			break;
		}
	}
	bm::rect_t _rect{ m_manager[_index].get_left(), m_manager[_index].get_top(), m_manager[_index].get_left() + m_manager[_index].get_width() * 2, m_manager[_index].get_top() + m_manager[_index].get_height() * 6 };
	capture_window(_rect);
	for (size_t it{ 0 }; it < m_manager.get(); it++)
	{
		mem::buffer_t&& _buffer = m_manager[it].get();
		res::vector_pixels _pixel(_buffer);
		if (_is_button = _pixel({ m_manager[it].get_width(), m_manager[it].get_height() }); _is_button)
		{
			auto&& [_x, _y] = _pixel.get();
			m_coord = { m_manager[it].get_left(), m_manager[it].get_top() };
			send_mouse_click(m_manager[it].get_left() + _x - 8 + res::c_button_width / 2, m_manager[it].get_top() + _y - 51 + res::c_button_height / 2);
			send_mouse_scroll();
		}
	}
#else
	mem::buffer_t _buffer{ m_buffer.get() };
	res::vector_pixels _pixel(_buffer);
	_pixel({ m_buffer.get_width(), m_buffer.get_height() });
	
	for (auto&& it : _pixel.get())
	{
		cout << get<0>(it) << ":" << get<1>(it) << endl;
		size_t _1{ get<0>(it) }, _2{ get<1>(it) - 177 };
		send_message<type_send_message::mouse> send(m_handle, { _1, _2 });
		send();
	}
#endif // _SLICING_POLYGON
#endif // _NO_CAPTURE_BITMAP_MAIN_FORM

	return false;
}

#ifndef _NO_CAPTURE_BITMAP_MAIN_FORM
bool execute_manager::capture_window()
{
	capture_dc _dc{ m_handle };
#ifdef _SLICING_POLYGON
	_dc(m_manager);
#else
	_dc(m_buffer);
#endif // _SLICING_POLYGON
	return false;
}

bool execute_manager::capture_window(const bm::rect_t& rect)
{
	capture_dc _dc{ m_handle };
#ifdef _SLICING_POLYGON
	_dc(m_manager, rect);
#else
	_dc(m_buffer);
#endif // _SLICING_POLYGON
	return false;
}
#endif // _NO_CAPTURE_BITMAP_MAIN_FORM

bool execute_manager::send_mouse_click(const int x, const int y) const
{
	send_message<type_send_message::mouse> send(m_handle, { x, y });
	return send();
}

#ifndef _NO_CAPTURE_BITMAP_MAIN_FORM
bool execute_manager::send_mouse_scroll() const
{
	send_message<type_send_message::scroll> send(m_handle, -120, m_coord);
	return send();
}
#endif // _NO_CAPTURE_BITMAP_MAIN_FORM

void execute_manager::set_search()
{
	m_search = L"[";
	m_search += fs::path{ m_path }.stem().wstring();
	m_search += L"]";
}

void execute_manager::check_windows()
{
	while (!m_execute) this_thread::sleep_for(chrono::seconds(c_pause_th));

	enumerate_windows _en;
	while (m_execute)
	{
		wstring _str{ L"Ableton" };
		_en(), m_handle = _en[m_search];
		this_thread::sleep_for(chrono::seconds(c_pause_th));
		cout << "Check window - " << hex << m_handle << endl;
	}
}

} // namespace amped::win
