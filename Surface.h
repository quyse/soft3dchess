#ifndef ___SURFACE_H___
#define ___SURFACE_H___

#include "windows.h"
#include "math.h"
#include <vector>

/*
‘айл содержит объ€вление внеэкранного буфера.
*/

//#define FLOAT_ZBUFFER
#define COLORBUFFER_32BIT

class Surface
{
private:
	int width, height;
#ifdef COLORBUFFER_32BIT
	int* buffer;
#else
	unsigned char* buffer;
	int pitch;
#endif
#ifdef FLOAT_ZBUFFER
	float* depthBuffer;
#else
	unsigned short* depthBuffer;
#endif
	int* linesPixelCount;

public:
	class OutputDevice
	{
	private:
		Surface* surface;

	public:
		int left, top, right, bottom, width, height;

		OutputDevice(Surface* surface, int left, int top, int right, int bottom, int width, int height);
		bool DepthTest(int x, int y, scalar z, bool write);
		vector<3> GetPixel(int x, int y) const;
		void SetPixel(int x, int y, const vector<3>& color);
	};

public:
	Surface(int width, int height);
	~Surface();

	int GetWidth() const;
	int GetHeight() const;
	void Resize(int width, int height);
	OutputDevice GetOutputDevice(int left, int top, int right, int bottom);

	void Clear(const vector<3>& color, scalar depth = 1);
	bool DepthTest(int x, int y, scalar z, bool write);
	vector<3> GetPixel(int x, int y) const;
	void SetPixel(int x, int y, const vector<3>& color);
	//вычислить разделительные линии дл€ €дер (lines.size() - означает требуемое количество линий)
	void CalculateCoreLines(std::vector<scalar>& lines);
	void Paint(HDC hdc, int x, int y);
};

#endif
