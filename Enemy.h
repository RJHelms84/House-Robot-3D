#pragma once
#include "Engine.h"

namespace Batman
{
	//types of enemy
	enum enemyTypes
	{
		ENEMY_WANDERER_TYPE,
		ENEMY_PATHFOLLOWER_TYPE
	};
	//main states (steering)
	enum enemyStates
	{
		ENEMY_ON_PATH,
		ENEMY_RETURN_TO_PATH
	};
	//life states (combat)
	enum enemyLifeStates
	{
		ENEMY_ATTACK,
		ENEMY_HURT,
		ENEMY_DYING,
		ENEMY_DEAD,
		ENEMY_OK
	};

	class Enemy
	{
	private:
		BoneMesh* visibleModel;	//visible animated mesh
		Mesh* enemyCollision;
		int enemyState;
		int enemyType;
		std::vector<Vector3> waypoints;
		Sprite* mapIcon;	//icon for minimap display
		Vector3 forwardVector;
		Vector3 goalPos;
		float currentVelocity;
		int currentWaypoint;
		int numWaypoints;
		int enemyLifeState;
		int health;
		int lives;
		bool isDead;
		Timer damageTimer;
		Vector3 PlayerPos;
		Vector3 PlayerDir;
		bool JustShot;
		Timer ShootTimer;
		int weaponFired;
		bool isWet;
		int wetTimer;

		//follow behaviour
		bool isFollowing;

		bool isMovementOn;
		bool canAttack;
		bool chaseStarted;
		bool flankActive;
		int flankStage;
		int randNum;
		bool ambushActive;
		bool waitingInAmbush;

		Vector3 vForward;
		Vector3 vUp;
		Vector3 vRight;
		Vector3 vBehind;
		Vector3 spawn;
		Vector3 camT;

	public:
		Enemy();
		void Init();
		~Enemy();
		void Update(float deltaTime);

		void checkGoal();

		//waypoints
		void setWaypoints(std::vector<Vector3> value)
		{
			waypoints = value;
			currentWaypoint = 1;
			numWaypoints = value.size();	//get number of waypoints
		}

		//sprite for map rendering
		Sprite* getMap() { return mapIcon; }

		//getters / setters
		//meshes needed for collision and rendering
		BoneMesh* getVisibleMesh() { return visibleModel; }
		Mesh* getInvisibleMesh() { return enemyCollision; }

		int getState() { return enemyState; }
		void setState(int value) { enemyState=value; }

		int getLifeState() { return enemyLifeState; }
		void setLifeState(int value) { enemyLifeState=value; }

		int getHealth() { return health; }
		void setHealth(int value) { health=value; }

		bool getJustShot() { return JustShot; }
		void setJustShot(bool value) { JustShot=value; }

		Vector3 getPlayerPos() { return PlayerPos; }
		void setPlayerPos(Vector3 value) { PlayerPos=value; }

		Vector3 getPlayerDir() { return PlayerDir; }
		void setPlayerDir(Vector3 value) { PlayerDir=value; }

		void setWeaponFired(int value) { weaponFired=value; }
		//manage damage taking
		void TakeDamage(int value);

		bool GetCanAttack() {return canAttack;}
		bool GetMovementOn() {return isMovementOn;}

		void SetCanAttack(bool value) {canAttack = value;}
		void SetMovementOn(bool value) {isMovementOn = value;}

		bool GetIsFollowing() {return isFollowing;}

		bool GetChaseStarted() {return chaseStarted;}
		void SetChaseStarted(bool value) { chaseStarted = value; }

		bool GetIsDead() {return isDead;}

		void SetCamTarg(Vector3 camTarg);

		void SetRand(int val) { randNum = val; }
	};
};