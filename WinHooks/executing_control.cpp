#include "pch.h"
#include "executing_control.hpp"


using namespace std;

namespace amped::win
{

namespace exec = amped::win::execute;

namespace res = amped::resources;

namespace fs = filesystem;

execute_manager::execute_manager()
{
	m_lock_th.lock();
}

execute_manager::execute_manager(const wstring_view path) 
	: m_path(path)
{
	m_lock_th.lock();
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
		m_lock_th.unlock();
		while (m_handle == nullptr) this_thread::sleep_for(chrono::seconds(c_pause_th));
	};

	m_execute = execute_process();
	
	thread th{ un_lock };
	th.join();

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
	res::button_res_t _buf, _old;

	for (uint32_t it_y = 0; it_y < m_buffer.get_height(res::c_button_height); it_y++)
	{

	}

	INPUT inputs[1] = { 0 };
	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = system_keys::Return_key;
	inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
	return true;

	send_message<type_send_message::keyboard> send(m_handle, { system_keys::Return_key });
	send();

	return false;
}

bool execute_manager::capture_window()
{
	capture_dc _dc{ m_handle };
	_dc(m_buffer);
	return false;
}

void execute_manager::set_search()
{
	m_search = fs::path{ m_path }.stem().wstring();
}

void execute_manager::check_windows()
{
	lock_guard<mutex> lock(m_lock_th);
	enumerate_windows _en;
	while (m_execute)
	{
		wstring str{ L"enumerate" };
		_en(), m_handle = _en[m_search];
		this_thread::sleep_for(chrono::seconds(c_pause_th));
	}
}

} // namespace amped::win
