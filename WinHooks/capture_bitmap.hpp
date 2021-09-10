#ifndef _CAPTURE_BITMAP_HPP_
#define _CAPTURE_BITMAP_HPP_


#include "pch.h"

#include "enumerate_windows.hpp"
#include "buffers_manager.hpp"


#define _SAVE_BITMAP_TO_FILE
#undef _SAVE_BITMAP_TO_FILE_

#define _SLICING_POLYGON
#undef _SLICING_POLYGON_


namespace amped::win
{
	
namespace bm = amped::win::bmp;

class capture_dc
{
public:
	explicit capture_dc(const bm::handle_t h);

	~capture_dc();

#ifdef _SLICING_POLYGON
	bool operator()(memory::manager& manager) const;
	bool operator()(memory::manager& manager, const bm::rect_t& rect) const;
#else
	bool operator()(memory::buffer_image& buffer);
#endif // _SLICING_POLYGON
protected:
	void init();
private:
	bm::handle_t m_handle;
}; // class capture_dc

} // namespace amped::win


#endif // _CAPTURE_BITMAP_HPP_
