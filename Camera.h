#pragma once

#include "Engine.h"

namespace Batman
{
	class Camera
	{
	private:
		//Projection, View and Rotation Matrices
		//and up direction
		Matrix p_matrixProj;
		Matrix p_matrixView;
		Matrix p_matrixRot;
		D3DXVECTOR3 p_updir;

		//Position, rotation and target vectors
		Vector3 p_position;
		Vector3 p_rotation;
		Vector3 p_target;

		//camera properties
		//near, far range, aspect ratio and FOV
		double p_nearRange;
		double p_farRange;
		double p_aspectRatio;
		double p_fov;

	public:
		//constructor / destructor
		Camera(void);
		~Camera(void);

		//set perspective
		void setPerspective(double fov, double aspectRatio,
							double nearRange, double farRange);

		//getters for view / projection matrices
		Matrix getProjMatrix() { return p_matrixProj; }
		Matrix getViewMatrix() { return p_matrixView; }

		//camera position get / set
		Vector3 getPosition() { return p_position; }
		void setPosition(double x, double y, double z);
		void setPosition(Vector3 vector) { p_position = vector; }

		//camera rotation setters
		void setRotation(double x, double y, double z);
		void setRotation(Vector3 vector) { p_rotation = vector; }

		//camera target getters and setters
		Vector3 getTarget() { return p_target; }
		void setTarget(Vector3 value) { p_target = value; }
		void setTarget(double x, double y, double z)
		{
			p_target.x=(float)x;
			p_target.y=(float)y;
			p_target.z=(float)z;
		}

		void Update();

		//rotate / look / move
		void Rotate(double x, double y, double z);
		void Look(double x, double y, double z);
		void Move(double x, double y, double z);
	};
};