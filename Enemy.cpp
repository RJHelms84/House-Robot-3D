#include "Engine.h"

using namespace Batman;

//constructor
Enemy::Enemy()
{
	visibleModel = new BoneMesh();
	visibleModel->Load("bones_all.x");
	visibleModel->setScale(3,3,3);

	//enemy cube mesh for collision
	enemyCollision = new Mesh();
	enemyCollision->createCube(6,9,6);

	//set enemy state
	enemyType = ENEMY_PATHFOLLOWER_TYPE;

	//set map icon
	mapIcon = new Sprite();
	mapIcon->Load("enemymap.png");

	//initial velocity and forward direction
	forwardVector = Vector3(0,0,-1);
	currentVelocity = 5;
	//set as on path
	enemyState = ENEMY_ON_PATH;
	//combat vars
	enemyLifeState = ENEMY_OK;
	health = 100;
	lives = 3;
	isDead = false;
	PlayerPos = Vector3(0,0,0);
	PlayerDir = Vector3(0,0,0);
	vForward = Vector3(0,0,0);
	vUp = Vector3(0,0,0);
	vRight = Vector3(0,0,0);
	vBehind = Vector3(0,0,0);
	spawn = Vector3(0,0,0);
	JustShot = false;
	weaponFired = 0;

	isFollowing = false;
	isWet = false;
	wetTimer = 0;

	isMovementOn = false;
	canAttack = false;
	chaseStarted = false;
	flankActive = false;
	flankStage = 0;
	randNum=0;
	ambushActive=false;
	waitingInAmbush=false;

	//default starting location
	visibleModel->setPosition(Vector3(200,1,-220));
}

//destructor
Enemy::~Enemy()
{
	if(visibleModel) delete visibleModel;
	if(enemyCollision) delete enemyCollision;
	if(mapIcon) delete mapIcon;
}

