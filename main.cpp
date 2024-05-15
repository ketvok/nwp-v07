#include "main.h"
#include "rc.h"
#include <filesystem>
#include <gdiplus.h>

void main_window::on_paint(HDC hdc) {
	if (!image) return;  // Check if points to 0.

	RECT rect;  // Lef, top, right, bottom
	GetClientRect(*this, &rect);
	Gdiplus::RectF layoutRect(rect.left, rect.top, rect.right, rect.bottom);  // X, Y, Width, Height
	Gdiplus::Graphics graphics(hdc);

	// Image
	graphics.DrawImage(image.get(), layoutRect);  // '.get()' gets underlying ('raw') pointer to the managed object.

	// Text formatting
	Gdiplus::StringFormat stringFormat;
	stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);  // Center
	stringFormat.SetLineAlignment(Gdiplus::StringAlignmentFar);  // Bottom
	Gdiplus::Font font(_T("Arial"), 24, Gdiplus::FontStyleBold);

	// Shadow
	Gdiplus::SolidBrush solidBrush(Gdiplus::Color::Black);

	graphics.DrawString(fileName.c_str(), -1, &font, layoutRect, &stringFormat, &solidBrush);

	// Text
	// To get equal shadow tickness both horizontally and vertically,
	// offset X and Y instead of changing Width and Height.
	layoutRect.X += -3;
	layoutRect.Y += -3;
	
	solidBrush.SetColor(Gdiplus::Color::White);

	graphics.DrawString(fileName.c_str(), -1, &font, layoutRect, &stringFormat, &solidBrush);
}

bool main_window::on_erase_bkgnd(HDC hdc) {
	return 1;
}

void main_window::on_command(int id) 
{
	switch (id) 
	{
		case ID_OPEN: {
			TCHAR path[MAX_PATH]; *path = 0;
			TCHAR filter[] = _T("Image Files (*.gif;*.jpg;*.png;*.bmp;*emf)\0*.gif;*.jpg;*.jpeg;*.png;*.bmp;*.emf\0");
			OPENFILENAME ofn{ sizeof OPENFILENAME };
			ofn.hwndOwner = *this;
			ofn.lpstrFile = path;  // lpstrFile points to the same location as path.
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrFilter = filter;
			ofn.Flags = OFN_HIDEREADONLY;
			if (::GetOpenFileName(&ofn)) {  // If specified file name and clicked on OK button: returns nonzero, OPENFILENAME contains full path and name.
				image = std::make_unique<Gdiplus::Image>(path);  // std::make_unique <object_type> (arguments)
				fileName = std::filesystem::path(path).filename();
				InvalidateRect(*this, 0, TRUE);
			}
		}
				break;
		case ID_EXIT:
			DestroyWindow(*this);
			break;
	}
}

void main_window::on_destroy() 
{
	::PostQuitMessage(0);
}

int WINAPI _tWinMain(HINSTANCE instance, HINSTANCE, LPTSTR, int)
{
	gdiplus_application app;
	main_window wnd;
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP 7"), (int64_t)LoadMenu(instance, MAKEINTRESOURCE(IDM_MAIN)));
	return app.run();
}
