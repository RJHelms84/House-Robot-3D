#pragma once

#include "Engine.h"

namespace Batman
{
	class EAngle {
		public:
			float p, y, r; //pitch / yaw / roll

			EAngle() {
				p = y = r = 0;
			}

			EAngle(float pitch,  float yaw, float roll) {
				p = pitch;
				y = yaw;
				r = roll;
			}

			Vector3 ToVector() const;

			void Normalize();
	};

}