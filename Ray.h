#pragma once

#include "Engine.h"

namespace Batman
{
	class Ray : public Entity
    {
		public:
			D3DXVECTOR3 RayBegin;		//origin
			D3DXVECTOR3 direction;		//direction
			float collisionDistance;	//distance from origin where collided
			Vector3 intersectionPoint;	//point where ray hits
			bool player;				//fired from player?

			Ray();	
			~Ray();

			void Init(Vector3 startFrom, Vector3 target, bool nPlayer);
			void Update(float deltaTime);
			void Render();
	};
};