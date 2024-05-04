#include "main.h"
#include "rc.h"
#include <filesystem>
#include <gdiplus.h>

void main_window::on_paint(HDC hdc) {
	RECT rect;  // Lef, top, right, bottom
	GetClientRect(*this, &rect);
	Gdiplus::RectF layoutRect( rect.left, rect.top, rect.right, rect.bottom);  // X, Y, Width, Height
	Gdiplus::Graphics graphics(hdc);

	// Image
	graphics.DrawImage(image, layoutRect);

	
	if (image) {
		Gdiplus::StringFormat stringFormat;
		stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
		Gdiplus::Font font(_T("Arial"), 24, 1);  // style 1 = bold

		// Shadow
		Gdiplus::Color color(Gdiplus::ARGB(0xFF000000));  // BLACK
		Gdiplus::SolidBrush solidBrush(color);

		layoutRect.X = rect.left + 3;
		layoutRect.Y = rect.bottom - 40;
		layoutRect.Width = rect.right;
		layoutRect.Height = rect.bottom;

		graphics.DrawString(fileName, -1, &font, layoutRect, &stringFormat, &solidBrush);

		// Text
		layoutRect.X = rect.left;
		layoutRect.Y = rect.bottom - 43;
		layoutRect.Width = rect.right;
		layoutRect.Height = rect.bottom;

		color.SetValue(Gdiplus::ARGB(0xFFFFFFFF));  // WHITE
		solidBrush.SetColor(color);

		graphics.DrawString(fileName, -1, &font, layoutRect, &stringFormat, &solidBrush);
	}
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
			ofn.lpstrFile = path;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrFilter = filter;
			ofn.Flags = OFN_HIDEREADONLY;
			if (::GetOpenFileName(&ofn)) {  // If specified file name and clicked on OK button: returns nonzero, OPENFILENAME contains full path and name.
				image = Gdiplus::Image::FromFile(ofn.lpstrFile);
				_tcscpy_s(fileName, std::filesystem::path(ofn.lpstrFile).filename().c_str());
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

main_window::main_window() : image{ nullptr } {}

int WINAPI _tWinMain(HINSTANCE instance, HINSTANCE, LPTSTR, int)
{
	gdiplus_application app;
	main_window wnd;
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP 7"), (int64_t)LoadMenu(instance, MAKEINTRESOURCE(IDM_MAIN)));
	return app.run();
}
