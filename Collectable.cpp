#include "Engine.h"

using namespace Batman;

Collectable::Collectable(int cType) : Entity(RENDER_NONE)
{
	//call init method and send type
	Init(cType);
}

Collectable::~Collectable()
{
	//release memory
	if(mapIcon) delete mapIcon;
	if(texture) delete texture;
}

void Collectable::Init(int cType)
{
	//set type
	type = cType;

	//collision cube
	collectableCollision = new Mesh();
	collectableCollision->createCube(2,2,2);
	visibleModel = new Mesh();
	visibleModel->createCube(2,2,2);
	visibleModel->setScale(0.5);

	height = 2;
	xRotation = 0;
	yDir = 0.025;

	texture = NULL;

	//This construct checks the collectable type
	//depending on the type, the corresponding
	//model and map icon sprite are loaded
	switch(type)
	{
	case COLLECT_SHURIKENS:
		{
			mapIcon = new Sprite();
			mapIcon->Load("HUD/Map/shurikenMapSprite.png");
		}
		break;
	case COLLECT_DARTS:
		{
			mapIcon = new Sprite();
			mapIcon->Load("HUD/Map/dartMapSprite.png");
		}
		break;
	case COLLECT_BOMBS:
		{
			mapIcon = new Sprite();
			mapIcon->Load("HUD/Map/bombMapSprite.png");
		}
		break;
	case COLLECT_STARS:
		{
			mapIcon = new Sprite();
			mapIcon->Load("HUD/Map/starMapSprite.png");
		}
		break;
	}
}

void Collectable::Update(float deltaTime)
{
	//every 100 milliseconds
	if(updateTimer.StopWatch(100))
	{
		//update height with Y direction
		height+=yDir;
		//if above 3, change direction
		if(height>3)
		{
			height=3;
			yDir*=-1;
		}
		//if below 1, change direction
		if(height<1)
		{
			height=1;
			yDir*=-1;
		}
		//update rotation value on x axis
		//and bound within 360
		xRotation+=0.0025;
		if(xRotation==360)
			xRotation=0;
	}

	//change height of model
	visibleModel->setPosition(visibleModel->getPosition().x,
							height,
							visibleModel->getPosition().z);
	//rotate model then update matrices
	visibleModel->Rotate(xRotation,0,0);
	visibleModel->Transform();
}