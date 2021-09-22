#include "pch.h"
#include "capture_bitmap.hpp"


namespace amped::win
{

namespace bm = amped::win::bmp;

capture_dc::capture_dc(const bm::handle_t h) 
	: m_handle(h)
{
}

capture_dc::~capture_dc()
{
}

#ifdef _SLICING_POLYGON
bool capture_dc::operator()(memory::manager& manager) const
{
	bm::handle_dc_t _hdc = GetWindowDC(m_handle);
	bm::handle_dc_t _bmp = CreateCompatibleDC(_hdc);

	bm::rect_t _rect;
	GetClientRect(m_handle, &_rect);
	SetStretchBltMode(_hdc, HALFTONE);

	array<tuple<int, int>, 2> _start{ make_tuple(0, 0), { bm::c_size_image_w / 2, bm::c_size_image_h / 2 } };
	array<size_t, 2> _count_x{ ((size_t)_rect.right - _rect.left) / bm::c_size_image_w, ((size_t)_rect.right - _rect.left + get<0>(_start[1])) / bm::c_size_image_w },
		             _count_y{ ((size_t)_rect.bottom - _rect.top) / bm::c_size_image_h, ((size_t)_rect.bottom - _rect.top + get<1>(_start[1])) / bm::c_size_image_h };

	manager.set(_count_x[0] * _count_y[0] + _count_x[1] * _count_y[1]);

	int _dx{ bm::c_size_image_w }, _dy{ bm::c_size_image_h };

	bm::hbitmap_t _hbitmap = CreateCompatibleBitmap(_hdc, _dx, _dy);

	SelectObject(_bmp, _hbitmap);

	size_t _index{ 0 };
	for (size_t it = 0; it < 2; it++)
	{
		auto&& [_start_dx, _start_dy] = _start[it];
		for (size_t it_y = 0; it_y < _count_y[it]; it_y++)
		{
			for (size_t it_x = 0; it_x < _count_x[it]; it_x++)
			{
				int _l{ _start_dx + static_cast<int>(it_x) * 100 }, _t{ _start_dy + static_cast<int>(it_y) * 50 };
				BitBlt(_bmp, 0, 0, _dx, _dy, _hdc, _l, _t, SRCCOPY);

				bm::bitmap_t _bitmap;
				GetObject(_hbitmap, sizeof(bm::bitmap_t), &_bitmap);

				memory::buffer_image& buffer = manager[_index++];

				bm::info_header_t _header{ sizeof(bm::info_header_t), _bitmap.bmWidth, _bitmap.bmHeight, 1, 32, BI_RGB, 0, 0, 0, 0, 0 };
				const amped::memory::dimensions _dim{ _header.biWidth, _header.biHeight, _header.biBitCount, 4, _l, _t };
				GetDIBits(_hdc, _hbitmap, 0, static_cast<uint32_t>(_bitmap.bmHeight), buffer(_dim), (bm::info_t*)&_header, DIB_RGB_COLORS);
#ifdef _SAVE_BITMAP_TO_FILE
				string _name{ "D:\\res_bmp\\polygon_" };
				_name += to_string(_t);
				_name += "_";
				_name += to_string(_l);
				_name += ".bmp";
				uint32_t _size_dib{ _dim.size() + sizeof(bm::file_header_t) + sizeof(bm::info_header_t) };
				bm::file_header_t f_header{ 0x4D42, _size_dib, 0, 0, sizeof(bm::file_header_t) + sizeof(bm::info_header_t) };
				if (ofstream f(_name, ios::binary); f.is_open())
				{
					f.write(reinterpret_cast<char*>(&f_header), sizeof(bm::file_header_t));
					f.write(reinterpret_cast<char*>(&_header), sizeof(bm::info_header_t));
					f.write(reinterpret_cast<char*>(buffer.get().data()), _dim.size());
					f.flush();
					f.close();
				}
#endif // _SAVE_BITMAP_TO_FILE
			}
		}
	}

	DeleteObject(_hbitmap);
	DeleteObject(_bmp);
	ReleaseDC(m_handle, _hdc);

	return true;
}
bool capture_dc::operator()(memory::manager& manager, const bm::rect_t& rect) const
{
	auto compare = [&](const bm::rect_t& client) -> bool
	{
		return (client.left < rect.left) & (client.right > rect.right) &
			   (client.top < rect.top)   & (client.bottom > rect.bottom);
	};

	bm::rect_t _rect;
	GetClientRect(m_handle, &_rect);

	if (compare(_rect) == false) return false;

	bm::handle_dc_t _hdc = GetWindowDC(m_handle);
	bm::handle_dc_t _bmp = CreateCompatibleDC(_hdc);

	SetStretchBltMode(_hdc, HALFTONE);

	array<tuple<int, int>, 2> _start{ make_tuple(rect.left, rect.top), { rect.left + bm::c_size_image_w / 2, rect.top + bm::c_size_image_h / 2 } };
	array<size_t, 2> _count_x{ ((size_t)rect.right - rect.left) / bm::c_size_image_w, ((size_t)rect.right - rect.left + get<0>(_start[1])) / bm::c_size_image_w },
		             _count_y{ ((size_t)rect.bottom - rect.top) / bm::c_size_image_h, ((size_t)rect.bottom - rect.top + get<1>(_start[1])) / bm::c_size_image_h };

	manager.set(_count_x[0] * _count_y[0] + _count_x[1] * _count_y[1]);

	int _dx{ bm::c_size_image_w }, _dy{ bm::c_size_image_h };

	bm::hbitmap_t _hbitmap = CreateCompatibleBitmap(_hdc, _dx, _dy);

	SelectObject(_bmp, _hbitmap);

	size_t _index{ 0 };
	for (size_t it = 0; it < 2; it++)
	{
		auto&& [_start_dx, _start_dy] = _start[it];
		for (size_t it_y = 0; it_y < _count_y[it]; it_y++)
		{
			for (size_t it_x = 0; it_x < _count_x[it]; it_x++)
			{
				int _l{ _start_dx + static_cast<int>(it_x) * 100 }, _t{ _start_dy + static_cast<int>(it_y) * 50 };
				BitBlt(_bmp, 0, 0, _dx, _dy, _hdc, _l, _t, SRCCOPY);

				bm::bitmap_t _bitmap;
				GetObject(_hbitmap, sizeof(bm::bitmap_t), &_bitmap);

				memory::buffer_image& buffer = manager[_index++];

				bm::info_header_t _header{ sizeof(bm::info_header_t), _bitmap.bmWidth, _bitmap.bmHeight, 1, 32, BI_RGB, 0, 0, 0, 0, 0 };
				const amped::memory::dimensions _dim{ _header.biWidth, _header.biHeight, _header.biBitCount, 4, _l, _t };
				GetDIBits(_hdc, _hbitmap, 0, static_cast<uint32_t>(_bitmap.bmHeight), buffer(_dim), (bm::info_t*)&_header, DIB_RGB_COLORS);
#ifdef _SAVE_BITMAP_TO_FILE
				uint32_t _size_dib{ _dim.size() + sizeof(bm::file_header_t) + sizeof(bm::info_header_t) };
				bm::file_header_t f_header{ 0x4D42, _size_dib, 0, 0, sizeof(bm::file_header_t) + sizeof(bm::info_header_t) };
				if (ofstream f("D:\\result.bmp", ios::binary); f.is_open())
				{
					f.write(reinterpret_cast<char*>(&f_header), sizeof(bm::file_header_t));
					f.write(reinterpret_cast<char*>(&_header), sizeof(bm::info_header_t));
					f.write(reinterpret_cast<char*>(buffer.get().data()), _dim.size());
					f.flush();
					f.close();
				}
#endif // _SAVE_BITMAP_TO_FILE
			}
		}
	}

	DeleteObject(_hbitmap);
	DeleteObject(_bmp);
	ReleaseDC(m_handle, _hdc);

	return true;
}
#else
bool capture_dc::operator()(memory::buffer_image& buffer)
{
	bm::handle_dc_t _hdc = GetWindowDC(m_handle);
	bm::handle_dc_t _bmp = CreateCompatibleDC(_hdc);

	bm::rect_t _rect;
	GetClientRect(m_handle, &_rect);
	SetStretchBltMode(_hdc, HALFTONE);

	int _dx{ _rect.right - _rect.left }, _dy{ _rect.bottom - _rect.top };

	bm::hbitmap_t _hbitmap = CreateCompatibleBitmap(_hdc, _dx, _dy);

	SelectObject(_bmp, _hbitmap);

	BitBlt(_bmp, 0, 0, _dx, _dy, _hdc, 0, 0, SRCCOPY);

	bm::bitmap_t _bitmap;
	GetObject(_hbitmap, sizeof(bm::bitmap_t), &_bitmap);

	bm::info_header_t _header{ sizeof(bm::info_header_t), _bitmap.bmWidth, _bitmap.bmHeight, 1, 32, BI_RGB, 0, 0, 0, 0, 0 };
	const amped::memory::dimensions _dim{ _header.biWidth, _header.biHeight, _header.biBitCount, 4 };
	GetDIBits(_hdc, _hbitmap, 0, static_cast<uint32_t>(_bitmap.bmHeight), buffer(_dim), (bm::info_t*)&_header, DIB_RGB_COLORS);

#ifdef _SAVE_BITMAP_TO_FILE
#endif // _SAVE_BITMAP_TO_FILE

	DeleteObject(_hbitmap);
	DeleteObject(_bmp);
	ReleaseDC(m_handle, _hdc);

	return true;
}
#endif //_SLICING_POLYGON

void capture_dc::init()
{
}

} // namespace amped::win
