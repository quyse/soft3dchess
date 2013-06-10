#ifndef ___FIRE_H___
#define ___FIRE_H___

/*
‘айл содержит класс эффекта огн€.
Ёффект заключаетс€ в том, что частицы, установленные в начальные положени€,
разбегаютс€ со случайными ускорени€ми.
*/

#include "VertexFormats.h"
#include "Renderer.h"
#include "Camera.h"
#include "Surface.h"
#include "math.h"
#include <vector>

class Fire
{
	//включен?
	bool enabled;
	//врем€ начала
	int beginTime;
	//текущее врем€
	scalar time;
	//цвет частиц
	vector<3> color;

public:
	//вектор частиц
	std::vector<vector<3> > particles;

private:
	//вектор направлений частиц
	std::vector<vector<3> > directions;

public:
	Fire() : enabled(false) {}

	//инициализировать эффект заново
	void Initialize();

	//проверить окончание
	void CheckFinish();

	//нарисовать эффект
	void Render(const Camera& camera, Renderer<Surface::OutputDevice>& renderer, Surface::OutputDevice& outputDevice) const;
};

#endif
