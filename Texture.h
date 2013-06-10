#ifndef ___TEXTURE_H___
#define ___TEXTURE_H___

#include "math.h"

/*
Файл содержит класс текстуры.
*/

class Texture
{
private:
	unsigned char* buffer;
	int width, height, pitch;

	Texture(unsigned char* buffer, int width, int height, int pitch);

public:
	~Texture();

	int GetWidth() const;
	int GetHeight() const;

	vector<3> Sample(const vector<2>& coords) const;

	static Texture* LoadFromFile(const wchar_t* fileName);
};

#endif
