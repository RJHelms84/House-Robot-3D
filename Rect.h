#pragma once

#include "Engine.h"

namespace Batman
{
    class Rect
    {
		public:
			//side measurements
			double left;
			double top;
			double right;
			double bottom;

			//Constructor / copy constructors
			Rect();
			Rect(const Rect& rect);
			Rect(const RECT& rect);
			Rect(int l, int t, int r, int b);
			Rect(double l, double t, double r, double b);
			virtual ~Rect() {}

			//operator overloads (=)
			Rect& operator=(const Rect& R);
			Rect& operator=(const RECT& R);

			//set Rect values
			void Set(const Rect& R);
			void Set(const RECT& R);
			void Set(int l, int t, int r, int b);
			void Set(double l, double t, double r, double b);
			
			//getters
			double getLeft() { return left; }
			double getTop() { return top; }
			double getRight() { return right; }
			double getBottom() { return bottom; }
			double getWidth() { return right-left;}
			double getHeight() { return bottom - top;}
			
			//test for intersections
			//(sprite collisions)
			bool Contains(Vector3 point);
			bool Contains(int x, int y);
			bool Contains(double x, double y);
			bool Intersects(Rect rect);

			//comparison operator overloads
			bool operator==(const Rect& R) const;
			bool operator!=(const Rect& R) const;
    };

}; //namespace

