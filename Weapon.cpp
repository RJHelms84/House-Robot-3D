#include "Engine.h"

using namespace Batman;

//constructor
Weapon::Weapon()
{
	visibleModel = NULL;
	weaponCollision = NULL;
	weaponState = 0;
	weaponType = 0;
	forwardVector = Vector3(0,0,0);
	goalPos = Vector3(0,0,0);
	currentVelocity = 0;
	rotationX = 0;
	spawnPoint = Vector3(0,0,0);
	effectiveDistance = Vector3(0,0,0);
	hVel = 0;
	yVel = 0;
	timeElapsed = 0;
	//initialisation in Init method
}

void Weapon::Init(Vector3 direction, int type, Vector3 spawn)
{
	//initial states
	weaponType = type;
	forwardVector = direction;
	weaponState = WEAPON_INAIR_STATE;
	spawnPoint = spawn;

	//This construct checks the weapon type
	//depending on the type, the corresponding
	//model and collision meshes are loaded
	switch(weaponType)
	{
		case WEAPON_SHURIKEN:
			{
				//effective distance 50 units from spawn point
				effectiveDistance = spawnPoint + (direction*50);
				//small flat(ish) cube
				visibleModel = new Mesh();
				visibleModel->createCube(0.25,0.1,0.25);

				//angle calculations (so shuriken 'flat' thrown from player)
				//vector for Y axis direction
				Vector3 vecY(0,1,0);
				//get dot product of forward vector and Y axis
				float scalar2 = Math::DotProduct(forwardVector,vecY);
				//use cos^-1 to get angle for rotation
				float Yangle = acos(scalar2);

				//convert to degrees to readjust angle accurately
				Yangle = D3DXToDegree(Yangle);
				Yangle+=90;
				Yangle*=-1;
				//END: Angle
				//set rotation
				visibleModel->setRotation(0,Yangle,0);
				//spawn in front of character
				visibleModel->setPosition(spawn+(forwardVector*2));
				//update matrices
				visibleModel->Transform();
				//create collision cube with same properties
				weaponCollision = new Mesh();
				weaponCollision->createCube(0.25,0.1,0.25);
				weaponCollision->setPosition(spawn+(forwardVector*2));
				weaponCollision->Transform();
				//set velocity at 30 (units per second)
				currentVelocity = 30;
			}
			break;

		case WEAPON_DART:
			{
				//effective distance 100 units from spawn point
				effectiveDistance = spawnPoint + (direction*100);

				visibleModel = new Mesh();
				visibleModel->createCube(0.5,0.1,0.1);
				visibleModel->setPosition(spawn+(forwardVector*2));

				//rotation calculations (point dart away from player
				//vector for x axis direction
				Vector3 vecX(1,0,0);
				//dont consider Y value for direction
				Vector3 hDirection = forwardVector;
				hDirection.y = 0;
				hDirection=Math::Normal(hDirection);
				//get dot product of forward vector and x axis
				float scalar = Math::DotProduct(hDirection,vecX);
				//use cos^-1 to get angle for rotation
				float rotationValue = acos(scalar);

				//check which direction on z dart rotation
				if(forwardVector.z>0)
				{
					//if going positive on z axis
					//recalculate rotation value
					float toAdd = PI-rotationValue;
					rotationValue=PI+toAdd;
				}
				//convert to degrees
				rotationValue = D3DXToDegree(rotationValue);

				//angle calculations
				//vector for y axis direction
				Vector3 vecY(0,1,0);
				//get dot product of forward vector and y axis
				float scalar2  = Math::DotProduct(forwardVector,vecY);
				//use cos^-1 to get angle for rotation
				float Zangle = acos(scalar2);

				//convert to degrees to readjust angle accurately
				Zangle = D3DXToDegree(Zangle);
				Zangle+=90;
				Zangle*=-1;
				//END: Angle

				//update model's rotation
				visibleModel->setRotation(rotationValue, 0, Zangle);
				visibleModel->Transform();

				//create collision cube with same properties
				weaponCollision = new Mesh();
				weaponCollision->createCube(0.5,0.1,0.1);
				weaponCollision->setPosition(spawn+(forwardVector*2));
				weaponCollision->Transform();
				//set velocity at 50 (units per second)
				currentVelocity = 50;

			}
			break;
		case WEAPON_BOMB:
			{
				//create small cube in front of player
				visibleModel = new Mesh();
				visibleModel->createCube(0.4,0.4,0.4);
				visibleModel->setPosition(spawn+(forwardVector*2));
				visibleModel->Transform();
				//create matching collision cube
				weaponCollision = new Mesh();
				weaponCollision->createCube(0.4,0.4,0.4);
				weaponCollision->setPosition(spawn+(forwardVector*2));
				weaponCollision->Transform();

				//angle calculations (get angle thrown at for SUVAT)
				//vector for y axis direction
				Vector3 vecY(0,1,0);
				//get dot product of forward vector and x axis
				float scalar = Math::DotProduct(forwardVector,vecY);
				//use cos^-1 to get angle for rotation
				float angle = acos(scalar);

				//convert to degrees to readjust angle accurately
				angle = D3DXToDegree(angle);
				angle-=90;
				angle*=-1;
				//END: Angle

				//get rad value of angle
				angle = Math::ToRadians(angle);

				//calculate y and x velocities
				//(modifiers added for each dimension)
				currentVelocity = 5;
				yVel = sin(angle) * currentVelocity;
				hVel = cos(angle) * currentVelocity;
			}
			break;
	}
}

