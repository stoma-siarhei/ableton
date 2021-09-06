#include <iostream>

#include <win_hooks_main.h>


using namespace std;
#pragma optimize("", off);
int main()
{
    std::cout << "Hello World!\n";
	amped::win::enumerate_windows e;
	amped::win::enumerate::handle_window_t h{ e["Electr Glitch"] };

	amped::win::capture_dc dc{ h };
	return 0;
	{
		amped::win::execute_manager manager;
		wstring str{ L"C:\\Users\\scander\\Documents\\test_2\\Electr Glitch  Project\\Electr Glitch .als" };
		manager(str);
		Sleep(20000);
		amped::win::enumerate_windows e;
		amped::win::enumerate::handle_window_t h{ e["Ableton"] };
		amped::win::export_vawe _ex(h);
		_ex(str);
		return 0;
	}
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
