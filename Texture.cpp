#include "Texture.h"
#include "windows.h"

Texture::Texture(unsigned char* buffer, int width, int height, int pitch)
: buffer(buffer), width(width), height(height), pitch(pitch)
{
}

Texture::~Texture()
{
	delete buffer;
}

int Texture::GetWidth() const
{
	return width;
}

int Texture::GetHeight() const
{
	return height;
}

inline scalar clamp01(scalar s)
{
	return s > 1 ? 1 : (s < 0 ? 0 : s);
}

vector<3> Texture::Sample(const vector<2>& coords) const
{
	int p = (int)(clamp01(coords.y/* + 1000*/) * (height - 1)) * pitch + (int)(clamp01(coords.x/* + 1000*/) * (width - 1)) * 3;
	return vector<3>(buffer[p + 2] / 255., buffer[p + 1] / 255., buffer[p] / 255.);
}

Texture* Texture::LoadFromFile(const wchar_t* fileName)
{
	HBITMAP image = (HBITMAP)LoadImage(NULL, fileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if(!image) return 0;

	BITMAP info;
	GetObject(image, sizeof(info), &info);
	int width = info.bmWidth;
	int height = info.bmHeight;
	if(height < 0) height = -height;
	int pitch = (width * 3 + 3) & ~3;

	unsigned char* buffer = new unsigned char[pitch * height];
	BITMAPINFOHEADER bih;
	ZeroMemory(&bih, sizeof(bih));
	bih.biSize = sizeof(bih);
	bih.biBitCount = 24;
	bih.biWidth = width;
	bih.biHeight = -height;
	bih.biPlanes = 1;

	HDC hdc = GetDC(NULL);
	GetDIBits(hdc, image, 0, height, buffer, (BITMAPINFO*)&bih, DIB_RGB_COLORS);
	ReleaseDC(NULL, hdc);

	return new Texture(buffer, width, height, pitch);
}
