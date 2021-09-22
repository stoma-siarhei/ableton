#ifndef _HOOKS_HEADERS_H_
#define _HOOKS_HEADERS_H_


#include "pch.h"


using namespace std;

namespace amped::resources
{

constexpr const size_t c_button_width{ 21 };

constexpr const size_t c_button_height{ 13 };

constexpr const size_t c_byte_pixel{ 4 };

using button_res_t = vector<uint8_t>;

using coord_t = tuple<size_t, size_t>;

using vector_coord_t = vector<coord_t>;

using array_pixel_t = array<uint8_t, 4>;

using vector_pixel_t = vector<array_pixel_t>;

using list_coord_t = list<coord_t>;

} // namespace amped::resources

namespace amped::memory
{

using buffer_t = vector<uint8_t>;

} // namespace amped::memory

namespace amped::win::message
{

using handle_t = HWND;

using message_t = UINT;

using wparam_t = WPARAM;

using lparam_t = LPARAM;

using result_t = LRESULT;

} // namespace amped::win::message

namespace amped::win::enumerate
{

using handle_window_t = amped::win::message::handle_t;

using param_t = LPARAM;

using class_name_t = tuple<wstring, wstring>;

using handle_map_t = map<handle_t, class_name_t>;

} // namespace amped::win::enumerate

namespace amped::win::bmp
{

constexpr const size_t c_size_image_w{ 100 };

constexpr const size_t c_size_image_h{ 50 };

using handle_t = amped::win::enumerate::handle_window_t;

using handle_dc_t = HDC;

using hbitmap_t = HBITMAP;

using bitmap_t = BITMAP;

using rect_t = RECT;

using info_t = BITMAPINFO;

using info_header_t = BITMAPINFOHEADER;

using file_header_t = BITMAPFILEHEADER;

} // namespace amped::win::bmp

namespace amped::win::execute
{

using shell_execute_info_t = SHELLEXECUTEINFO;

} // namespace amped::win::execute


#endif // _HOOKS_HEADERS_H_