void Weapon::Update(float deltaTime)
{
	//update x rotation, bounded by 360
	rotationX += 0.5;
	if(rotationX==360)
	{
		rotationX = 0;
	}
	//vector for current position (less verbose code later)
	Vector3 currentPos = visibleModel->getPosition();
	//float for current velocity (less verbose code later)
	float vel = currentVelocity;

	//This construct checks the weapon type
	//depending on the type, the corresponding
	//model and collision meshes are updated
	switch(weaponType)
	{
	case WEAPON_SHURIKEN:
		{
			//rotate shuriken on x direction
			visibleModel->Rotate(rotationX,0,0);

			//if in air, move forwards based on forward direction, time and velocity
			if(weaponState == WEAPON_INAIR_STATE)
			{
				visibleModel->setPosition(currentPos.x + forwardVector.x * deltaTime * vel,
										currentPos.y + forwardVector.y * deltaTime * vel,
										currentPos.z + forwardVector.z * deltaTime * vel);
				//use length to compare if distance shuriken is from the point fired
				//is greater than the effective distance; if so, change state
				if(Math::Length(spawnPoint - currentPos) >
					Math::Length(spawnPoint - effectiveDistance))
				{
					weaponState = WEAPON_FALL_STATE;
				}
			}
			//if in fall state, still move forwards on x and z as before,
			//but fall downwards rapidly (look at Y position setting)
			if(weaponState == WEAPON_FALL_STATE)
			{
				visibleModel->setPosition(currentPos.x + forwardVector.x * deltaTime * vel,
										currentPos.y - deltaTime * vel * 2,
										currentPos.z + forwardVector.z * deltaTime * vel);
				//if it hits the floor, change state
				if(currentPos.y<0)
				{
					weaponState = WEAPON_HIT_STATE;
				}
			}
		}
		break;
	case WEAPON_DART:
		{
			//if in air, move forwards based on forward direction, time and velocity
			if(weaponState == WEAPON_INAIR_STATE)
			{
				visibleModel->setPosition(currentPos.x + forwardVector.x * deltaTime * vel,
										currentPos.y + forwardVector.y * deltaTime * vel,
										currentPos.z + forwardVector.z * deltaTime * vel);
				//use length to compare if distance dart is from the point fired
				//is greater than the effective distance; if so, change state
				if(Math::Length(spawnPoint - visibleModel->getPosition()) >
					Math::Length(spawnPoint - effectiveDistance))
				{
					weaponState = WEAPON_FALL_STATE;
				}
			}
			//if in fall state, still move forwards on x and z as before,
			//but fall downwards rapidly (look at Y position setting)
			if(weaponState == WEAPON_FALL_STATE)
			{
				visibleModel->setPosition(currentPos.x + forwardVector.x * deltaTime * vel,
										currentPos.y - deltaTime * vel * 2,
										currentPos.z + forwardVector.z * deltaTime * vel);
				//if it hits the floor, change state
				if(visibleModel->getPosition().y<0)
				{
					weaponState = WEAPON_HIT_STATE;
				}
			}
		}
		break;
	case WEAPON_BOMB:
		{
			if(weaponState == WEAPON_INAIR_STATE)
			{
				//aggregate time elapsed since firing (for SUVAT calculation)
				timeElapsed+=deltaTime;

				//gravity setting (adjust for funky effects)
				float gravity = -9.8;
				//calculate 3D movement using SUVAT equations
				float x = hVel*timeElapsed*forwardVector.x;
				float y = ((yVel*timeElapsed)+0.5*(gravity*(timeElapsed*timeElapsed)));
				float z = hVel*timeElapsed*forwardVector.z;

				visibleModel->setPosition(currentPos.x + x,
										currentPos.y + y,
										currentPos.z + z);

				//if it hits the floor, change state
				if(currentPos.y<0)
				{
					weaponState = WEAPON_HIT_STATE;
				}
			}
		}
		break;
	}

	//set collision position then update both model's matrices
	weaponCollision->setPosition(currentPos);
	visibleModel->Update(deltaTime);
	weaponCollision->Update(deltaTime);
}