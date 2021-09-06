#ifndef _CAPTURE_BITMAP_HPP_
#define _CAPTURE_BITMAP_HPP_


#include "pch.h"

#include "enumerate_windows.hpp"
#include "buffers_manager.hpp"


#define _SAVE_BITMAP_TO_FILE
#undef _SAVE_BITMAP_TO_FILE


namespace amped::win
{
	
namespace bm = amped::win::bmp;

class capture_dc
{
public:
	explicit capture_dc(const bm::handle_t h);

	~capture_dc();

	bool operator()(memory::buffer_image& buffer);
protected:
	void init();
private:
	bm::handle_t m_handle;
}; // class capture_dc

} // namespace amped::win


#endif // _CAPTURE_BITMAP_HPP_
