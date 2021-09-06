#ifndef _UTILS_HPP_
#define _UTILS_HPP_


#include "pch.h"

#include "hooks_headers.h"
#include "hooks_resources.hpp"


using namespace std;

namespace amped::utils
{

namespace res = amped::resources;

namespace mem = amped::memory;

res::vector_pixel_t make_vector_pixel(const res::button_res_t& source, const res::vector_coord_t& coord)
{
	res::vector_pixel_t v{ coord.size() };
	size_t _index{ 0 };
	for (auto&& it : coord)
	{
		auto&& [w, h] = it;
		size_t i{ (h * res::c_button_width + w) * 4 };
		v[_index++] = { source[i], source[i + 1], source[i + 2], source[i + 3] };
	}
	return v;
}

template <size_t _Width, size_t _Height>
struct make_vector_pixel_ex
{
	make_vector_pixel_ex(mem::buffer_t& buff)
		: m_buffer(buff)
	{
	}

	res::vector_pixel_t operator()(const res::vector_coord_t& coord, const size_t pos) const
	{

	}
private:
	mem::buffer_t& m_buffer;
};

} // namespace amped::utils


#endif // _UTILS_HPP_
