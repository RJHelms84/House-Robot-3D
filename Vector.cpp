#include "Engine.h"

namespace Batman
{
									//BEGIN: Vector3
	//BEGIN: Vector3 constructors
	//default - set all values at 0
	Vector3::Vector3()
	{
		x = y = z = 0;
	}

	//copy existing Vector3
	Vector3::Vector3(const Vector3& V)
	{
		*this = V;
	}

	//take doubles and use Set method
	Vector3::Vector3(double x, double y, double z)
	{
		Set(x, y, z);
	}

	//take ints and cast to doubles for Set method
	Vector3::Vector3(int x, int y, int z)
	{
		Set((double)x, (double)y, (double)z);
	}

	//copy from DX 2D Vector (set z=0)
	Vector3::Vector3(const D3DXVECTOR2& dv)
	{
		x=dv.x; y=dv.y; z=0.0;
	}

	//copy from DX 3D Vector
	Vector3::Vector3(const D3DXVECTOR3& dv)
	{
		x=dv.x; y=dv.y; z=dv.z;
	}

	//operator overload for copy construct
	Vector3& Vector3::operator=(const Vector3& V)
	{
		x=V.x; y=V.y; z=V.z;
		return *this;
	}

	//END: Vector3 Constructors

	//BEGIN: Vector3 Set Methods

	//set using double values
	void Vector3::Set(double x1, double y1, double z1)
	{
		x=x1; y=y1; z=z1;
	}

	//set using existing Vector3 values
	void Vector3::Set(const Vector3& V)
	{
		x=V.x; y=V.y; z=V.z;
	}

	//END: Vector 3 Set methods

	//Move method; vector addition on 3 axes
	void Vector3::Move(double mx, double my, double mz)
	{
		x+=mx; y+=my; z+=mz;
	}

	//BEGIN: operator overloads for vector arithmetic

	//These 2 store the result in the vector on the left
	//add to existing vector
	void Vector3::operator+=(const Vector3& V)
	{
		x+=V.x; y+=V.y; z+=V.z;
	}

	//subtract from existing vector
	void Vector3::operator-=(const Vector3& V)
	{
		x-=V.x; y-=V.y; z-=V.z;
	}

	//these 2 return a vector value from the calculation
	//add vectors and return result vector
	Vector3 Vector3::operator+(const Vector3& V)
	{
		Vector3 v = Vector3(x+V.x, y+V.y, z+V.z);
		return v;
	}

	//subtract vectors and return result vector
	Vector3 Vector3::operator-(const Vector3& V)
	{
		Vector3 v = Vector3(x-V.x, y-V.y, z-V.z);
		return v;
	}

	//END: operator overloads for vector arithmetic

	//BEGIN: operator overloads for scalar operations

	//divide to existing vector
	void Vector3::operator/=(const double& d)
	{
		x/=d; y/=d; z/=d;
	}

	//multiply to existing vector
	void Vector3::operator*=(const double& d)
	{
		x*=d; y*=d; z*=d;
	}

	//divide with vector and return result vector
	Vector3 Vector3::operator/(const double& d)
	{
		Vector3 v = Vector3(x/d, y/d, z/d);
		return v;
	}

	//multiply with vector and return results vector
	Vector3 Vector3::operator*(const double& d)
	{
		Vector3 v = Vector3(x*d, y*d, z*d);
		return v;
	}
	//END: operator overloads for scalar operations

	//BEGIN: Comparison operations
	//equality operator comparison includes double rounding
	bool Vector3::operator==(const Vector3& V) const
	{
		//coords within range of 0.0001 are considered equal
		return(	( ((V.x - 0.0001f) < x)) && (x < (V.x + 0.0001f) ) &&
				( ((V.y - 0.0001f) < y)) && (y < (V.y + 0.0001f) ) &&
				( ((V.z - 0.0001f) < z)) && (z < (V.z + 0.0001f) )	);
	}

	//inequality operator
	bool Vector3::operator!=(const Vector3& V) const
	{
		//check based on == overload result
		return(!(*this==V));
	}
	//END: comparison operators

	//BEGIN: D3D conversions
	D3DXVECTOR3 Vector3::ToD3DXVECTOR3()
	{
		return D3DXVECTOR3((float)x, (float)y, (float)z);
	}

