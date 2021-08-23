#include <iostream>

#include <win_hooks_main.h>


using namespace std;

int main()
{
    std::cout << "Hello World!\n";
	amped::win::enumerate_windows();
	return 0;
}
