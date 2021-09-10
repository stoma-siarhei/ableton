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
	
	return ShellExecuteEx(&sh);
}

bool execute_manager::save_project()
{
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
		/*
		for (auto&& itt : _pixel.get())
		{
			cout << m_manager[it].get_left() + get<0>(itt) - 8 << ":" << m_manager[it].get_top() + get<1>(itt) - 51 << endl;
		}
		*/
	}
	bm::rect_t _rect{ m_manager[_index].get_left(), m_manager[_index].get_top(), m_manager[_index].get_left() + res::c_button_width * 2, m_manager[_index].get_height() - m_manager[_index].get_top() - res::c_button_height };
	capture_window(_rect);
	for (size_t it{ 0 }; it < m_manager.get(); it++)
	{
		mem::buffer_t&& _buffer = m_manager[it].get();
		res::vector_pixels _pixel(_buffer);
		if (_is_button = _pixel({ m_manager[it].get_width(), m_manager[it].get_height() }); _is_button)
		{
			auto&& [_x, _y] = _pixel.get();
			send_mouse(m_manager[it].get_left() + _x - 8 + res::c_button_width / 2, m_manager[it].get_top() + _y - 51 + res::c_button_height / 2);
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

	return false;
}

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
	return false;
}

bool execute_manager::send_mouse(const size_t x, const size_t y)
{
	send_message<type_send_message::mouse> send(m_handle, { x, y });
	return send();
}

void execute_manager::set_search()
{
	m_search = fs::path{ m_path }.stem().wstring();
}

void execute_manager::check_windows()
{
	while (!m_execute) this_thread::sleep_for(chrono::seconds(c_pause_th));

	enumerate_windows _en;
	while (m_execute)
	{
		_en(), m_handle = _en[m_search];
		this_thread::sleep_for(chrono::seconds(c_pause_th));
	}
}

} // namespace amped::win
