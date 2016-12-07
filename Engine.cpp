#include "Engine.h"
using namespace std;

namespace Batman
{
	//constructor
	Engine::Engine()
	{
		//initialise all vars with default vals
		p_apptitle = "Batman Engine";
		p_screenwidth = 640;
		p_screenheight = 480;
		p_colordepth = 32;
		p_fullscreen = false;
		p_device = NULL;
		p_coreFrameCount = 0;
		p_coreFrameRate = 0;
		p_screenFrameCount = 0;
		p_screenFrameRate = 0;
		//D3D Blue
		p_backdropColor = D3DCOLOR_XRGB(0, 0, 80);
		p_windowHandle = 0;
		p_pauseMode = false;
		p_versionMajor = VERSION_MAJOR;
		p_versionMinor = VERSION_MINOR;
		p_revision = REVISION;
		p_commandLineParams = " ";

		//null render target variables
		p_MainSurface = 0;
		p_MainDepthStencilSurface = 0;

		//window handle must be set later for DirectX
		p_windowHandle = 0;
	}

	//destructor
	Engine::~Engine()
	{
		//delete objects in order of creation
		delete p_input;
		if (p_device) p_device->Release();
		if (p_d3d) p_d3d->Release();
	}

	//returns string text of engine version using string stream
	std::string Engine::getVersionText()
	{
		std::ostringstream s;
		s << "Batman Engine v" << p_versionMajor << ". "
			<< p_versionMinor << "." << p_revision;
		return s.str();
	}

	//create message box from string and title
	void Engine::Message(std::string message, std::string title)
	{
		MessageBox(0, message.c_str(), title.c_str(), 0);
	}

	//set screen properties using setter methods
	void Engine::setScreen(int w, int h, int d, bool full)
	{
		setScreenWidth(w);
		setScreenHeight(h);
		setColorDepth(d);
		setFullscreen(full);
	}

	//initialise engine (window, D3D device, input, renderer)
	bool Engine::Init(HINSTANCE hInstance, int width, int height,
						int colordepth, bool fullscreen)
	{
		//get window caption string from engine
		string title;
		title = g_engine->getAppTitle();

		//set window dimensions (begins with engine defaults)
		RECT windowRect;
		windowRect.left = 0;
		windowRect.right = g_engine->getScreenWidth();
		windowRect.top=0;
		windowRect.bottom = g_engine->getScreenHeight();

		//create window class structure
		WNDCLASSEX wc;						//struct def
		memset(&wc, 0, sizeof(WNDCLASS));
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = (WNDPROC)WndProc;
		wc.hInstance = hInstance;
		wc.lpszClassName = title.c_str();
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hIcon = 0;
		wc.hIconSm = 0;
		wc.hbrBackground = 0;
		wc.lpszMenuName = 0;

		//setup the window with the class info
		RegisterClassEx(&wc);

		//setup in windowed or fullscreen?
		DWORD dwStyle, dwExStyle;
		if(g_engine->getFullScreen())
		{
			//struct for screen setup
			DEVMODE dm;
			memset(&dm, 0, sizeof(dm));
			dm.dmSize = sizeof(dm);
			dm.dmPelsWidth =			//pixel width of visible screen
				g_engine->getScreenWidth();
			dm.dmPelsHeight =
				g_engine->getScreenHeight();
			dm.dmBitsPerPel =
				g_engine->getColorDepth();
			dm.dmFields = 
				DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			//attempt to change to fullscreen
			if(ChangeDisplaySettings(&dm, CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
			{
				//if failed, debd message and set to false
				debug << "Display mode change failed" << std::endl;
				g_engine->setFullscreen(false);
			}
			//constants for window setup
			dwStyle = WS_POPUP;
			dwExStyle = WS_EX_APPWINDOW;
		}
		else
		{
			//constants for window setup
			dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
			dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		}

		//adjust window to true requested size
		AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

		//create program window
		int wwidth = windowRect.right - windowRect.left;
		int wheight = windowRect.bottom - windowRect.top;

		//some output for log
		debug << "Screen size: " << width << ", " << height << endl;
		debug << "Creating a program window" << endl;

		//create window functions
		HWND hWnd = CreateWindowEx(
			0,
			title.c_str(),
			title.c_str(),
			dwStyle |
			WS_CLIPCHILDREN |
			WS_CLIPSIBLINGS,
			0,0,
			wwidth,
			wheight,
			0,
			0,
			hInstance,
			0
			);

		//was there an error creating the window
		if(!hWnd)
		{
			//output for log
			debug << "error creating program window" << endl;
			return 0;
		}

		//display the window
		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);

		//save window handle in engine
		g_engine->setWindowHandle(hWnd);

		//output log
		debug << "Creating direct3d object" << endl;

		//initialise direct3d
		p_d3d = Direct3DCreate9(D3D_SDK_VERSION);
		if(p_d3d == NULL)
		{
			return 0;
		}

		//get system desktop color depth
		D3DDISPLAYMODE dm;
		//display mode saved at dm location
		p_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm);

		//set configuration options for direct3d
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.Windowed = (!fullscreen);
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

		d3dpp.EnableAutoDepthStencil = 1;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		d3dpp.Flags =
			D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
		d3dpp.PresentationInterval =
			D3DPRESENT_INTERVAL_IMMEDIATE;
		d3dpp.BackBufferFormat = dm.Format;
		d3dpp.BackBufferCount = 1;
		d3dpp.BackBufferWidth = width;
		d3dpp.BackBufferHeight = height;
		d3dpp.hDeviceWindow = p_windowHandle;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;

		//output log
		debug << "Creating direct3d device" << endl;

		//create direct3d device (hardware T&L)
		p_d3d->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				p_windowHandle,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp,
				&p_device
			);

