#pragma once

#include "Engine.h"

namespace Batman
{
	//as the classes refer to each other, prototypes here
	class Vector3;
	class Vector2;

	/**
	This define lets us use Point interchangeably with Vector2
	**/
	#define Point Vector2

	//3D vectors (x y z)
	class Vector3
	{
	public:
		double x, y, z;
		virtual ~Vector3() {}

		//constructors and overloads
		Vector3();									//default
		Vector3(const Vector3& V);					//copies existing Vector3
		Vector3(double x, double y, double z);		//uses double values
		Vector3(int x, int y, int z);				//uses int values
		Vector3(const D3DXVECTOR2& dv);				//uses DX 2D conversion
		Vector3(const D3DXVECTOR3& dv);				//uses DX 3D conversion

		//operator overload - constructs with copy of other Vector3 values
		//eg Vector3 newVector = otherVector;
		Vector3& operator=(const Vector3& V);

		//manipulation functions
		//set x, y, z with values or with copy
		void Set(double x1, double y1, double z1);
		void Set(const Vector3& V);

		//get and set x, y, z
		double getX() { return x; }
		void setX(double value) { x=value; }
		double getY() { return y; }
		void setY(double value) { y=value; }
		double getZ() { return z; }
		void setZ(double value) { z=value; }

		//move method
		void Move(double mx, double my, double mz);

		//operator overloads for vector arithmetic
		void operator+=(const Vector3& V);
		void operator-=(const Vector3& V);

		Vector3 operator+(const Vector3& V);
		Vector3 operator-(const Vector3& V);

		//scalar multiplication / division
		void operator*=(const double& d);
		void operator/=(const double& d);

		Vector3 operator*(const double& d);
		Vector3 operator/(const double& d);

		//comparison operators
		bool operator==(const Vector3& V) const;
		bool operator!=(const Vector3& V) const;

		//exporters to direct3d vectors
		D3DXVECTOR3 ToD3DXVECTOR3();
		D3DXVECTOR2 ToD3DXVECTOR2();
	};

	//2D vectors (x, y)
	class Vector2
	{
		public:
		double x, y;
		virtual ~Vector2() {}

		//constructors and overloads
		Vector2();									//default
		Vector2(const Vector2& V);					//copies existing Vector3
		Vector2(double x, double y);		//uses double values
		Vector2(int x, int y);				//uses int values
		Vector2(const D3DXVECTOR2& dv);				//uses DX 2D conversion
		Vector2(const D3DXVECTOR3& dv);				//uses DX 3D conversion

		//operator overload - constructs with copy of other Vector3 values
		//eg Vector3 newVector = otherVector;
		Vector2& operator=(const Vector2& V);

		//manipulation functions
		//set x, y, z with values or with copy
		void Set(double x1, double y1);
		void Set(const Vector2& V);

		//get and set x, y, z
		double getX() { return x; }
		void setX(double value) { x=value; }
		double getY() { return y; }
		void setY(double value) { y=value; }

		//move method
		void Move(double mx, double my);

		//operator overloads for vector arithmetic
		void operator+=(const Vector2& V);
		void operator-=(const Vector2& V);

		Vector2 operator+(const Vector2& V);
		Vector2 operator-(const Vector2& V);

		//scalar multiplication / division
		void operator*=(const double& d);
		void operator/=(const double& d);

		Vector2 operator*(const double& d);
		Vector2 operator/(const double& d);

		//comparison operators
		bool operator==(const Vector2& V) const;
		bool operator!=(const Vector2& V) const;

		//exporters to direct3d vectors
		D3DXVECTOR3 ToD3DXVECTOR3();
		D3DXVECTOR2 ToD3DXVECTOR2();
	};
};