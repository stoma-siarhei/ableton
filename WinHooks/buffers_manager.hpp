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

	template <class A, class B, class C, class D, class E, class F>
	dimensions(A a, B b, C c, D d, E e, F f)
	{
		width = static_cast<uint32_t>(a);
		height = static_cast<uint32_t>(b);
		bit = static_cast<uint32_t>(c);
		size_pixel = static_cast<uint32_t>(d);
		left = static_cast<int>(e);
		top = static_cast<int>(f);
	}

	dimensions operator=(dimensions dim);

	uint32_t size() const;

	template <int N>
	auto get() const
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
		else if constexpr (N == 5)
		{
			return left;
		}
		else if constexpr (N == 6)
		{
			return top;
		}
		else
		{
			uint32_t* t = new uint32_t[-1];
			return 0;
		}
	}
private:
	uint32_t width, height, bit, size_pixel;
	int left, top;
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

	int get_left() const;

	int get_top() const;

	buffer_t&& get() const;
protected:
private:
	dimensions m_dim;
	buffer_t m_buffer;
}; // class buffer_image

class manager
{
	using buffers_t = vector<buffer_image>;
public:
	manager() noexcept;

	~manager();

	size_t get() const noexcept;

	void set(size_t size);

	buffer_image& operator[](const size_t index) const;
protected:
private:
	buffers_t m_buffers;
}; // class manager

} // namespace amped::memory


#endif // _BUFFERS_MANAGER_HPP_