void Enemy::Update(float deltaTime)
{
	if(enemyLifeState==ENEMY_OK)
	{
		//MOVEMENT LOGIC
		if(isMovementOn)
		{

		//keep model animation (walking) going
		if(!waitingInAmbush)
			visibleModel->setAnimationSet(1);
		else visibleModel->setAnimationSet(0);

		//ensure current position vector matches that
		//of invisible model (whihc may change due to collision)
		Vector3 currentPos = enemyCollision->getPosition();

		//set goal position to aim for
		checkGoal();

		//calculate direction vector from current position to goal
		//(SEEK) Behaviour)
		forwardVector = Math::Normal(goalPos-currentPos);

		//move from current position in direction of goal
		//calculated with consideration of time delay and velocity
		if(!waitingInAmbush)
		{
			visibleModel->setPosition(currentPos +
				(forwardVector*deltaTime*currentVelocity));
		}

		//if shot timer passes a second
		if(ShootTimer.StopWatch(1000))
		{
			//if enemy within 10 units range on x / z
			if(PlayerPos.z < enemyCollision->getPosition().z+10 &&
				PlayerPos.z > enemyCollision->getPosition().z-10)
			{
				if(PlayerPos.x < enemyCollision->getPosition().x+10 &&
					PlayerPos.x > enemyCollision->getPosition().x-10)
				{
					//set state to shoot and just shot flag
					if(canAttack)
					{
						enemyLifeState=ENEMY_ATTACK;
						JustShot=true;
					}
				}
			}

			//if player within 50 units range, start following
			if(!isWet)
			{
				if(ambushActive==false)
				{
					if(PlayerPos.z < enemyCollision->getPosition().z+50 &&
						PlayerPos.z > enemyCollision->getPosition().z-50)
					{
						if(PlayerPos.x < enemyCollision->getPosition().x+50 &&
							PlayerPos.x > enemyCollision->getPosition().x-50)
						{
							if(canAttack)
							{
								if(isFollowing==false)
								{
									isFollowing = true; 
									chaseStarted = true;
									currentVelocity = 25;
									if(randNum>=5)
									{
										ambushActive=true;
									}
									else
									{
										ambushActive=false;
									}
								}
							}
						}
					}
				}
				else //if AMBUSH behaviour activated, attack at a shorter distance
				{
					if(PlayerPos.z < enemyCollision->getPosition().z+40 &&
						PlayerPos.z > enemyCollision->getPosition().z-40)
					{
						if(PlayerPos.x < enemyCollision->getPosition().x+40 &&
							PlayerPos.x > enemyCollision->getPosition().x-40)
						{
							if(canAttack)
							{
								if(isFollowing==false)
								{
									isFollowing = true; 
									chaseStarted = true;
									currentVelocity = 25;
									waitingInAmbush = false;
									if(randNum>=5)
									{
										ambushActive=true;
									}
									else
									{
										ambushActive=false;
									}
								}
							}
						}
					}
				}
			}

			//if player beyond 50 units range, stop following
			if(PlayerPos.z > enemyCollision->getPosition().z+50 ||
				PlayerPos.z < enemyCollision->getPosition().z-50)
			{
				//set state to shoot and just shot flag
				isFollowing = false;
				flankStage = 0;

				if(randNum>=5)
				{
					flankActive=true;
				}
				else
				{
					flankActive=false;
				}

				currentVelocity = 5;
			}
			if(PlayerPos.x > enemyCollision->getPosition().x+50 ||
				PlayerPos.x < enemyCollision->getPosition().x-50)
			{
				//set state to shoot and just shot flag
				isFollowing = false;
				currentVelocity = 5;
			}

			//wet timer
			if(isWet)
			{
				currentVelocity = 25;
				if(wetTimer >= 5) //5 seconds wet
				{
					isWet=false;
					currentVelocity = 5;
					wetTimer=0;
				}
				else
				{
					wetTimer++;
				}
			}
		}//end stopwatch
		}//end isMovementOn check
	}
	else if (enemyLifeState==ENEMY_HURT)
	{
		//update animation
		visibleModel->setAnimationSet(2);

		//after 200ms update state
		if(damageTimer.StopWatch(200))
			enemyLifeState=ENEMY_OK;
	}
	else if(enemyLifeState==ENEMY_DYING)
	{
		//update animation
		visibleModel->setAnimationSet(4);

		//after 300ms update state
		if(damageTimer.StopWatch(10000))
		{
			if(!isDead)
			{
				enemyLifeState=ENEMY_OK;
				health = 100;
				damageTimer.Reset();
				isWet=false;
			}
		}
	}
	else if(enemyLifeState==ENEMY_ATTACK)
	{
		Vector3 currentPos = enemyCollision->getPosition();
		//calculate direction to player
		forwardVector = Math::Normal(PlayerPos-currentPos);

		//update animation
		visibleModel->setAnimationSet(3);

		//if we've just shot on last update, reset flag
		if(JustShot)
		{
			JustShot=false;
			//use this space for shooting sound fx
		}
		//if 400 ms has passed, change state
		if(ShootTimer.StopWatch(400))
		{
			enemyLifeState=ENEMY_OK;
		}
	}

	//rotation calculations
	//vector for x axis direction
	Vector3 vecX(1,0,0);
	//get dot product of forward vector and x axis
	float scalar = Math::DotProduct(forwardVector,vecX);
	//use cos^-1 to get angle for rotation
	float rotationValue = acos(scalar);

	//check which direction on Z character heading
	if(forwardVector.z>0)
	{
		//if going positive on Z then
		//recalculate rotation value
		float toAdd = PI-rotationValue;
		rotationValue=PI+toAdd;
	}

	//convert to degrees
	rotationValue = D3DXToDegree(rotationValue);

	//update model's rotation
	visibleModel->setRotation(rotationValue-90,0,0);
	//END: Rotation

	//Update, transform and animate visible model
	visibleModel->Update(deltaTime);
	visibleModel->Transform();
	visibleModel->Animate(deltaTime,&visibleModel->getMatrix());

	//update position of invisible collision mesh
	enemyCollision->setPosition(visibleModel->getPosition());
	enemyCollision->Update(deltaTime);
}