		//if hardware T&L fails, try software
		if(p_device==NULL)
		{
			//output log
			debug << "hardware vertex option failed! trying software ..." << endl;

			p_d3d->CreateDevice(
					D3DADAPTER_DEFAULT,
					D3DDEVTYPE_HAL,
					p_windowHandle,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp,
					&p_device
				);

			//has it worked? tell output log the result
			if(p_device==NULL)
			{
				debug << "software vertex option failed; shutting down." << endl;
				return 0;
			}
			else
			{
				debug << "software vertex processing" << endl;
			}

		}
		else
		{
			//output log
			debug << "hardware vertex processing" << endl;
		}

		//output log
		debug << "creating 2d renderer" << endl;

		//initialise 2d renderer / error handling
		HRESULT result = D3DXCreateSprite(
			p_device,
			&p_spriteObj
			);
		if(result != D3D_OK)
		{
			debug << "D3DXCreateSprite failed" << endl;
			return 0;
		}

		//initialise directinput
		debug << "init input system" << endl;
		p_input = new Input(getWindowHandle());
		p_padinput = new XboxPad(1);

		//output log
		debug << "calling game_init(" << getWindowHandle() << ")" << endl;

		//call game initialisation extern function
		if(!game_init(getWindowHandle())) return 0;

		//output log
		debug << "engine init succeeded" << endl;

