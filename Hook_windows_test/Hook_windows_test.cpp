#include <iostream>

#include <win_hooks_main.h>


using namespace std;
#pragma optimize("", off)
int main()
{
	using namespace amped::win;
	{
		amped::win::execute_manager manager;
		wstring str{ L"C:\\Users\\scander\\Documents\\test_2\\Electr Glitch  Project\\Electr Glitch .als" };
		manager(str);
		// amped::win::enumerate_windows e;
		// amped::win::enumerate::handle_window_t h{ e["Electr Glitch"] };
		// amped::win::export_vawe _ex(h);
		// _ex(str);
		return 0;
	}
	{
		enumerate_windows e;
		enumerate::handle_window_t h{ e["MIDI Monitor"] };
		send_message_impl::handle_t _h{ (send_message_impl::handle_t)0xE0926 };
		send_message<type_send_message::mouse> cl(h, { 400, 800 });
		cl();
		send_message<type_send_message::scroll> sc(h, -120, { 400, 400 });
		cout << sc() << endl;
		// Sleep(10000);
		// SendMessage(h, WM_ACTIVATE, (1 << 16) + WA_ACTIVE, 0);
		SetFocus(h);
		// PostMessage(NULL, WM_MOUSEMOVE, NULL, DWORD(400 << 16 | 2400));
		cout << SendMessage(h, WM_MOUSEWHEEL, DWORD(-120 << 16), DWORD((400 << 16) + 2400)) << endl;
		cout << GetLastError() << endl;
		return 0;
	}
    std::cout << "Hello World!\n";
	amped::win::enumerate_windows e;
	amped::win::enumerate::handle_window_t h{ e["Electr Glitch"] };

	amped::win::capture_dc dc{ h };

	amped::memory::buffer_t buff;
	amped::resources::vector_pixels pixel(buff);
	return 0;
	{
		ifstream rf("D:\\button_off.dat", ios::binary);
		ofstream wf("D:\\button_off.txt");
		for (int i = 0; i < 10; i++)
		{
			cout << "0x" << hex << setfill('0') << setw(2) << rf.get() << ", ";
		}
		size_t p{ 0 };
		vector<uint8_t> v(444);
		rf.read((char*)v.data(), v.size());
		rf.close();
		for (size_t it = 0; it < v.size(); it++)
		{
			int r{ 0 };
			r = v[it];
			wf << "0x" << hex << setfill('0') << setw(2) << r << ", ";
			if (++p == 16)
			{
				p = 0;
				wf << endl;
			}
		}
		wf.close();
		return 0;
	}
}
