#pragma once

#include "Engine.h"

namespace Batman
{
	//unique id for each collectable type
	enum collectableType
	{
		COLLECT_SHURIKENS = 10,		//weapon pickup
		COLLECT_DARTS = 20,			//weapon pickup
		COLLECT_BOMBS = 30,			//weapon pickup
		COLLECT_STARS = 40,			//pixie star pickup
	};

	//inherits from mesh
	class Collectable : public Entity
	{
		private:
			Texture* texture;		//texture for the mesh
			Sprite* mapIcon;		//sprite for map icon
			int type;				//type (from collectableType)
			Mesh* visibleModel;		//visible mesh
			Mesh* collectableCollision;	//invisible collision mesh
			float height;
			float xRotation;
			float yDir;
			Timer updateTimer;
		public:
			Collectable(int cType);
			~Collectable();

			//empty methods for entity override
			void Update(float deltaTime);
			void Render() {}

			void Init(int cType);
			int getType() { return type; }
			Texture* getTexture() { return texture; }
			Sprite* getMap() { return mapIcon; }
			void setMap(Sprite* spr) { mapIcon = spr; }
			Mesh* getVisibleMesh() { return visibleModel; }
			Mesh* getInvisibleMesh() { return collectableCollision; }
	};
};