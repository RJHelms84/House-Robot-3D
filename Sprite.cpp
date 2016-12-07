#include "Engine.h"

namespace Batman 
{
    Sprite::Sprite() : Entity(RENDER_2D)
    {
		//basic properties
        image = NULL;
        imageLoaded = false;
        velocity = Vector2(0,0);
        state = 0;
        facingAngle = 0;
        movingAngle = 0;
        color = Color(255,255,255,255);
        pivotPoint = Vector2(0,0);

        //animation properties
        sourcePosition = Vector2(0,0);
        currentFrame = 0;
        firstFrame = 0;
        lastFrame = 0;
        animationDirection = 1;
        animationColumns = 1;

        //transformation properties
        position = Vector2(0,0);
        rotation = 0;
        scale = Vector2(1,1);
        size = Vector2(1,1);
    }
    
	//destructor
    Sprite::~Sprite()  
    {
        if (imageLoaded)
            delete image;
    }
    
	//Load image
    bool Sprite::Load(std::string filename, Color transcolor)
    {
        //de-allocated existing texture
        if (!image) 
        {
            delete image;
            image = NULL;
        }
    
        //create new texture
        image = new Texture();
		//use built in Load model
        if (image->Load(filename,transcolor))
        {
            size.x = image->getWidth();
            size.y = image->getHeight();
            imageLoaded = true;
            return true;
        }
        else
            return false;
    }
    
	//set image (change sprite)
    void Sprite::setImage(Texture *img)
    {
        //protect pointer
        if (!img) 
        {
            debug << "Sprite::setImage: texture is null\n";
            return;
        }
        else
        {
            //free existing image
            if (imageLoaded) 
            {
                delete image;
                image = NULL;
            }
        }

        //set new image
        image = img;
        size.x = image->getWidth();
        size.y = image->getHeight();
        imageLoaded = false;
    }

    //optimized rendering without animation
    void Sprite::RenderFast()
    {
        D3DXVECTOR3 pivot(0.0f, 0.0f, 0.0f);

		//just draw
        g_engine->getSpriteObj()->Draw(
            image->getTexture(),		//texture
            NULL,						
            &pivot,						//pivot / origin
            &position.ToD3DXVECTOR3(),	//position
            color.ToD3DCOLOR() );		//color
    }

    //Entity::Render implementation
    void Sprite::Render()
    {
        Render(false);
    }

    //full animation frame rendering
    void Sprite::Render(bool autoPivot)
    {
        //if autopivot, this calculates the centre
		if (autoPivot)
        {
            pivotPoint.x = (float)((size.x*scale.x)/2);
            pivotPoint.y = (float)((size.y*scale.y)/2);
        }

		//set origin, position and scale
        D3DXVECTOR2 center((float)pivotPoint.x, (float)pivotPoint.y);
        D3DXVECTOR2 trans((float)position.x, (float)position.y);
        D3DXVECTOR2 scale((float)scale.x,(float)scale.y);
		//Use the scale, center, rotation and 
		//trans values to get transform matrix
        D3DXMatrixTransformation2D(&matTransforms,NULL,0,
									&scale,&center,(float)rotation,&trans);
		//set the transform matrix to the sprite
        g_engine->getSpriteObj()->SetTransform(&matTransforms);

        //draw animation frame
        int fx = (int)(((int)currentFrame % animationColumns) * size.x);
        int fy = (int)(((int)currentFrame / animationColumns) * size.y);
        RECT srcRect = {fx,fy, (int)(fx+size.x), (int)(fy+size.y)};

		//use the Draw method to render the sprite
        g_engine->getSpriteObj()->Draw(image->getTexture(),
										&srcRect,NULL,NULL,
										color.ToD3DCOLOR());
        //set identity matrix (reset)
        g_engine->setSpriteIdentity();
    }
    
    void Sprite::Update(float deltaTime)
    {
        //move sprite by velocity amount
        position.x += velocity.x * deltaTime;
        position.y += velocity.y * deltaTime;

        //increment animation frame
        currentFrame += (float)((float)animationDirection * deltaTime);

        //keep current frame within bounds    
        if ((int)currentFrame < firstFrame) currentFrame = (float)lastFrame;
        if ((int)currentFrame > lastFrame) currentFrame = (float)firstFrame;
    }

    Rect Sprite::getBounds()
    {
		//calculate bounding box
        Rect rect;
        rect.left = position.x;
        rect.top = position.y;
        rect.right = position.x + size.x * scale.x;
        rect.bottom = position.y + size.y * scale.y;
        return rect;
    }
}