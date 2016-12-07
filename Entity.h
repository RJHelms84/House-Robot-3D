#pragma once

#include "Engine.h"

namespace Batman 
{

    //entity type used internally to identify type of entity
    enum EntityType
	{
        ENTITY_UNKNOWN = -1,
        ENTITY_MESH = 1, 
        ENTITY_SPRITE,
		ENTITY_BONE_MESH,
		ENTITY_PLAYER_MESH,
		ENTITY_SCENERY_MESH,
		ENTITY_RAY,
		ENTITY_COLLECTABLE_MESH,
		ENTITY_WEAPON_MESH,
		ENTITY_ENEMY_MESH
    };


    //Render type used internally to determine how entity is rendered
    enum RenderType
	{
        RENDER_NONE = 0,
        RENDER_2D,
        RENDER_3D
    };

    class Entity 
    {
    protected:
        int id;							
        std::string name;				
        bool alive;						
        enum RenderType renderType;		
        enum EntityType entityType;		
        float lifetimeStart;			//start of life
        float lifetimeCounter;			//how long lived
        float lifetimeLength;			//how long to live
        bool visible;					//is visible
		bool collidable;				//is collidable
        bool collided;					//has collided
		Entity* collideBuddy;			//pointer to collided entity

	public:
		//constructor/destructor
        Entity(enum RenderType renderType);
        virtual ~Entity() {};

		//virtual. must be overridden
        virtual void Update(float deltaTime) = 0;
        virtual void Render() = 0;

		//get/set id
        void setID(int value) {id = value;}
        int getID() {return id;}

		//get/set rendertype
        void setRenderType(enum RenderType type) { renderType = type; }
        enum RenderType getRenderType() { return renderType; }

		//get/set name
        std::string getName() {return name;}
        void setName(std::string value) {name = value;}

		//get/set visible
        bool getVisible() {return visible;}
        void setVisible(bool value) {visible = value;}

		//get/set alive
        bool getAlive() {return alive;}
        void setAlive(bool value) {alive = value;}

		//get/set lifetime
        float getLifetime() {return lifetimeLength;}
        void setLifetime(float value) {lifetimeLength = value;}

		//get current counter
		float getLifetimeCounter() {return lifetimeCounter;}

		//check if lifetime expired
        bool lifetimeExpired() {return (lifetimeStart + lifetimeCounter > lifetimeLength);}

		//time management for entities
		void addToLifetimeCounter(float value) {lifetimeCounter+=value;}

        EntityType getEntityType() {return entityType;}
        void setEntityType(enum EntityType value) {entityType = value;}

		// ----------------------------- collision support-------------

        //get/set collided flag
		bool isCollided() {return collided;}
        void setCollided(bool value) {collided = value;}

		//get/set collidable flag
        bool isCollidable() {return collidable;}
        void setCollidable(bool value) {collidable = value;}

		//getset collide buddy
		void setCollideBuddy(Entity* entity) {collideBuddy = entity;}
		Entity* getCollideBuddy() {return collideBuddy;}
    };
};