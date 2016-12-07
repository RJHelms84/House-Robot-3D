#include "Engine.h"
using namespace std;

namespace Batman
{
	Ray::Ray():Entity(RENDER_3D)
    {
		entityType = ENTITY_RAY;
    }

	void Ray::Init(Vector3 startFrom, Vector3 dir, bool nPlayer)
	{
		
		//set properties for beginning and direction of ray
		RayBegin= startFrom.ToD3DXVECTOR3();
		direction=Math::Normal(dir).ToD3DXVECTOR3();

		//collision detection properties / where the ray hits
		collisionDistance=0;
		intersectionPoint = Vector3(0,0,0);

		//set player flag
		player=nPlayer;
	}

	Ray::~Ray()
	{}

	void Ray::Update(float deltaTime)
	{	
		//not really much to update!
	}

	void Ray::Render()
	{	
		//this shouldn't be called
	}
};