	D3DXVECTOR2 Vector3::ToD3DXVECTOR2()
	{
		return D3DXVECTOR2((float)x, (float)y);
	}
	//END:: D3D Conversions

								//END: Vector3

							//BEGIN: Vector2
	//BEGIN: Vector2 constructors
	//default - set all values at 0
	Vector2::Vector2()
	{
		x = y = 0;
	}

	//copy existing Vector2
	Vector2::Vector2(const Vector2& V)
	{
		*this = V;
	}

	//take doubles and use Set method
	Vector2::Vector2(double x, double y)
	{
		Set(x, y);
	}

	//take ints and cast to doubles for Set method
	Vector2::Vector2(int x, int y)
	{
		Set((double)x, (double)y);
	}

	//copy from DX 2D Vector (set z=0)
	Vector2::Vector2(const D3DXVECTOR2& dv)
	{
		x=dv.x; y=dv.y;
	}

	//copy from DX 3D Vector
	Vector2::Vector2(const D3DXVECTOR3& dv)
	{
		x=dv.x; y=dv.y;
	}

	//operator overload for copy construct
	Vector2& Vector2::operator=(const Vector2& V)
	{
		x=V.x; y=V.y;
		return *this;
	}

	//END: Vector2 Constructors

	//BEGIN: Vector2 Set Methods

	//set using double values
	void Vector2::Set(double x1, double y1)
	{
		x=x1; y=y1;
	}

	//set using existing Vector2 values
	void Vector2::Set(const Vector2& V)
	{
		x=V.x; y=V.y;
	}

	//END: Vector 2 Set methods

	//Move method; vector addition on 2 axes
	void Vector2::Move(double mx, double my)
	{
		x+=mx; y+=my;
	}

	//BEGIN: operator overloads for vector arithmetic

	//These 2 store the result in the vector on the left
	//add to existing vector
	void Vector2::operator+=(const Vector2& V)
	{
		x+=V.x; y+=V.y;
	}

	//subtract from existing vector
	void Vector2::operator-=(const Vector2& V)
	{
		x-=V.x; y-=V.y;
	}

	//these 2 return a vector value from the calculation
	//add vectors and return result vector
	Vector2 Vector2::operator+(const Vector2& V)
	{
		Vector2 v = Vector2(x+V.x, y+V.y);
		return v;
	}

	//subtract vectors and return result vector
	Vector2 Vector2::operator-(const Vector2& V)
	{
		Vector2 v = Vector2(x-V.x, y-V.y);
		return v;
	}

	//END: operator overloads for vector arithmetic

	//BEGIN: operator overloads for scalar operations

	//divide to existing vector
	void Vector2::operator/=(const double& d)
	{
		x/=d; y/=d;
	}

	//multiply to existing vector
	void Vector2::operator*=(const double& d)
	{
		x*=d; y*=d;
	}

	//divide with vector and return result vector
	Vector2 Vector2::operator/(const double& d)
	{
		Vector2 v = Vector2(x/d, y/d);
		return v;
	}

	//multiply with vector and return results vector
	Vector2 Vector2::operator*(const double& d)
	{
		Vector2 v = Vector2(x*d, y*d);
		return v;
	}
	//END: operator overloads for scalar operations

	//BEGIN: Comparison operations
	//equality operator comparison includes double rounding
	bool Vector2::operator==(const Vector2& V) const
	{
		//coords within range of 0.0001 are considered equal
		return(	( ((V.x - 0.0001f) < x)) && (x < (V.x + 0.0001f) ) &&
				( ((V.y - 0.0001f) < y)) && (y < (V.y + 0.0001f) )	);
	}

	//inequality operator
	bool Vector2::operator!=(const Vector2& V) const
	{
		//check based on == overload result
		return(!(*this==V));
	}
	//END: comparison operators

	//BEGIN: D3D conversions
	D3DXVECTOR3 Vector2::ToD3DXVECTOR3()
	{
		return D3DXVECTOR3((float)x, (float)y, 0.0f);
	}

	D3DXVECTOR2 Vector2::ToD3DXVECTOR2()
	{
		return D3DXVECTOR2((float)x, (float)y);
	}
	//END:: D3D Conversions

								//END: Vector3
};