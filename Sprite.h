#pragma once

#include "Engine.h"

namespace Batman 
{
	//Inherit from Entity
    class Sprite : public Entity
    {
    protected:
        Texture *image;				//sprite img
        bool imageLoaded;			//img loaded?
        D3DXMATRIX matTransforms;	//transform matrix

    public:
		Vector2 position;			//position
		Vector2 velocity;			//velocity
		Vector2 size;				//size
		int state;					//state value
		int animationColumns;		//anim cols
		float currentFrame;			//anim frame
		int firstFrame;				//first frame
        int lastFrame;				//last frame
		int animationDirection;		//anim dir
		double rotation;			//rotation
		Vector2 scale;				//scale
		Color color;				//render color
		Vector2 pivotPoint;			//origin
		//use to adjust location of source rectangle
        Vector2 sourcePosition;

		//constructor / destructor
        Sprite();
        virtual ~Sprite();

		//load / set image
        bool Load(std::string filename, 
			Color transcolor = Color(255,0,255,0));
        void setImage(Texture *);

        //managed functions
        void Update(float deltaTime);
        void Render();

        void RenderFast(); //fast draw with no animation
        void Render(bool autoPivot); //draw with animation

        //center-pivot property
        Vector2 getPivot() { return pivotPoint; }
        void setPivot(Vector2 pivot) { pivotPoint = pivot; }

        //position on screen
        Vector2 getPosition() { return position; }
        void setPosition(Vector2 value) { position = value; }
        void setPosition(double x, double y) { position.Set(x,y); }
        double getX() { return position.x; }
        double getY() { return position.y; }
        void setX(double x) { position.x=x; }
        void setY(double y) { position.y=y; }

        //movement velocity
        Vector2 getVelocity() { return velocity; }
        void setVelocity(Vector2 value) { velocity = value; }
        void setVelocity(double x, double y) { velocity.x=x; velocity.y=y; }
    
        //image dimensions
        void setSize(Vector2 dim) { size = dim; }
        void setSize(int width, int height) { size.x=width; size.y=height; }
        int getWidth() { return (int)size.x; }
        int getHeight() { return (int)size.y; }

        //multi-use sprite state
        int getState() { return state; }
        void setState(int value) { state = value; }

        //animation columns
        int getColumns() { return animationColumns; }
        void setColumns(int value) { animationColumns = value; }

        //current animation frame
        int getCurrentFrame() { return (int)currentFrame; }
        void setCurrentFrame(int value) { currentFrame = (float)value; }

        //animation range
        int getFirstFrame() { return firstFrame; }
        int getLastFrame() { return lastFrame; }
        void setAnimationRange(int first,int last) { firstFrame=first; 
													lastFrame=last; }

        //animation direction property
        int getAnimationDirection() { return animationDirection; }
        void setAnimationDirection(int value) { animationDirection = value; }

        //rotation transform
        double getRotation() { return rotation; }
        void setRotation(double value) { rotation = value; }

        //scale transform
        double getScaleHoriz() { return scale.x; }
        double getScaleVert() { return scale.y; }
        void setScale(double horiz,double vert) { scale.x = horiz; 
												  scale.y = vert; }
        void setScale(double scale) { setScale(scale,scale); }

        //rendering color
        Color getColor() { return color; }
        void setColor(Color col) { color = col; }

        //returns boundary of sprite
        Rect getBounds();

        //facing and moving angles are helpful for targeting
        double facingAngle;
        double getFacingAngle() { return facingAngle; }
        void setFacingAngle(double angle) { facingAngle = angle; }
        double movingAngle;
        double getMovingAngle() { return movingAngle; }
        void setMovingAngle(double angle) { movingAngle = angle; }

    }; //class
};