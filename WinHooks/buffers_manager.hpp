#ifndef _BUFFERS_MANAGER_HPP_
#define _BUFFERS_MANAGER_HPP_


#include "pch.h"

#include "hooks_headers.h"


using namespace std;

namespace amped::memory
{

struct dimensions
{
	dimensions();

	template <class A, class B, class C, class D>
	dimensions(A a, B b, C c, D d)
	{
		width = static_cast<uint32_t>(a);
		height = static_cast<uint32_t>(b);
		bit = static_cast<uint32_t>(c);
		size_pixel = static_cast<uint32_t>(d);
	}

	dimensions operator=(dimensions dim);

	uint32_t size() const;

	template <int N>
	uint32_t get() const
	{
		if constexpr (N == 1)
		{
			return width;
		}
		else if constexpr (N == 2)
		{
			return height;
		}
		else if constexpr (N == 3)
		{
			return bit;
		}
		else if constexpr (N == 4)
		{
			return size_pixel;
		}
		else
		{
			uint32_t* t = new uint32_t[-1];
			return 0;
		}
	}
private:
	uint32_t width, height, bit, size_pixel;
}; // struct dimensions

class buffer_image
{
public:
	buffer_image();

	void* operator()(const dimensions& dim);

	uint32_t get_width() const;
	uint32_t get_width(const uint32_t w) const;

	uint32_t get_height() const;
	uint32_t get_height(const uint32_t h) const;

	uint32_t get_bit() const;

	uint32_t get_size_pixel() const;

protected:
private:
	dimensions m_dim;
	buffer_t m_buffer;
}; // class buffer_image

} // namespace amped::memory


#endif // _BUFFERS_MANAGER_HPP_
