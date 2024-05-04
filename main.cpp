#include "main.h"
#include "rc.h"
#include <filesystem>

Gdiplus::Image* Image = nullptr;
TCHAR imagepath[MAX_PATH] = { 0 };

void file_open(HWND parent)
{
	TCHAR path[MAX_PATH]; *path = 0;
	ZeroMemory(path, sizeof(path));
	TCHAR filter[] = _T("Image\0*.png;*\0*.jpeg;*\0*.bmp;*\0*.gif;*\0*.tiff;*\0*.emf\0");
	OPENFILENAME ofn{ sizeof OPENFILENAME };
	ofn.hwndOwner = parent;
	ofn.lpstrFile = path;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = filter;
	ofn.Flags = OFN_HIDEREADONLY;
	if (::GetOpenFileName(&ofn)) {
		Image = Gdiplus::Image::FromFile(path);
		_tcscpy_s(imagepath, MAX_PATH, path);
	}
}

void draw_string(Gdiplus::Graphics& g, const TCHAR* text, int x, int y) {
	Gdiplus::Font font(L"Arial", 12);
	Gdiplus::SolidBrush brush1(Gdiplus::Color(0, 0, 0));
	Gdiplus::SolidBrush brush2(Gdiplus::Color(255, 255, 255));
	Gdiplus::StringFormat format;
	Gdiplus::Rect TR;
	Gdiplus::PointF point1(x + 2, y + 2);
	g.DrawString(text, -1, &font, point1, &brush1);
	Gdiplus::PointF point2(x, y);
	g.DrawString(text, -1, &font, point2, &brush2);
}

void main_window::on_paint(HDC hdc) 
{
	RECT rc;
	GetClientRect(*this, &rc);
	int widht = (rc.right - rc.left)/4;
	int height = rc.bottom - rc.top;
	Gdiplus::Graphics g(hdc);
	g.DrawImage(Image,0,0, rc.right, rc.bottom);
	draw_string(g,imagepath,widht,height-50);

}

void main_window::on_command(int id) 
{
	switch (id) 
	{
		case ID_OPEN:
			file_open(*this);
			InvalidateRect(*this, NULL, true);
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
