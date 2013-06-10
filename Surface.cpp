#include "Surface.h"
#include <algorithm>

Surface::OutputDevice::OutputDevice(Surface* surface, int left, int top, int right, int bottom, int width, int height)
: surface(surface), left(left), top(top), right(right), bottom(bottom), width(width), height(height)
{
}

bool Surface::OutputDevice::DepthTest(int x, int y, scalar z, bool write)
{
	return surface->DepthTest(x, y, z, write);
}

vector<3> Surface::OutputDevice::GetPixel(int x, int y) const
{
	return surface->GetPixel(x, y);
}

void Surface::OutputDevice::SetPixel(int x, int y, const vector<3>& color)
{
	surface->SetPixel(x, y, color);
}

Surface::Surface(int width, int height) : width(0), height(0), buffer(0), depthBuffer(0), linesPixelCount(0)
{
	Resize(width, height);
}

Surface::~Surface()
{
	Resize(0, 0);
}

int Surface::GetWidth() const
{
	return width;
}

int Surface::GetHeight() const
{
	return height;
}

void Surface::Resize(int width, int height)
{
	if(width == this->width && height == this->height)
		return;
	this->width = width;
	this->height = height;
#ifdef COLORBUFFER_32BIT
#else
	pitch = (width * 3 + 3) & ~3;
#endif
	if(buffer)
		delete [] buffer;
	if(depthBuffer)
		delete [] depthBuffer;
	if(linesPixelCount)
		delete [] linesPixelCount;
	if(width * height)
	{
#ifdef COLORBUFFER_32BIT
		buffer = new int[height * width];
#else
		buffer = new unsigned char[height * pitch];
#endif
#ifdef FLOAT_ZBUFFER
		depthBuffer = new float[height * width];
#else
		depthBuffer = new unsigned short[height * width];
#endif
		linesPixelCount = new int[height];
	}
	else
	{
		buffer = 0;
		depthBuffer = 0;
		linesPixelCount = 0;
	}
}

Surface::OutputDevice Surface::GetOutputDevice(int left, int top, int right, int bottom)
{
	return OutputDevice(this, left, top, right, bottom, width, height);
}

void Surface::Clear(const vector<3>& color, scalar depth)
{
#ifdef COLORBUFFER_32BIT
	int p =
		((int)(color.z * 255)) |
		(((int)(color.y * 255)) << 8) |
		(((int)(color.x * 255)) << 16);
	int* line = buffer;
	for(int i = 0; i < height; ++i)
	{
		int* pixel = line;
		for(int j = 0; j < width; ++j)
			*pixel++ = p;
		line += width;
	}
#else
	unsigned char p[3] =
	{
		(unsigned char)(color.z * 255),
		(unsigned char)(color.y * 255),
		(unsigned char)(color.x * 255)
	};
	unsigned char* line = buffer;
	for(int i = 0; i < height; ++i)
	{
		unsigned char* pixel = line;
		for(int j = 0; j < width; ++j)
		{
			for(int k = 0; k < 3; ++k)
				pixel[k] = p[k];
			pixel += 3;
		}
		line += pitch;
	}
#endif
	std::fill(depthBuffer, depthBuffer + width * height,
#ifdef FLOAT_ZBUFFER
		depth
#else
		(unsigned short)(depth * 0xFFFF)
#endif
		);
	std::fill(linesPixelCount, linesPixelCount + height, 0);
}

bool Surface::DepthTest(int x, int y, scalar z, bool write)
{
	if(z < 0 || z > 1) return false;
#ifdef FLOAT_ZBUFFER
	float& s = depthBuffer[y * width + x];
	if(s > z)
	{
		if(write)
			s = z;
		return true;
	}
#else
	unsigned short Z = (unsigned short)(z * 0xFFFF);
	unsigned short& s = depthBuffer[y * width + x];
	if(s > Z)
	{
		if(write)
			s = Z;
		return true;
	}
#endif
	return false;
}

vector<3> Surface::GetPixel(int x, int y) const
{
#ifdef COLORBUFFER_32BIT
	int p = buffer[y * width + x];
	return vector<3>((p >> 16) & 0xFF, (p >> 8) & 0xFF, p & 0xFF) / 255;
#else
	int p = y * pitch + x * 3;
	return vector<3>(buffer[p + 2], buffer[p + 1], buffer[p]) / 255;
#endif
}

void Surface::SetPixel(int x, int y, const vector<3>& color)
{
#ifdef COLORBUFFER_32BIT
	buffer[y * width + x] =
		((int)(color.z * 255)) |
		(((int)(color.y * 255)) << 8) |
		(((int)(color.x * 255)) << 16);
#else
	int p = y * pitch + x * 3;
	buffer[p] = (unsigned char)(color.z * 255);
	buffer[p + 1] = (unsigned char)(color.y * 255);
	buffer[p + 2] = (unsigned char)(color.x * 255);
#endif
	linesPixelCount[y]++;
}

void Surface::CalculateCoreLines(std::vector<scalar>& lines)
{
	for(int i = 1; i < height; ++i)
		linesPixelCount[i] += linesPixelCount[i - 1];
	for(int i = 0; i < lines.size(); ++i)
		lines[i] = scalar(std::lower_bound(linesPixelCount, linesPixelCount + height, linesPixelCount[height - 1] * (i + 1) / (lines.size() + 1)) - linesPixelCount) / height;
}

void Surface::Paint(HDC hdc, int x, int y)
{
	BITMAPINFOHEADER bih;
	ZeroMemory(&bih, sizeof(bih));
	bih.biSize = sizeof(bih);
	bih.biWidth = width;
	bih.biHeight = -height;
	bih.biPlanes = 1;
	bih.biBitCount =
#ifdef COLORBUFFER_32BIT
		32
#else
		24
#endif
		;
	StretchDIBits(hdc, x, y, width, height, 0, 0, width, height, buffer, (BITMAPINFO*)&bih, DIB_RGB_COLORS, SRCCOPY);
}
