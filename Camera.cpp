#include "Camera.h"

const scalar pi = 3.1415926535897932;

Camera::Camera() : nearClip(1), farClip(10000), verticalAngle(pi / 4), viewProjTransformAvailable(false)
{
}

void Camera::SetScreenSize(int width, int height)
{
	this->width = width;
	this->height = height;
	viewProjTransformAvailable = false;
}

vector<3> Camera::GetPosition() const
{
	return position;
}

void Camera::SetPosition(const vector<3>& position)
{
	this->position = position;
	viewProjTransformAvailable = false;
}

void Camera::SetTarget(const vector<3>& target)
{
	this->target = target;
	viewProjTransformAvailable = false;
}

void Camera::Update() const
{
	if(viewProjTransformAvailable)
		return;

	matrix<4, 4> view = CreateLookAtMatrix(position, target, vector<3>(0, 0, 1));
	projTransform = CreateProjectionPerspectiveFovMatrix(verticalAngle, scalar(width) / height, nearClip, farClip);

	viewProjTransform = view * projTransform;
	viewProjTransformAvailable = true;
}

matrix<4, 4> Camera::GetProjTransform() const
{
	Update();
	return projTransform;
}

matrix<4, 4> Camera::GetViewProjTransform() const
{
	Update();
	return viewProjTransform;
}
