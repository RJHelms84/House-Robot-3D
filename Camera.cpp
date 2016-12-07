#include "Engine.h"

namespace Batman
{
	Camera::Camera(void)
	{
		//create default perspective matrix
		//position at +10 on Z
		p_position = Vector3(0.0f,0.0f,10.0f);
		//Up direction is y
		p_updir = D3DXVECTOR3(0.0f,1.0f,0.0f);
		//aspect ratio
		double ratio = 640 / 480;
		//SEND - FOV, Aspect ratio, nearplane, farplane
		setPerspective(Batman::PI/4.0f, ratio, 1.0f, 10000.0f);
		//create default view matrix
		this->Update();
	}

	Camera::~Camera(void) { }

	void Camera::setPerspective(double fov, double aspectRatio,
								double nearRange, double farRange)
	{
		//set vars
		p_fov = fov;
		p_aspectRatio = aspectRatio;
		p_nearRange = nearRange;
		p_farRange = farRange;

		//set the camera's perspective matrix
		D3DXMatrixPerspectiveFovLH(&this->p_matrixProj, (float)p_fov,
									(double)p_aspectRatio, (float)p_nearRange,
									(float)p_farRange);
	}

	void Camera::Update()
	{
		//convert to D3D vectors
		D3DXVECTOR3 pos = p_position.ToD3DXVECTOR3();
		D3DXVECTOR3 target = p_target.ToD3DXVECTOR3();
		//send view matrix, pos, target and up direction
		D3DXMatrixLookAtLH(&p_matrixView, &pos, &target, &p_updir);
	}

	//set specific position values
	void Camera::setPosition(double x, double y, double z)
	{
		p_position.x=(float)x;
		p_position.y=(float)y;
		p_position.z=(float)z;
	}

	//set specific rotation values
	void Camera::setRotation(double x, double y, double z)
	{
		p_rotation.x=(float)x;
		p_rotation.y=(float)y;
		p_rotation.z=(float)z;

		//update rotation matrix
		D3DXMatrixRotationYawPitchRoll(&p_matrixRot, (float)p_rotation.x,
										(float)p_rotation.y, (float)p_rotation.z);
	}

	//adjust rotation relative to current rotation values
	void Camera::Rotate(double x, double y, double z)
	{
		p_rotation.x += (float)x;
		p_rotation.y += (float)y;
		p_rotation.z += (float)z;
	}

	//set target values relative to current
	void Camera::Look(double x, double y, double z)
	{
		p_target.x+=(float)x;
		p_target.y+=(float)y;
		p_target.z+=(float)z;
	}

	//move relative position and target
	void Camera::Move(double x, double y, double z)
	{
		p_position.x += (float)x;
		p_position.y += (float)y;
		p_position.z += (float)z;

		p_target.x+=(float)x;
		p_target.y+=(float)y;
		p_target.z+=(float)z;
	}
};