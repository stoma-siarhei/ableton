#include "pch.h"
#include "capture_bitmap.hpp"


int CaptureAnImage(HWND hWnd)
{
	HDC hdcWindow;
	HDC hdcMemDC = NULL;
	HBITMAP hbmScreen = NULL;
	BITMAP bmpScreen;
	DWORD dwBytesWritten = 0;
	DWORD dwSizeofDIB = 0;
	HANDLE hFile = NULL;
	char* lpbitmap = NULL;
	HANDLE hDIB = NULL;
	DWORD dwBmpSize = 0;

	// Retrieve the handle to a display device context for the client 
	// area of the window. 
	hdcWindow = GetDC(hWnd);

	// Create a compatible DC, which is used in a BitBlt from the window DC.
	hdcMemDC = CreateCompatibleDC(hdcWindow);

	if (!hdcMemDC)
	{
		MessageBox(hWnd, L"CreateCompatibleDC has failed", L"Failed", MB_OK);
		goto done;
	}

	// Get the client area for size calculation.
	RECT rcClient;
	GetClientRect(hWnd, &rcClient);

	// This is the best stretch mode.
	SetStretchBltMode(hdcWindow, HALFTONE);

	// Create a compatible bitmap from the Window DC.
	hbmScreen = CreateCompatibleBitmap(hdcWindow, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);

	if (!hbmScreen)
	{
		MessageBox(hWnd, L"CreateCompatibleBitmap Failed", L"Failed", MB_OK);
		goto done;
	}

	// Select the compatible bitmap into the compatible memory DC.
	SelectObject(hdcMemDC, hbmScreen);

	// Bit block transfer into our compatible memory DC.
	if (!BitBlt(hdcMemDC,
		0, 0,
		// rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
		21, 13,
		hdcWindow,
		// 0, 0,
		899, 117,
		SRCCOPY))
	{
		MessageBox(hWnd, L"BitBlt has failed", L"Failed", MB_OK);
		goto done;
	}

	// Get the BITMAP from the HBITMAP.
	GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);

	BITMAPFILEHEADER   bmfHeader;
	BITMAPINFOHEADER   bi;

	bi.biSize = sizeof(BITMAPINFOHEADER);
	// bi.biWidth = bmpScreen.bmWidth;
	// bi.biHeight = bmpScreen.bmHeight;
	bi.biWidth = 21;
	bi.biHeight = 13;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;
	// dwBmpSize = ((21 * bi.biBitCount + 31) / 32) * 4 * 13;

	// Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that 
	// call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc 
	// have greater overhead than HeapAlloc.
	hDIB = GlobalAlloc(GHND, dwBmpSize);
	lpbitmap = (char*)GlobalLock(hDIB);

	// Gets the "bits" from the bitmap, and copies them into a buffer 
	// that's pointed to by lpbitmap.
	GetDIBits(hdcWindow, hbmScreen, 0,
		(UINT)bmpScreen.bmHeight,
		lpbitmap,
		(BITMAPINFO*)&bi, DIB_RGB_COLORS);

	// A file is created, this is where we will save the screen capture.
	hFile = CreateFile(L"D:\\captureqwsx.bmp",
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);

	// Add the size of the headers to the size of the bitmap to get the total file size.
	dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	// Offset to where the actual bitmap bits start.
	bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

	// Size of the file.
	bmfHeader.bfSize = dwSizeofDIB;

	// bfType must always be BM for Bitmaps.
	bmfHeader.bfType = 0x4D42; // BM.

	WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);

	// Unlock and Free the DIB from the heap.
	GlobalUnlock(hDIB);
	GlobalFree(hDIB);

	// Close the handle for the file that was created.
	CloseHandle(hFile);
	// Clean up.
done:
	DeleteObject(hbmScreen);
	DeleteObject(hdcMemDC);
	ReleaseDC(hWnd, hdcWindow);

	return 0;
}

namespace amped::win
{

namespace bm = amped::win::bmp;

capture_dc::capture_dc(const bm::handle_t h) 
	: m_handle(h)
{
	CaptureAnImage(h);
}

capture_dc::~capture_dc()
{
}

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

void capture_dc::init()
{
}

} // namespace amped::win
