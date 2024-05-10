#include "main.h"
#include "rc.h"
#include <filesystem>

void main_window::on_paint(HDC hdc) 
{
	RECT rc;
	GetClientRect(*this, &rc);
	Gdiplus::Graphics g(hdc);
	g.DrawImage(Image.get(), 0, 0, rc.right, rc.bottom);

	Gdiplus::Font font(L"Arial", 12);
	//Gdiplus::SolidBrush brush1(Gdiplus::Color(0, 0, 0));
	//Gdiplus::SolidBrush brush2(Gdiplus::Color(255, 255, 255));
	Gdiplus::SolidBrush brush1(Gdiplus::Color::Black);
	Gdiplus::SolidBrush brush2(Gdiplus::Color::White);
	Gdiplus::StringFormat fmat;
	Gdiplus::RectF TR(rc.left, rc.top, rc.right, rc.bottom);
	Gdiplus::RectF TS(rc.left-2, rc.top, rc.right+2, rc.bottom);
	fmat.SetAlignment(Gdiplus::StringAlignmentCenter);
	fmat.SetLineAlignment(Gdiplus::StringAlignmentFar);

	std::filesystem::path filePath(imagePath);
	std::wstring fileName = filePath.filename();
	
	//int widht = (rc.right - rc.left) / 2.0f;
	//int height = rc.bottom-70;

	//Gdiplus::PointF point1(); //iskoristiti alignment
	g.DrawString(fileName.c_str(), -1, &font, TR, &fmat, &brush1);
	//Gdiplus::PointF point2();
	g.DrawString(fileName.c_str(), -1, &font, TS, &fmat, &brush2);

}

void main_window::on_command(int id) 
{
	switch (id) 
	{
	case ID_OPEN: {

		TCHAR path[MAX_PATH]; *path = 0;
		ZeroMemory(path, sizeof(path));
		TCHAR filter[] = _T("Image Files\0*.jpeg;*.jpg;*.png;*.bmp;*.gif;*.tif;*.tiff;*.emf\0");
		OPENFILENAME ofn{ sizeof OPENFILENAME };
		ofn.hwndOwner = *this;
		ofn.lpstrFile = path;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrFilter = filter;
		ofn.Flags = OFN_HIDEREADONLY;
		if (::GetOpenFileName(&ofn)) {
			auto newImage = Gdiplus::Image::FromFile(path);
			if (newImage->GetLastStatus() == Gdiplus::Ok)
			{
				imagePath = path;
				Image.reset(newImage);
			}
			InvalidateRect(*this, NULL, true);
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
