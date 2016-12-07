#include "Engine.h"
using namespace std;

namespace Batman {

	//default contructor; all vals 0
	Rect::Rect() {
		left = top = right = bottom = 0;
	}

	//copy constructor
	Rect::Rect(const Rect& R) {
		Set(R);
	}

	//copy constructor
	//windows RECT
	Rect::Rect(const RECT& R) {
		Set(R);
	}

	//constructor - set int values 
	Rect::Rect(int l, int t, int r, int b) {
		Set(l,r,t,b);
	}

	//constructor - set double values 
	Rect::Rect(double l, double t, double r, double b) {
		Set(l,t,r,b);
	}

	//assignment operator
	Rect& Rect::operator=(const Rect& R) {
		Set(R); return *this;
	}
	
	//assignment operator - windows RECT
	Rect& Rect::operator=(const RECT& R) {
		Set(R); return *this;
	}

	//Set methods;
	//using another Rect
	void Rect::Set(const Rect& R) {
		left=R.left; top = R.top;
		right=R.right; bottom=R.bottom;
	}

	//using windows RECT
	void Rect::Set(const RECT& R) {
		left=R.left; top = R.top;
		right=R.right; bottom=R.bottom;
	}

	//using ints
	void Rect::Set(int l, int t, int r, int b) {
		left=(double)l; top = (double)t;
		right=(double)r; bottom=(double)b;
	}

	//using doubles
	void Rect::Set(double l, double t, double r, double b) {
		left=l; top = t;
		right=r; bottom=b;
	}

	//send Vector3 point to main Contains
	bool Rect::Contains(Vector3 point) {
		return Contains(point.getX(), point.getY());
	}

	//send ints to main Contains
	bool Rect::Contains(int x, int y) {
		return Contains((double)x,(double)y);
	}

	//checks to see if an x / y 
	bool Rect::Contains(double x, double y) {
		return (x > left && x < right && y > top && y < bottom);
	}

	//intersection between 2 rectangles
	bool Rect::Intersects(Rect rect) {
		//AABB Intersection
		//Check min / max of rectangles
		if(	right < rect.left ||
			rect.right < left ||
			bottom < rect.top ||
			rect.bottom < top
		  )
			return false;

		return true;
	}

	//equality operator comparison includes double rounding
	bool Rect::operator==(const Rect& R) const {
		return (left==R.left && top == R.top &&
				right ==- R.right && bottom == R.bottom);
	}
	//inequality operator
	bool Rect::operator!=(const Rect& V) const {
		return (!(*this==V));
	}
}