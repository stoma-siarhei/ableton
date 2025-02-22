#include "pch.h"
#include "hooks_resources.hpp"


using namespace std;

namespace amped::resources
{

vector_pixel_t make_vector_pixel(const button_res_t& source, const vector_coord_t& coord)
{
	vector_pixel_t v{ coord.size() };
	size_t _index{ 0 };
	for (auto&& it : coord)
	{
		auto&& [w, h] = it;
		size_t i{ (h * c_button_width + w) * 4 };
		v[_index++] = { source[i], source[i + 1], source[i + 2], source[i + 3] };
	}
	return v;
}

vector_pixels::vector_pixels(mem::buffer_t& buff)
	: m_buffer(buff)
{
	auto delta = [](const array_pixel_t& a) {
		array_pixel_t res{ 0, 0, 0, 0 };
		transform(begin(a), end(a), begin(res), [](auto arg) { return arg / 10; });
		return res;
	};

	m_pixel_border = s_border_data(false);
	m_pixel_letter = s_letter_data(false);
	m_pixel_border_s.resize(m_pixel_border.size());
	m_pixel_letter_s.resize(m_pixel_letter.size());
	m_delta_border.resize(m_pixel_border.size());
	m_delta_letter.resize(m_pixel_letter.size());
	transform(begin(m_pixel_border), end(m_pixel_border), begin(m_delta_border), delta);
	transform(begin(m_pixel_letter), end(m_pixel_letter), begin(m_delta_letter), delta);
}

bool vector_pixels::operator()(const coord_t& coord, const size_t pos)
{
	size_t st_h{ 0 }, st_w{ 0 };
	auto&& [w, h] = coord;
	if (pos > 0)
	{
		st_h = pos / w;
		st_w = pos % w;
	}
	for (size_t it_h{ st_h }; it_h < h - c_button_height; it_h++)
	{
		for (size_t it_w{ st_w }; it_w < w - c_button_width; it_w++)
		{
			make_pixel(it_h * w + it_w, w);
			if (compare())
			{
				m_coord = { it_w, it_h };
				return true;
			}
		}
	}
	return false;
}

coord_t vector_pixels::get() const
{
	return m_coord;
}

void vector_pixels::make_pixel(const size_t _pos, const size_t _width)
{
	size_t _index{ 0 };
	for (auto&& it : s_primitive.m_border)
	{
		auto&& [w, h] = it;
		size_t p{ (h * _width + w + _pos) * 4 };
		m_pixel_border_s[_index++] = { m_buffer[p], m_buffer[p + 1], m_buffer[p + 2], m_buffer[p + 3] };
	}
	_index = 0;
	for (auto&& it : s_primitive.m_letter)
	{
		auto&& [w, h] = it;
		size_t p{ (h * _width + w + _pos) * 4 };
		m_pixel_letter_s[_index++] = { m_buffer[p], m_buffer[p + 1], m_buffer[p + 2], m_buffer[p + 3] };
	}
}

bool vector_pixels::compare() const
{
	for (size_t it{ 0 }; it < m_pixel_border.size(); it++)
	{
		array_pixel_t dest{ m_pixel_border[it] }, sour{ m_pixel_border_s[it] }, delt{ m_delta_border[it] };
		for (size_t itt{ 0 }; itt < 4; itt++)
		{
			if (dest[itt] == sour[itt]) continue;
			return false;
		}
	}
	for (size_t it{ 0 }; it < m_pixel_letter.size(); it++)
	{
		array_pixel_t dest{ m_pixel_letter[it] }, sour{ m_pixel_letter_s[it] }, delt{ m_delta_border[it] };
		for (size_t itt{ 0 }; itt < 4; itt++)
		{
			if (dest[itt] == sour[itt]) continue;
			return false;
		}
	}
	return true;
}

button_select::button_select()
{
	off =
	{
		0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50,
		0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF,
		0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50,
		0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5,
		0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5,
		0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0x9B, 0x9B, 0x9B, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5,
		0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x65, 0x65,
		0x65, 0xFF, 0x13, 0x13, 0x13, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x20, 0x20, 0x20, 0xFF, 0x7E, 0x7E, 0x7E, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50,
		0x50, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0x90, 0x90, 0x90, 0xFF, 0x05, 0x05, 0x05, 0xFF, 0x63, 0x63, 0x63, 0xFF, 0x9A, 0x9A, 0x9A, 0xFF, 0x4B, 0x4B, 0x4B, 0xFF, 0x28, 0x28, 0x28, 0xFF, 0xA5, 0xA5,
		0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5,
		0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x8C, 0x8C, 0x8C, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x92, 0x92, 0x92, 0xFF, 0x35, 0x35, 0x35, 0xFF,
		0x17, 0x17, 0x17, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5,
		0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x87, 0x87, 0x87, 0xFF, 0x2E, 0x2E, 0x2E, 0xFF, 0x01, 0x01,
		0x01, 0xFF, 0x2A, 0x2A, 0x2A, 0xFF, 0x81, 0x81, 0x81, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5,
		0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x21, 0x21, 0x21, 0xFF,
		0x2D, 0x2D, 0x2D, 0xFF, 0x8E, 0x8E, 0x8E, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5,
		0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5,
		0xA5, 0xFF, 0x31, 0x31, 0x31, 0xFF, 0x47, 0x47, 0x47, 0xFF, 0x96, 0x96, 0x96, 0xFF, 0x37, 0x37, 0x37, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x50, 0x50,
		0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x82, 0x82, 0x82, 0xFF, 0x1E, 0x1E, 0x1E, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x20, 0x20, 0x20, 0xFF, 0x8A, 0x8A,
		0x8A, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5,
		0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x9B, 0x9B, 0x9B, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5,
		0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5,
		0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50,
		0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF,
		0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50,
		0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF
	}; // const button_res_t button_select::off

	on =
	{
		0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50,
		0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF,
		0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50,
		0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1,
		0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1,
		0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0xEF, 0x97, 0x45, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1,
		0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0x9C, 0x62,
		0x2D, 0xFF, 0x1E, 0x13, 0x09, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x31, 0x1F, 0x0E, 0xFF, 0xC3, 0x7B, 0x39, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50,
		0x50, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0xDE, 0x8C, 0x40, 0xFF, 0x08, 0x05, 0x02, 0xFF, 0x99, 0x61, 0x2C, 0xFF, 0xEE, 0x96, 0x45, 0xFF, 0x74, 0x49, 0x22, 0xFF, 0x3E, 0x27, 0x12, 0xFF, 0xFF, 0xA1,
		0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1,
		0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xD8, 0x88, 0x3F, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xE2, 0x8F, 0x42, 0xFF, 0x52, 0x34, 0x18, 0xFF,
		0x23, 0x16, 0x0A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1,
		0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xD1, 0x84, 0x3D, 0xFF, 0x47, 0x2D, 0x15, 0xFF, 0x02, 0x01,
		0x01, 0xFF, 0x41, 0x29, 0x13, 0xFF, 0xC8, 0x7E, 0x3A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1,
		0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0x33, 0x20, 0x0F, 0xFF,
		0x45, 0x2C, 0x14, 0xFF, 0xDB, 0x8A, 0x40, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1,
		0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF,
		0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1,
		0x4A, 0xFF, 0x4B, 0x2F, 0x16, 0xFF, 0x6D, 0x45, 0x20, 0xFF, 0xE8, 0x92, 0x43, 0xFF, 0x55, 0x36, 0x19, 0xFF, 0x7C, 0x4E, 0x24, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0x50, 0x50,
		0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xC9, 0x7F, 0x3A, 0xFF, 0x2E, 0x1D, 0x0D, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x31, 0x1F, 0x0E, 0xFF, 0xD6, 0x87,
		0x3E, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0xFF, 0xA1, 0x4A, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1,
		0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xEF, 0x97, 0x45, 0xFF,
		0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1,
		0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1,
		0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50,
		0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF,
		0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50,
		0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF
	}; // const button_res_t button_select::on

	off_focus =
	{
		0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50,
		0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF,
		0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50,
		0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5,
		0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5,
		0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0x9B, 0x9B, 0x9B, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5,
		0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x65, 0x65,
		0x65, 0xFF, 0x13, 0x13, 0x13, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x20, 0x20, 0x20, 0xFF, 0x7E, 0x7E, 0x7E, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50,
		0x50, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0x90, 0x90, 0x90, 0xFF, 0x05, 0x05, 0x05, 0xFF, 0x63, 0x63, 0x63, 0xFF, 0x9A, 0x9A, 0x9A, 0xFF, 0x4B, 0x4B, 0x4B, 0xFF, 0x28, 0x28, 0x28, 0xFF, 0xA5, 0xA5,
		0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5,
		0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x8C, 0x8C, 0x8C, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x92, 0x92, 0x92, 0xFF, 0x35, 0x35, 0x35, 0xFF,
		0x17, 0x17, 0x17, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5,
		0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x87, 0x87, 0x87, 0xFF, 0x2E, 0x2E, 0x2E, 0xFF, 0x01, 0x01,
		0x01, 0xFF, 0x2A, 0x2A, 0x2A, 0xFF, 0x81, 0x81, 0x81, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5,
		0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x21, 0x21, 0x21, 0xFF,
		0x2D, 0x2D, 0x2D, 0xFF, 0x8E, 0x8E, 0x8E, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5,
		0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5,
		0xA5, 0xFF, 0x31, 0x31, 0x31, 0xFF, 0x47, 0x47, 0x47, 0xFF, 0x96, 0x96, 0x96, 0xFF, 0x37, 0x37, 0x37, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x50, 0x50,
		0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x82, 0x82, 0x82, 0xFF, 0x1E, 0x1E, 0x1E, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x20, 0x20, 0x20, 0xFF, 0x8A, 0x8A,
		0x8A, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5,
		0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x9B, 0x9B, 0x9B, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5,
		0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5,
		0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF,
		0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0xA5, 0xA5, 0xA5, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50,
		0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF,
		0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50,
		0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF
	}; // const button_res_t button_select::is_focused::off

	on_focus =
	{
		0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50,
		0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF,
		0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50,
		0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1,
		0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1,
		0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0xEF, 0x97, 0x45, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1,
		0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0x9C, 0x62,
		0x2D, 0xFF, 0x1E, 0x13, 0x09, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x31, 0x1F, 0x0E, 0xFF, 0xC3, 0x7B, 0x39, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50,
		0x50, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0xDE, 0x8C, 0x40, 0xFF, 0x08, 0x05, 0x02, 0xFF, 0x99, 0x61, 0x2C, 0xFF, 0xEE, 0x96, 0x45, 0xFF, 0x74, 0x49, 0x22, 0xFF, 0x3E, 0x27, 0x12, 0xFF, 0xFF, 0xA1,
		0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1,
		0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xD8, 0x88, 0x3F, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xE2, 0x8F, 0x42, 0xFF, 0x52, 0x34, 0x18, 0xFF,
		0x23, 0x16, 0x0A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1,
		0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xD1, 0x84, 0x3D, 0xFF, 0x47, 0x2D, 0x15, 0xFF, 0x02, 0x01,
		0x01, 0xFF, 0x41, 0x29, 0x13, 0xFF, 0xC8, 0x7E, 0x3A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1,
		0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0x33, 0x20, 0x0F, 0xFF,
		0x45, 0x2C, 0x14, 0xFF, 0xDB, 0x8A, 0x40, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1,
		0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF,
		0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1,
		0x4A, 0xFF, 0x4B, 0x2F, 0x16, 0xFF, 0x6D, 0x45, 0x20, 0xFF, 0xE8, 0x92, 0x43, 0xFF, 0x55, 0x36, 0x19, 0xFF, 0x7C, 0x4E, 0x24, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0x50, 0x50,
		0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xC9, 0x7F, 0x3A, 0xFF, 0x2E, 0x1D, 0x0D, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x31, 0x1F, 0x0E, 0xFF, 0xD6, 0x87,
		0x3E, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0xFF, 0xA1, 0x4A, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1,
		0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xEF, 0x97, 0x45, 0xFF,
		0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1,
		0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1,
		0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF,
		0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0xFF, 0xA1, 0x4A, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50,
		0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF,
		0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50,
		0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF, 0x50, 0x50, 0x50, 0xFF
	}; // const button_res_t button_select::is_focused::on
}

primitive::primitive()
{
m_border =
{
	{ 0, 0 },  { 1, 0 },  { 2, 0 },  { 3, 0 },  { 4, 0 },  { 5, 0 },  { 6, 0 },  { 7, 0 },  { 8, 0 },  { 9, 0 }, { 10, 0 },
	{ 11, 0 }, { 12, 0 }, { 13, 0 }, { 14, 0 }, { 15, 0 }, { 16, 0 }, { 17, 0 }, { 18, 0 }, { 19, 0 }, { 20, 0 },
	{ 0, 1 },																						   { 20, 1 },
	{ 0, 2 },																						   { 20, 2 },
	{ 0, 3 },																						   { 20, 3 },
	{ 0, 4 },																						   { 20, 4 },
	{ 0, 5 },																						   { 20, 5 },
	{ 0, 6 },																						   { 20, 6 },
	{ 0, 7 },																						   { 20, 7 },
	{ 0, 8 },																						   { 20, 8 },
	{ 0, 9 },																						   { 20, 9 },
	{ 0, 10 },																						   { 20, 10 },
	{ 0, 11 },																						   { 20, 11 },
	{ 0, 12 },  { 1, 12 },  { 2, 12 },  { 3, 12 },  { 4, 12 },  { 5, 12 },  { 6, 12 },  { 7, 12 },  { 8, 12 },  { 9, 12 }, { 10, 12 },
	{ 11, 12 }, { 12, 12 }, { 13, 12 }, { 14, 12 }, { 15, 12 }, { 16, 12 }, { 17, 12 }, { 18, 12 }, { 19, 12 }, { 20, 12 }
}; // const vector_coord_t primitive::m_border

m_letter =
{
		{ 9, 3 }, { 10, 3 }, { 11, 3 },

	{ 8, 4 },                { 11, 4 },

	{ 8, 5 }, { 9, 5 },

		{ 9, 6 }, { 10, 6 }, { 11, 6 },

							 { 11, 7 }, { 12, 7 },

	{ 8, 8 },                           { 12, 8 },

		{ 9, 9 }, { 10, 9 }, { 11, 9 }
}; // const vector_coord_t primitive::m_letter

}

} // namespace amped::resources
