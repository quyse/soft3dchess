#ifndef ___CAMERA_H___
#define ___CAMERA_H___

#include "math.h"

class Camera
{
	int width, height;
	scalar nearClip, farClip;
	scalar verticalAngle;
	vector<3> position;
	vector<3> target;
	mutable matrix<4, 4> projTransform;
	mutable matrix<4, 4> viewProjTransform;
	mutable bool viewProjTransformAvailable;

public:
	Camera();

	void SetScreenSize(int width, int height);
	vector<3> GetPosition() const;
	void SetPosition(const vector<3>& position);
	void SetTarget(const vector<3>& target);

	void Update() const;
	matrix<4, 4> GetProjTransform() const;
	matrix<4, 4> GetViewProjTransform() const;
};

#endif
