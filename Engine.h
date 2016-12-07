//Our main engine class!! Here comes lots of header files...

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>			//Windows functions
#include <iostream>				//input / output streams
#include <map>					//STL map data structure (hash tables)
#include <list>					//STL list data structure
#include <vector>				//STL vector data structure
#include <string>				//string vars
#include <sstream>				//string streams
#include <fstream>				//file streams
#include <iomanip>				//input-output manipulation
#include <ctime>				//time functions
#include <cstdio>				//input / output functions 
#include <cstdlib>				//standard library functions
#include <cmath>				//math functions
#include <io.h>					//another input / output header
#include <algorithm>			//standard algorithms
#include <boost/timer.hpp>		//boost timer functions
#include <boost/foreach.hpp>	//boost for-each loop
#include <boost/format.hpp>		//boost format helpers

//DirectX Headers
#define DIRECTINPUT_VERSION 0x0800
#include <d3d9.h>	//Direct3D9
#include <d3dx9.h>	//Additional D3D9 functions
#include <dinput.h> //DirectInput
#include <XInput.h> //XBox Pads
#include <mmsystem.h>	//for audio
#include <mmreg.h>		//for audio
#include <dsound.h>		//for audio

//engine class headers
#include "Entity.h"
#include "Timer.h"
#include "Input.h"
#include "XboxPad.h"
#include "SoundLayer.h"
#include "Sound.h"
#include "Event.h"
#include "Font.h"
#include "LogFile.h"
#include "Vector.h"
#include "Math.h"
#include "EAngle.h"
#include "Matrix.h"
#include "Camera.h"
#include "Effect.h"
#include "Color.h"
#include "Texture.h"
#include "Mesh.h"
#include "BoneMesh.h"
#include "Skybox.h"
#include "Rect.h"
#include "Sprite.h"
#include "ParticleEmitter.h"
#include "BitmapFont.h"
#include "Ray.h"
#include "Collectable.h"
#include "Weapon.h"
#include "Enemy.h"

//required libraries
/*#pragma comment(lib, "d3d9.lib")	//Direct3D9
#pragma comment(lib, "d3dx9.lib")	//D3D9 extras
#pragma comment(lib, "dsound.lib")	//DirectSound
#pragma comment(lib, "dinput8.lib")	//DirectInput8
#pragma comment(lib, "xinput.lib")	//X-Input (pads)
#pragma comment(lib, "dxguid.lib")	//Global ID
#pragma comment(lib, "winmm.lib")	//Windows MultiMedia
#pragma comment(lib, "user32.lib")	//UI helpers
#pragma comment(lib, "gdi32.lib")	//Windows GDI*/			//added in link properties

//Version Definitions
#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define REVISION 0

//end user functions - game loop
//externally defined - main.cpp
//preload game - set init engine properties
extern bool game_preload();
//initialise game assets
extern bool game_init(HWND hwnd);
//update logic
extern void game_update(float deltaTime);
//render 3D objects
extern void game_render3d();
//render 2d objects
extern void game_render2d();
//game event handling
extern void game_event(Batman::IEvent* e);
//what happens at shutdown
extern void game_end();

//windows callback - function prototype
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam);

namespace Batman
{
	//template function to convert values to string
	template <class T>
	std::string static ToString(const T &t, int places = 2)
	{
		std::ostringstream oss;
		oss.precision(places); //set decimal places
		oss.setf(std::ios_base::fixed); //gets format right
		oss << t;
		return oss.str();
	}

	class Engine
	{
	private:
		std::string p_commandLineParams;					//cmd line
		int p_versionMajor, p_versionMinor, p_revision;		//versions
		HWND p_windowHandle;								//window handle
		LPDIRECT3D9 p_d3d;									//D3D pointer
		LPDIRECT3DDEVICE9 p_device;							//D3D device pointer
		LPD3DXSPRITE p_spriteObj;							//D3D sprite pointer
		std::string p_apptitle;								//app title
		bool p_fullscreen;									//fullscreen?
		int p_screenwidth;									//screen width
		int p_screenheight;									//screen height
		int p_colordepth;									//color depth
		bool p_pauseMode;									//pause mode?
		Timer p_coreTimer;									//core frame timer
		long p_coreFrameCount;								//core frame count
		long p_coreFrameRate;								//core frame rate
		Timer p_screenTimer;								//screen frame timer
		long p_screenFrameCount;
		long p_screenFrameRate;
		Timer timedUpdate;									//timed update
		D3DCOLOR p_backdropColor;							//window bg color