		return 1;
	}

	/**
	Resets transforms by setting the identity matrix
	**/
	void Engine::setIdentity()
	{
		D3DXMATRIX identity;
		D3DXMatrixIdentity(&identity);
		g_engine->getSpriteObj()->SetTransform(&identity);
	}

	void Engine::setSpriteIdentity()
	{
		D3DXMATRIX identity;
		D3DXMatrixIdentity(&identity);
		g_engine->getSpriteObj()->SetTransform(&identity);
	}

	/**
	Saving and restoring the render target is used when rendering to a texture
	**/
	void Engine::savePrimaryRenderTarget()
	{
		//save primary rendering & depth stencil surfaces
		p_device->GetRenderTarget(0, &p_MainSurface);
		p_device->GetDepthStencilSurface(&p_MainDepthStencilSurface);
	}

	void Engine::restorePrimaryRenderTarget()
	{
		//restore normal render target
		p_device->SetRenderTarget(0, p_MainSurface);
		p_device->GetDepthStencilSurface(&p_MainDepthStencilSurface);
	}

	void Engine::clearScene(D3DCOLOR color)
	{
		//clears the scene with bg colour
		p_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			color, 1.0f, 0);
	}

	void Engine::Update(float elapsedTime)
	{
		static float accumTime=0;

		//calculate core framerate
		//increment core frame count
		p_coreFrameCount++;
		//if second passed, update rate value
		if(p_coreTimer.StopWatch(1000))
		{
			p_coreFrameRate = p_coreFrameCount;
			p_coreFrameCount = 0;
		}

		//fast update (logic)
		game_update(elapsedTime);

		//ENTITY MANAGEMENT
		//update entities
		if(!p_pauseMode)
		{
			updateEntities(elapsedTime);
			testForCollisions();	//collision testing
		}

		//60fps ~16ms per frame
		if(!timedUpdate.StopWatch(16))
		{
			//free the cpu for 1 ms
			timedUpdate.Rest(1);
		}
		else
		{
			//calculate real framerate
			p_screenFrameCount++;
			if(p_screenTimer.StopWatch(1000))
			{
				p_screenFrameRate = p_screenFrameCount;
				p_screenFrameCount=0;
			}

			//update input devices
			p_input->Update();
			updateKeyboard();
			updateMouse();

			//begin rendering
			if(p_device->BeginScene()==D3D_OK)
			{
				//clear scene
				g_engine->clearScene(p_backdropColor);

				//ENTITY MANAGEMENT
				entitiesRender3D();

				//3d rendering
				game_render3d();

				//2d rendering (using sprite object)
				p_spriteObj->Begin(D3DXSPRITE_ALPHABLEND);

				//ENTITY MANAGEMENT
				entitiesRender2D();

				game_render2d();

				p_spriteObj->End();

				//finish scene and refresh
				p_device->EndScene();
				p_device->Present(0,0,0,0);
			}
		}

		//ENTITY MANAGEMENT
		//clean up expired entities
		buryEntities();
	}

	void Engine::updateMouse()
	{
		static int oldPosX = 0;
		static int oldPosY = 0;

		//check mouse buttons
		for(int n=0; n<4; n++)
		{
			if(p_input->GetMouseButton(n))
			{
				//launch event
				raiseEvent(new MouseClickEvent(n));
			}
		}

		//check mouse position
		int posx = p_input->GetMousePosX();
		int posy = p_input->GetMousePosY();
		if(posx!=oldPosX || posy != oldPosY)
		{
			int oldPosX = p_input->GetMousePosX();
			int oldPosY = p_input->GetMousePosY();
			//launch event
			raiseEvent(new MouseMoveEvent(posx, posy));
		}

		//check mouse motion
		int deltax = p_input->GetMouseDeltaX();
		int deltay = p_input->GetMouseDeltaY();
		if(deltax!=0||deltay!=0)
		{
			//launch event
			raiseEvent(new MouseMotionEvent(deltax, deltay));
		}

		//check mouse wheel
		int wheel = p_input->GetMouseDeltaWheel();
		if(wheel!=0)
		{
			//launch event
			raiseEvent(new MouseWheelEvent(wheel));
		}

		XINPUT_STATE PadState = p_padinput->GetState();
		XboxPad* padPtr = p_padinput;
		raiseEvent(new XButtonEvent(PadState, padPtr));
	}

	void Engine::updateKeyboard()
	{
		static char old_keys[256];

		for(int n=0; n<255; n++)
		{
			//check for key press
			if(p_input->GetKeyState(n) & 0x80)
			{
				old_keys[n] = p_input->GetKeyState(n);
				//launch event
				raiseEvent(new KeyPressEvent(n));
			}
			//check for release
			else if(old_keys[n] & 0x80)
			{
				old_keys[n] = p_input->GetKeyState(n);
				//launch event
				raiseEvent(new KeyReleaseEvent(n));
			}
		}
	}

	void Engine::Shutdown()
	{
		PostQuitMessage(0);
	}

	void Engine::raiseEvent(IEvent* e)
	{
		//call game event function
		game_event(e);
		delete e;
	}

	//ENTITY MANAGEMENT

	//adds entity to entity vector
	void Engine::addEntity(Entity* entity)
	{
		//log text
		debug << "Engine::addEntity: received new entity\n";
		//counter for unique ids
		static int id = 0;
		//set entity id
		entity->setID(id);
		//add entity to vector
		p_entities.push_back(entity);
		//increment id
		id++;
	}

	//update entities
	void Engine::updateEntities(float deltaTime)
	{
		//loop through each entity
		BOOST_FOREACH(Entity* entity, p_entities)
		{
			//is this entity alive?
			if(entity->getAlive())
			{
				//move/animate entity
				entity->Update(deltaTime);

				//tell game that this entity has been updated
				raiseEvent(new EntityUpdateEvent(entity));

				//see if this entity will auto-expire
				if(entity->getLifetime() > 0)
				{
					//update the entities life counter
					entity->addToLifetimeCounter(deltaTime*1000);

					//if life expired, set alive to false
					if(entity->lifetimeExpired())
					{
						entity->setAlive(false);
					}
				}
			}
		}
	}

	//remove dead entities
	void Engine::buryEntities()
	{
		//an iterator will work better than BOOST_FOREACH in this case
		std::vector<Entity*>::iterator iter = p_entities.begin();
		while(iter != p_entities.end())
		{
			//if entity not alive, erase from vector
			if((*iter)->getAlive() == false)
			{
				iter = p_entities.erase(iter);
			}
			else iter++;
		}
	}

	//render 3d entities
	void Engine::entitiesRender3D()
	{
		BOOST_FOREACH( Entity* entity, p_entities )
		 {
			//is this 3D entity
			if ( entity->getRenderType() == RENDER_3D ) 
			{
				//is this entity in use
				if ( entity->getAlive() && entity->getVisible() ) 
				{
					//use this if global rendering
					//entity->Render();
					//raiseEvent( new EntityRenderEvent( entity ) );
				}
			}
		}
	}

	//render 2d entities
	void Engine::entitiesRender2D()
	{
		BOOST_FOREACH( Entity* entity, p_entities )
		{
			//is this 2D entity
			if ( entity->getRenderType() == RENDER_2D ) 
			{
				//is this entity in use
				if ( entity->getAlive() && entity->getVisible() ) 
				{
					//use this if global rendering
					//entity->Render();
					//raiseEvent( new EntityRenderEvent( entity ) );
				}
			}
		}
	}

	//find entity by name
    Entity *Engine::findEntity(std::string name)
    {
        BOOST_FOREACH( Entity* entity, p_entities )
        {
			if ( entity->getAlive() && entity->getName() == name ) return entity;
        }
        return NULL;
    }

	//find entity by id
    Entity *Engine::findEntity(int id)
    {
        BOOST_FOREACH( Entity* entity, p_entities )
        {
			if ( entity->getAlive() && entity->getID() == id ) return entity;
        }
        return NULL;
    }

    //count 'living' entities by type
    int Engine::getEntityCount(enum EntityType entityType)
    {
		int total = 0;

        BOOST_FOREACH( Entity* entity, p_entities )
        {
			if ( entity->getAlive() && entity->getEntityType() == entityType ) total++;
        }
        return total;
    }

	//main collision test
	void Engine::testForCollisions()
    {
		//reset all collided properties
        BOOST_FOREACH( Entity* entity, p_entities )
		{
			entity->setCollided(false);
			entity->setCollideBuddy(NULL);
		}

		//escape if global collisions are disabled
		if (!p_globalCollision) return;

		//loop through all entities (first)
		BOOST_FOREACH( Entity* first, p_entities )
		{
			if (first->getAlive() && first->isCollidable())
			{
				//test all other entities for collision (second)
				BOOST_FOREACH(Entity* second, p_entities)
				{
					//don't test object with itself
					if (second->getID() != first->getID())
					{
						if (second->getAlive() && second->isCollidable()) 
                        {
							//test for collision
							if (Collision( first, second )) 
							{
								//set collision flags and buddies
								first->setCollided(true);
								first->setCollideBuddy(second);
								second->setCollided(true);
								second->setCollideBuddy(first);
							}//if
						}//if
					}//if
				}//foreach
			}//if
		}//foreach
	}

	//main collision check between entity types
	bool Engine::Collision(Entity* entity1, Entity* entity2)
	{
		switch(entity1->getEntityType())
		{
			//determine if the first entity is a player
			case ENTITY_PLAYER_MESH:
				switch(entity2->getEntityType())
				{
					//if second one a scenery mesh
					case ENTITY_SCENERY_MESH:
						return PlayerToSceneryCollision((Mesh*)entity1,(Mesh*)entity2);
						break;
					case ENTITY_COLLECTABLE_MESH:
						return Collision((Mesh*)entity1,(Mesh*)entity2);
						break;
				}
			case ENTITY_ENEMY_MESH:
				switch(entity2->getEntityType())
				{
					//if second one is a mesh
					case ENTITY_WEAPON_MESH:
						return Collision((Mesh*)entity1,(Mesh*)entity2);
						break;
				}
			case ENTITY_RAY:
				switch(entity2->getEntityType())
				{
					//if second one is player
					case ENTITY_PLAYER_MESH:
						return RayAABBCollision((Mesh*)entity2,(Ray*)entity1);
						break;
				}
		}//switch
		return false;
	}//function

	//sprite to sprite collisions
	bool Engine::Collision(Sprite* sprite1, Sprite* sprite2)
	{
		//use 2d AABB based on previously created sprite methods
		Rect r1 = sprite1->getBounds();
		Rect r2 = sprite2->getBounds();

		if(r1.Intersects(r2)) return true;
		else return false;
	}

	//mesh to mesh collisions
	bool Engine::Collision(Mesh* mesh1, Mesh* mesh2)
	{
		if(mesh1->getPosition().x+mesh1->getBoundingBox().min.x > 
			mesh2->getPosition().x+mesh2->getBoundingBox().max.x) return false;

		if(mesh1->getPosition().y+mesh1->getBoundingBox().min.y > 
			mesh2->getPosition().y+mesh2->getBoundingBox().max.y) return false;

		if(mesh1->getPosition().z+mesh1->getBoundingBox().min.z > 
			mesh2->getPosition().z+mesh2->getBoundingBox().max.z) return false;
	
		if(mesh1->getPosition().x+mesh1->getBoundingBox().max.x < 
			mesh2->getPosition().x+mesh2->getBoundingBox().min.x) return false;

		if(mesh1->getPosition().y+mesh1->getBoundingBox().max.y < 
			mesh2->getPosition().y+mesh2->getBoundingBox().min.y) return false;

		if(mesh1->getPosition().z+mesh1->getBoundingBox().max.z < 
			mesh2->getPosition().z+mesh2->getBoundingBox().min.z) return false;
	
		//if none of the tests above are valid, there's a collision
		return true;
	}

	bool Engine::PlayerToSceneryCollision(Mesh* player, Mesh* scenery)
	{

		//check the AABB collision first - if no collision return false
		if(!Collision(player,scenery))
			return false;

		//if collision, update player's position to previous position
		player->restorePreviousPosition();

		//if none of above tests are valid, theres a collision
		return true;
	}

	//ray AABB collision
	bool Engine::RayAABBCollision(Mesh* player, Ray* shot)
	{
        int hasHit;
        float distanceToCollision;

		//create vector from ray origin for length of 100
		Vector3 rayTestVec0 = shot->RayBegin;
		Vector3 rayTestVec1 = shot->RayBegin + shot->direction * 100;

		//get aabb details
		Vector3 BoxMin = player->getPosition()+player->getBoundingBox().min;
		Vector3 BoxMax = player->getPosition()+player->getBoundingBox().max;

		//set 0 as beginning and 1 as end value
		float fractionLow = 0;
		float fractionHigh = 1;

		//clip x
		//x distance from box min/max to ray begin / full vector x distance
		float fractionLowX = (BoxMin.x - rayTestVec0.x) / (rayTestVec1.x - rayTestVec0.x);
		float fractionHighX = (BoxMax.x - rayTestVec0.x) / (rayTestVec1.x - rayTestVec0.x);
		
		//ensure the high/low values are correct
		if(fractionHighX < fractionLowX) swap(fractionLowX,fractionHighX);

		//if the max x clip is less than 0, no intersection
		if(fractionHighX < fractionLow) return false;

		//if the min x clip is greater than 1, no intersection
		if(fractionLowX > fractionHigh)	return false;

		//reset the low/high fractions of the line to the min/max x clip
		//this clips the line
		fractionLow = max(fractionLowX,fractionLow);
		fractionHigh = min(fractionHighX,fractionHigh);

		//if the low clip is greater than the high clip, no intersection
		if(fractionLow > fractionHigh)	return false;

		//clip y
		//y distance from box min/max to ray begin / full vector y distance
		float fractionLowY = (BoxMin.y - rayTestVec0.y) / (rayTestVec1.y - rayTestVec0.y);
		float fractionHighY = (BoxMax.y - rayTestVec0.y) / (rayTestVec1.y - rayTestVec0.y);
		
		//ensure the high/low values are correct
		if(fractionHighY < fractionLowY)	swap(fractionLowY,fractionHighY);

		//if the max y clip is less than 0, no intersection
		if(fractionHighY < fractionLow)	return false;

		//if the min y clip is greater than 1, no intersection
		if(fractionLowY > fractionHigh)	return false;

		//reset the low/high fractions of the line to the min/max y clip
		//this clips the line
		fractionLow = max(fractionLowY,fractionLow);
		fractionHigh = min(fractionHighY,fractionHigh);

		//if the low clip is greater than the high clip, no intersection
		if(fractionLow > fractionHigh)	return false;

		//clip z
		//z distance from box min/max to ray begin / full vector z distance
		float fractionLowZ = (BoxMin.z - rayTestVec0.z) / (rayTestVec1.z - rayTestVec0.z);
		float fractionHighZ = (BoxMax.z - rayTestVec0.z) / (rayTestVec1.z - rayTestVec0.z);
		
		//ensure the high/low values are correct
		if(fractionHighZ < fractionLowZ)	swap(fractionLowZ,fractionHighZ);

		//if the max z clip is less than 0, no intersection
		if(fractionHighZ < fractionLow)	return false;

		//if the min z clip is greater than 1, no intersection
		if(fractionLowZ > fractionHigh)	return false;

		//reset the low/high fractions of the line to the min/max z clip
		//this clips the line
		fractionLow = max(fractionLowZ,fractionLow);
		fractionHigh = min(fractionHighZ,fractionHigh);

		//if the low clip is greater than the high clip, no intersection
		if(fractionLow > fractionHigh)	return false;

		//----------------------------------------------------------------------

		//here we have an intersection point

		//get full line length
		Vector3 raySection = rayTestVec1 - rayTestVec0;

		//get the intersection point / the low clip
		/*this is where the bullet entered, using the high clip
		//you could do some exit wound stuff*/
		//calculate distance from origin using line portion/fraction
		Vector3 intersection = rayTestVec0 + raySection *  fractionLow;

		//update ray class with intersection point
		shot->intersectionPoint = intersection;

		//collision true
		return true;
	}
}; //namespace