void Enemy::checkGoal()
{
	if(isFollowing == false)
	{
		if(ambushActive==false) //WANDER behaviour (follow waypoints)
		{
			//set goal position as current waypoint
			goalPos = waypoints[currentWaypoint];

			//AABB check on waypoint
			if(!(enemyCollision->getPosition().x + 1 < goalPos.x - 1 ||
				enemyCollision->getPosition().x - 1 > goalPos.x + 1 ||
				enemyCollision->getPosition().z + 1 < goalPos.z - 1 ||
				enemyCollision->getPosition().z - 1 > goalPos.z + 1))
			{
				//if in range of goal point, set to the next waypoint
				currentWaypoint++;
				//if above number of waypoints, wrap to first
				if(currentWaypoint==numWaypoints)
					currentWaypoint=0;
				//update goal position to the correct waypoint
				goalPos = waypoints[currentWaypoint];
			}
		}
		else //AMBUSH behaviour
		{
			goalPos = Vector3(172,1,-190);

			if(!waitingInAmbush)
			{
				if(!(enemyCollision->getPosition().x + 1 < goalPos.x - 1 ||
					enemyCollision->getPosition().x - 1 > goalPos.x + 1 ||
					enemyCollision->getPosition().z + 1 < goalPos.z - 1 ||
					enemyCollision->getPosition().z - 1 > goalPos.z + 1))
				{
					waitingInAmbush = true;
				}
			}
		}
	}
	else
	{
		if(flankActive) //FLANK behaviour
		{
			if(flankStage==0)
			{
				//determine forward vector from camera target direction
				//set y to 0 and get x/z vector and normalize
				vForward = camT;
				vForward.y = 0;
				vForward = Math::Normal(vForward);
				vBehind = vForward*-1;

				//set up as positive on the y
				vUp = Vector3(0,1,0);

				//calculate direction to the right of where the camera is facing using cross product
				vRight = Math::CrossProduct(vUp, vForward);

				goalPos = (PlayerPos + (vRight * 30));
				goalPos.y = 1;

				//check if arrived
				if(!(enemyCollision->getPosition().x + 1 < goalPos.x - 1 ||
					enemyCollision->getPosition().x - 1 > goalPos.x + 1 ||
					enemyCollision->getPosition().z + 1 < goalPos.z - 1 ||
					enemyCollision->getPosition().z - 1 > goalPos.z + 1))
				{
					flankStage++;
				}
			}
			else if(flankStage==1)
			{
				//determine forward vector from camera target direction
				//set y to 0 and get x/z vector and normalize
				vForward = camT;
				vForward.y = 0;
				vForward = Math::Normal(vForward);
				vBehind = vForward*-1;

				//set up as positive on the y
				vUp = Vector3(0,1,0);

				//calculate direction to the right of where the camera is facing using cross product
				vRight = Math::CrossProduct(vUp, vForward);

				goalPos = (PlayerPos + (vBehind * 30));
				goalPos.y = 1;

				//check if arrived
				if(!(enemyCollision->getPosition().x + 1 < goalPos.x - 1 ||
					enemyCollision->getPosition().x - 1 > goalPos.x + 1 ||
					enemyCollision->getPosition().z + 1 < goalPos.z - 1 ||
					enemyCollision->getPosition().z - 1 > goalPos.z + 1))
				{
					flankStage++;
				}
			}
			else if(flankStage==2)
			{
				//determine forward vector from camera target direction
				//set y to 0 and get x/z vector and normalize
				vForward = camT;
				vForward.y = 0;
				vForward = Math::Normal(vForward);
				vBehind = vForward*-1;

				//set up as positive on the y
				vUp = Vector3(0,1,0);

				//calculate direction to the right of where the camera is facing using cross product
				vRight = Math::CrossProduct(vUp, vForward);

				goalPos = (PlayerPos + (vBehind * 10));
				goalPos.y = 1;
			}
		}
		else //CHASE behaviour
		{
			//determine forward vector from camera target direction
			//set y to 0 and get x/z vector and normalize
			vForward = camT;
			vForward.y = 0;
			vForward = Math::Normal(vForward);
			vBehind = vForward*-1;

			//set up as positive on the y
			vUp = Vector3(0,1,0);

			//calculate direction to the right of where the camera is facing using cross product
			vRight = Math::CrossProduct(vUp, vForward);

			goalPos = (PlayerPos + (vForward * 10));
			goalPos.y = 1;
		}
	}
}

//SHOT TIME
void Enemy::TakeDamage(int value)
{
	//subtract damage from health
	//health-=value;
	if(enemyLifeState!=ENEMY_DYING)
	{
		if(value == 20) //if shuriken/water
		{
			isWet = true;
			//Begin FLEE behaviour
			isFollowing = false;
			currentVelocity = 25;
			ambushActive=false;
			waitingInAmbush=false;

		}
		else if(value == 30)//if dart/hair dryer
		{
			if(isWet == true)
				health=0;
		}
	
		//set life state
		enemyLifeState=ENEMY_HURT;

		if(health<=0)
		{
			enemyLifeState=ENEMY_DYING;
			lives--;
			if(lives<=0)
				isDead=true;
		}

		//reset damage timer
		damageTimer.Reset();
	}
}

void Enemy::SetCamTarg(Vector3 camTarg)
{
	camT = camTarg;
}