		//primary surface pointers used when restoring render target
		LPDIRECT3DSURFACE9 p_MainSurface;
		LPDIRECT3DSURFACE9 p_MainDepthStencilSurface;

		//input pointer and input methods
		Input *p_input;
		XboxPad *p_padinput;
		void updateKeyboard();
		void updateMouse();

		//entity management
		bool p_globalCollision;
		std::vector<Entity*> p_entities;
		void updateEntities(float deltaTime);
		void drawEntities();
		void entitiesRender3D();
		void entitiesRender2D();

	public:
		//constructor
		Engine();
		//destructor
		virtual ~Engine();
		//initialisation method
		bool Init(HINSTANCE hInstance, int width, int height,
			int colordepth, bool fullscreen);
		//update method
		void Update(float deltaTime);
		//create message box
		void Message(std::string message,
			std::string title="Engine");
		//fatal error method
		void fatalError(std::string message,
			std::string title="FATAL ERROR");
		//shutdown engine
		void Shutdown();
		//clear scene
		void clearScene(D3DCOLOR color);
		//set identity matrix
		void setIdentity();
		//set sprite identity
		void setSpriteIdentity();
		//release memory
		int Release();
		//manipulate render targets
		//(what's being rendered to)
		void savePrimaryRenderTarget();
		void restorePrimaryRenderTarget();

		//accessor / mutator function expose private vars
		//paused get / set
		bool isPaused() { return p_pauseMode; }
		void setPaused(bool value) { p_pauseMode = value; }

		//getters for D3D objects
		LPDIRECT3D9 getObject() { return p_d3d; }
		LPDIRECT3DDEVICE9 getDevice() { return p_device; }
		LPD3DXSPRITE getSpriteObj() { return p_spriteObj; }

		//get / set window handle
		void setWindowHandle(HWND hwnd) { p_windowHandle = hwnd; }
		HWND getWindowHandle() { return p_windowHandle; }

		//get / set app title
		std::string getAppTitle() { return p_apptitle; }
		void setAppTitle(std::string value) { p_apptitle = value; }

		//get version text / numbers
		int getVersionMajor() { return p_versionMajor; }
		int getVersionMinor() { return p_versionMinor; }
		int getRevision() { return p_revision; }
		std::string getVersionText();

		//get core / screen FPS
		long getCoreFrameRate() { return p_coreFrameRate; }
		long getScreenFrameRate() { return p_screenFrameRate; }

		//get screen properties
		void setScreen(int w, int h, int d, bool full);
		int getScreenWidth() { return p_screenwidth; }
		void setScreenWidth(int value) { p_screenwidth = value; }
		int getScreenHeight() { return p_screenheight; }
		void setScreenHeight(int value) { p_screenheight = value; }
		int getColorDepth() { return p_colordepth; }
		void setColorDepth(int value) { p_colordepth = value; }
		int getFullScreen() { return p_fullscreen; }
		void setFullscreen(bool value) { p_fullscreen = value; }

		//get / set backdrop color
		D3DCOLOR getBackdropColor() { return p_backdropColor; }
		void setBackdropColor(D3DCOLOR value) { p_backdropColor = value; }

		//command line params
		std::string getCommandLineParams() { return p_commandLineParams; }
		void setCommandLineParams(std::string value) { p_commandLineParams = value; }

		//event system
		void raiseEvent(IEvent*);

		//entity support
		void addEntity(Entity* entity);
		int getEntityCount(enum EntityType entityType);
		Entity* findEntity(int id);
		Entity* findEntity(std::string name);
		void buryEntities();

		//collision support
		void enableGlobalCollisions() {p_globalCollision = true;}
		void disableGlobalCollisions() {p_globalCollision = false;}
		void setGlobalCollisions(bool value) {p_globalCollision = value;}
		bool getGlobalCollisions() {return p_globalCollision;}

		//collision overloads
		void testForCollisions();
		bool Collision(Entity* entity1, Entity* entity2);
		bool Collision(Sprite* sprite1, Sprite* sprite2);
		bool Collision(Mesh* mesh1, Mesh* mesh2);
		bool PlayerToSceneryCollision(Mesh* player, Mesh* scenery);

		bool RayAABBCollision(Mesh* player, Ray* shot);

	};//class
};//namespace

//define the global engine object (visible everywhere)
//the auto_ptr takes responsibility for deleting the memory
//related to the object
extern std::auto_ptr<Batman::Engine> g_engine;