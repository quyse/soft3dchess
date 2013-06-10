#ifndef ___FIRE_H___
#define ___FIRE_H___

/*
���� �������� ����� ������� ����.
������ ����������� � ���, ��� �������, ������������� � ��������� ���������,
����������� �� ���������� �����������.
*/

#include "VertexFormats.h"
#include "Renderer.h"
#include "Camera.h"
#include "Surface.h"
#include "math.h"
#include <vector>

class Fire
{
	//�������?
	bool enabled;
	//����� ������
	int beginTime;
	//������� �����
	scalar time;
	//���� ������
	vector<3> color;

public:
	//������ ������
	std::vector<vector<3> > particles;

private:
	//������ ����������� ������
	std::vector<vector<3> > directions;

public:
	Fire() : enabled(false) {}

	//���������������� ������ ������
	void Initialize();

	//��������� ���������
	void CheckFinish();

	//���������� ������
	void Render(const Camera& camera, Renderer<Surface::OutputDevice>& renderer, Surface::OutputDevice& outputDevice) const;
};

#endif
