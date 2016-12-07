#pragma once

#include "Engine.h"

namespace Batman
{
	//struct inherits from D3DXMATRIX
	struct Matrix:public D3DXMATRIX
	{
	public:
		//constructors and copy constructors
		Matrix();
		Matrix(const Matrix&);
		Matrix(const D3DXMATRIX&);
		Matrix& operator=(const Matrix&);
		Matrix& operator=(const D3DXMATRIX&);

		//set zero and identity matrices
		void setZero();
		void setIdentity();

		//fill matrices with values
		void Fill(int);
		void Fill(float);
		void Fill(double);

		//translate (move object position)
		void Translate(float x, float y, float z);
		void Translate(Vector3&);

		//rotate on yaw / pitch / roll
		void rotateYawPitchRoll(float x, float y, float z);
		void rotateYawPitchRoll(Vector3&);

		//rotate on axes
		void rotateX(float);
		void rotateY(float);
		void rotateZ(float);

		//scale
		void Scale(float x, float y, float z);
		void Scale(Vector3&);
	};
};