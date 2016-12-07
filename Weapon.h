#pragma once
#include "Engine.h"

namespace Batman
{
	//type of weapon
	enum weaponTypes
	{
		WEAPON_SHURIKEN,
		WEAPON_DART,
		WEAPON_BOMB
	};
	//weapon state
	enum weaponStates
	{
		WEAPON_INAIR_STATE,
		WEAPON_FALL_STATE,
		WEAPON_HIT_STATE
	};

	class Weapon
	{
		private:
			Mesh* visibleModel;
			Mesh* weaponCollision;
			int weaponState;
			int weaponType;
			Vector3 forwardVector;
			Vector3 goalPos;
			float currentVelocity;
			float rotationX;
			Vector3 spawnPoint;
			Vector3 effectiveDistance;
			float hVel;
			float yVel;
			float timeElapsed;
		public:
			Weapon();
			void Init(Vector3 direction, int type, Vector3 spawn);
			~Weapon();

			void Update(float deltaTime);

			//getters and setters
			//meshes needed for collision and rendering
			Mesh* getVisibleMesh()
				{ return visibleModel; }
			Mesh* getInvisibleMesh()
				{ return weaponCollision; }

			//state machine
			int getState()
				{ return weaponState; }
			void setState(int value)
				{ weaponState=value; }

			//get type
			int getWeaponType()
			{
				return weaponType;
			}
	};
};