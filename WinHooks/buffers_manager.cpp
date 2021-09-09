#include "pch.h"
#include "buffers_manager.hpp"


namespace amped::memory
{
	dimensions::dimensions()
		: width(0), height(0), bit(0), size_pixel(0)
	{
	}

	dimensions dimensions::operator=(dimensions dim)
{
	width = dim.width;
	height = dim.height;
	bit = dim.bit;
	size_pixel = dim.size_pixel;
	return *this;
}

uint32_t dimensions::size() const
{
	return ((width * bit + 31) / 32) * 4 * height;
}

buffer_image::buffer_image()
{
}

void* buffer_image::operator()(const dimensions& dim)
{
	m_dim = dim;
	if (uint32_t s = m_dim.size(); s != m_buffer.size()) m_buffer.resize(s);
	return static_cast<void*>(m_buffer.data());
}

uint32_t buffer_image::get_width() const
{
	return m_dim.get<1>();
}

uint32_t buffer_image::get_width(const uint32_t w) const
{
	return m_dim.get<1>() - w;
}

uint32_t buffer_image::get_height() const
{
	return m_dim.get<2>();
}

uint32_t buffer_image::get_height(const uint32_t h) const
{
	return m_dim.get<2>() - h;
}

uint32_t buffer_image::get_bit() const
{
	return m_dim.get<3>();
}

uint32_t buffer_image::get_size_pixel() const
{
	return m_dim.get<4>();
}

buffer_t buffer_image::get() const
{
    return m_buffer;
}

} // namespace amped::memory
