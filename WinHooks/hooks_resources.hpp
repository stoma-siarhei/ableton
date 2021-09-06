#ifndef _HOOKS_RESOURCES_HPP_
#define _HOOKS_RESOURCES_HPP_


#include "pch.h"

#include "hooks_headers.h"
#include "utils.hpp"


using namespace std;

namespace amped::resources
{

struct button_select
{
	static const button_res_t off;
	static const button_res_t on;

	struct is_focused
	{
		static const button_res_t off;
		static const button_res_t on;
	}; // struct is_focused

}; // struct button_select

template<class _Container>
class iterator;

template <size_t _Width = c_button_width, size_t _Height = c_button_height>
struct border_data
{
	border_data()
	{
		m_data = move(amped::utils::make_vector_pixel(button_select::off, m_border));
	}

	const vector_pixel_t& operator()() const
	{
		return add_const_t<vector_pixel_t&>(add_lvalue_reference_t<vector_pixel_t>(m_data));
	}
private:
	static const vector_coord_t m_border;
	vector_pixel_t m_data;
}; // struct border_data

template <size_t _Width, size_t _Height>
const vector_coord_t border_data<_Width, _Height>::m_border =
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
};

template <char _Letter = 'S'>
struct letter_data
{
	letter_data()
	{

	}
private:
	static const vector_coord_t m_letter;
	vector_pixel_t m_data;
}; // struct letter_data

template <char _Letter>
const vector_coord_t letter_data<_Letter>::m_letter =
{
		{ 9, 3 }, { 10, 3 }, { 11, 3 },

	{ 8, 4 },                { 11, 4 },

	{ 8, 5 }, { 9, 5 },

		{ 9, 6 }, { 10, 6 }, { 11, 6 },

	                         { 11, 7 }, { 12, 7 },

	{ 8, 8 },                           { 12, 8 },

		{ 9, 9 }, { 10, 9 }, { 11, 9 }
};

template<class _Container>
class iterator
{

}; // class iterator

} // namespace amped::resources


#endif // _HOOKS_RESOURCES_HPP_
