#pragma once

#include "Engine.h"

namespace Batman
{
	//Constraints for easy use (trig, angles etc)
	const double PI = 3.1415926535;
	const double PI_over_180 = PI / 180.0f;
	const double PI_under_180 = 180.0f / PI;

	class Math
	{
	public:
		//angle units conversion
		static double ToDegrees(double radian);
		static double ToRadians(double degree);

		//wrap angle values
		static double WrapAngleDegs(double degs);
		static double WrapAngleRads(double rads);

		//wrap, limit values according to range given
		static double WrapValue(double value, double min, double max);
		static double Limit(double value, double min, double max);

		//calculate direction vectors based on angle (degrees)
		static double DirectionX(double angle);
		static double DirectionY(double angle);
		static Vector2 Direction(double angle);

		//compare vectors and get angle to target (in radians)
		static double AngleToTarget(double x1, double y1, double x2, double y2);
		static double AngleToTarget(Vector3& source, Vector3& target);
		static double AngleToTarget(Vector2& source, Vector2& target);

		//get distance between two points
		static double Distance(double x1, double y1, double z1, double x2, double y2, double z2);
		static double Distance(double x1, double y1, double x2, double y2);
		static double Distance(Vector3& A, Vector3& B);
		static double Distance(Vector2& A, Vector2& B);

		//get vector length (magnitude)
		static double Length(double x, double y, double z);
		static double Length(double x, double y);
		static double Length(Vector3& V);
		static double Length(Vector2& V);

		//dot product methods (gets scalar value -1 to 1)
		//useful in determining if vectors are facing the same direction
		//can also be used for getting angle between two vectors
		static double DotProduct(double x1, double y1, double z1, double x2, double y2, double z2);
		static double DotProduct(double x1, double y1, double x2, double y2);
		static double DotProduct(Vector3& A, Vector3& B);
		static double DotProduct(Vector2& A, Vector2& B);

		//cross product methods - gets perpendicular vector
		//useful for determining right / left direction
		//when given forward vector and up vector
		static Vector3 CrossProduct(double x1, double y1, double z1, double x2, double y2, double z2);
		static Vector3 CrossProduct(Vector3& A, Vector3& B);

		//normalise vector methods
		static Vector3 Normal(double x, double y, double z);
		static Vector3 Normal(Vector3& V);
	};
};