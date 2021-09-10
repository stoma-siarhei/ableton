#ifndef _HOOKS_RESOURCES_HPP_
#define _HOOKS_RESOURCES_HPP_


#include "pch.h"

#include "hooks_headers.h"


using namespace std;

namespace amped::resources
{

namespace mem = amped::memory;

vector_pixel_t make_vector_pixel(const button_res_t& source, const vector_coord_t& coord);

struct button_select
{
	button_select();

	button_res_t off;
	button_res_t on;

	button_res_t off_focus;
	button_res_t on_focus;
}; // struct button_select

static button_select s_button_select;

struct primitive
{
	primitive();

	vector_coord_t m_border;
	vector_coord_t m_letter;
}; // struct primitive

static primitive s_primitive;

template<class _Container>
class iterator;

template <size_t _Width = c_button_width, size_t _Height = c_button_height>
struct border_data
{
	border_data()
	{
		m_data_off = move(make_vector_pixel(s_button_select.off, s_primitive.m_border));
		m_data_on = move(make_vector_pixel(s_button_select.on, s_primitive.m_border));
	}

	const vector_pixel_t& operator()(const bool is_active) const
	{
		if (is_active) return add_const_t<vector_pixel_t&>(add_lvalue_reference_t<vector_pixel_t>(m_data_on));
		return add_const_t<vector_pixel_t&>(add_lvalue_reference_t<vector_pixel_t>(m_data_off));
	}

private:
	vector_pixel_t m_data_off, m_data_on;
}; // struct border_data

template <char _Letter = 'S'>
struct letter_data
{
	letter_data()
	{
		m_data_off = move(make_vector_pixel(s_button_select.off, s_primitive.m_letter));
		m_data_on = move(make_vector_pixel(s_button_select.on, s_primitive.m_letter));
	}

	const vector_pixel_t& operator()(const bool is_active) const
	{
		if (is_active) return add_const_t<vector_pixel_t&>(add_lvalue_reference_t<vector_pixel_t>(m_data_on));
		return add_const_t<vector_pixel_t&>(add_lvalue_reference_t<vector_pixel_t>(m_data_off));
	}

private:
	vector_pixel_t m_data_off, m_data_on;
}; // struct letter_data

static border_data<c_button_width, c_button_height> s_border_data;

static letter_data<'S'> s_letter_data;

template<class _Container>
class iterator
{

}; // class iterator

struct vector_pixels
{
	vector_pixels(mem::buffer_t& buff);

	bool operator()(const coord_t& coord, const size_t pos = 0);

	coord_t get() const;
protected:
	void make_pixel(const size_t _pos, const size_t _width);
	bool compare() const;
private:
	vector_pixel_t m_pixel_border_s, m_pixel_letter_s, m_pixel_border, m_pixel_letter, m_delta_border, m_delta_letter;
	mem::buffer_t& m_buffer;
	coord_t m_coord;
};

} // namespace amped::resources


#endif // _HOOKS_RESOURCES_HPP_
