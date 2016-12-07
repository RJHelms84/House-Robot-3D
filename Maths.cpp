#include "Engine.h"

namespace Batman
{
	//convert from rads to deg
	double Math::ToDegrees(double radians)
	{
		//degrees = rads * 180/pi
		return radians * PI_under_180;
	}

	//convert from deg to rads
	double Math::ToRadians(double degrees)
	{
		//radians = deg * pi/180
		return degrees * PI_over_180;
	}

	//wrap angle value - degrees
	double Math::WrapAngleDegs(double degs)
	{
		//returns remainder of degs / 360
		double result = fmod(degs, 360.0);
		//if less than 0, adds 360 to result
		if(result<0) result += 360.0f;
		return result;
	}

	//wrap angle value - rads
	double Math::WrapAngleRads(double rads)
	{
		//returns remainder of rads / PI
		double result = fmod(rads, PI);
		//if less than 0, adds PI to result
		if(result<0) result+=PI;
		return result;
	}

	//Wrap value according to range
	double Math::WrapValue(double value, double min, double max)
	{
		//if value less than min, set as max
		if(value<min) value = max;
		//alternatively, if greater than max, set as min
		else if (value>max) value = min;
		return value;
	}

	//Limit value according to range
	double Math::Limit(double value, double min, double max)
	{
		//if value less than min, set as min
		if(value<min) value = min;
		//alternatively, if greater than max, set as max
		else if(value>max) value = max;
		return value;
	}

	/**
	Calculate X direction vector based on degree angle
	**/
	double Math::DirectionX(double angle)
	{
		//minus 90 degrees for heading vector
		angle -= 90;
		//wrap angle value if less than 0
		if(angle < 0) angle = 360 + angle;
		//unit circle - cos angle for X
		return cos(ToRadians(angle));
	}

	/**
	Calculate Y direction based on degree angle
	**/
	double Math::DirectionY(double angle)
	{
		//minus 90 degrees for heading vector
		angle -= 90;
		//wrap angle value if less than 0
		if (angle < 0) angle = 360 + angle;
		//unit circle - sin angle for Y
		return sin(ToRadians(angle));
	}

	/**
	Calculate vector direction based on degree angle
	**/
	Vector2 Math::Direction(double angle)
	{
		//get X and Y directions from angle
		double vx = DirectionX(angle);
		double vy = DirectionY(angle);
		//send vector for X/Y direction
		return Vector2(vx,vy);
	}

	//compare values and get angle to target (in radians)
	double Math::AngleToTarget(double x1, double y1, double x2, double y2)
	{
		//get difference of X and Y position
		double deltaX = (x2-x1);
		double deltaY = (y2-y1);
		//calculate the arctangent of deltaY/deltaX
		//(This gives the angle from the first position to
		//the second position in radians)
		return atan2(deltaY,deltaX);
	}

	//compare vectors and get angle to target (in radians)
	double Math::AngleToTarget(Vector3& A, Vector3& B)
	{
		//uses previously declared method
		return AngleToTarget(A.getX(), A.getY(), B.getX(), B.getY());
	}

	//compare vectors and get angle to target (in radians)
	double Math::AngleToTarget(Vector2& A, Vector2& B)
	{
		//uses previously declared method
		return AngleToTarget(A.getX(), A.getY(), B.getX(), B.getY());
	}

	//get distance between two points
	double Math::Distance(double x1, double y1, double z1,
							double x2, double y2, double z2)
	{
		//(adaptation of pythagoras)
		//get differences between points
		double deltaX = (x2-x1);
		double deltaY = (y2-y1);
		double deltaZ = (z2-z1);
		//add squares of each delta and then return square root
		return sqrt(deltaX*deltaX+deltaY*deltaY+deltaZ*deltaZ);
	}

	//get distance between two points
	double Math::Distance(double x1, double y1,
							double x2, double y2)
	{
		//(adaptation of pythagoras)
		//get differences between points
		double deltaX = (x2-x1);
		double deltaY = (y2-y1);
		//add squares of each delta and then return square root
		return sqrt(deltaX*deltaX+deltaY*deltaY);
	}

	double Math::Distance(Vector3& A, Vector3& B)
	{
		//(Adaption of pythagoras)
		//get differences between points
		return Distance(A.getX(), A.getY(), A.getZ(),
						B.getX(), B.getY(), B.getZ());
	}

	double Math::Distance(Vector2& A, Vector2& B)
	{
		//use existing method to calculate
		return Distance(A.getX(), A.getY(),
						B.getX(), B.getY());
	}

	//calculate vector length
	double Math::Length(double x, double y, double z)
	{
		//pythagoras to calculate
		return sqrt(x*x+y*y+z*z);
	}

	double Math::Length(double x, double y)
	{
		//pythagoras to calculate
		return sqrt(x*x+y*y);
	}

	double Math::Length(Vector3& V)
	{
		//use existing method to calculate
		return Length(V.getX(), V.getY(), V.getZ());
	}

	double Math::Length(Vector2& V)
	{
		//use existing method to calculate
		return Length(V.getX(), V.getY());
	}

	/*
		//dot product methods (gets scalar value -1 to 1)
		//useful in determining if vectors are facing the same direction
		//can also be used for getting angle between two vectors
		//google "vectors dot product" for more info
	*/
	double Math::DotProduct(double x1, double y1, double z1,
							double x2, double y2, double z2)
	{
		//adds the multiplication of
		//relative dimensions between
		//both vector values
		return(x1*x2+y1*y2+z1*z2);
	}

	double Math::DotProduct(double x1, double y1,
							double x2, double y2)
	{
		//adds the multiplication of
		//relative dimensions between
		//both vector values
		return(x1*x2+y1*y2);
	}

	double Math::DotProduct(Vector3& A, Vector3& B)
	{
		//use existing method to calculate
		return DotProduct(A.getX(), A.getY(), A.getZ(),
						B.getX(), B.getY(), B.getZ());
	}

	double Math::DotProduct(Vector2& A, Vector2& B)
	{
		//use existing method to calculate
		return DotProduct(A.getX(), A.getY(),
						B.getX(), B.getY());
	}

	/*
		//cross product methods - gets perpendicular vector
		//useful for determining right / left direction
		//when given forward vector and up vector
	*/
	Vector3 Math::CrossProduct(double x1, double y1, double z1,
								double x2, double y2, double z2)
	{
		//Cross-Product is a very specific calculation
		//Explanation here:
		//search google for vectors cross product

		double nx = (y1*z2)-(z1*y2);
		double ny = (z1*y2) - (x1*z2);
		double nz = (x1*y2) - (y1*x2);
		return Vector3(nx, ny, nz);
	}

	Vector3 Math::CrossProduct(Vector3& A, Vector3& B)
	{
		//use existing method to calculate
		return CrossProduct(A.getX(), A.getY(), A.getZ(),
						B.getX(), B.getY(), B.getZ());
	}

	/*
		Normalising vectors gives them a magnitude of 1
		(Unit vectors)
		Very handy for determining direction
		//explanation here
		//google vector unit in algebra
	*/
	Vector3 Math::Normal(double x, double y, double z)
	{
		double length = Length(x,y,z);
		if(length!=0) length = 1 / length;
		double nx = x*length;
		double ny = y*length;
		double nz = z*length;
		return Vector3(nx,ny,nz);
	}

	Vector3 Math::Normal(Vector3& V)
	{
		//use existing method to calculate
		return Normal(V.getX(), V.getY(), V.getZ());
	}

};