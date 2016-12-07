#include <stdlib.h>
#include <time.h>
#include "Engine.h"
using namespace std;
using namespace Batman;

//GLOBALS: Core Object Pointers
Effect* effect = NULL;
Effect* skyboxEffect = NULL;
Skybox* skybox = NULL;
Camera* camera = NULL;
Mesh* floorBox = NULL;
Mesh* playerCollision = NULL;
Texture* terrainTex = NULL;
Font* debugText = NULL;
Font* dialogueText = NULL;
Sprite* mapBg = NULL;
Sprite* charMap = NULL;
Sprite* HUDBg = NULL;
Sprite* dialogueWindow = NULL;
Sprite* damageHUD = NULL;
bool damageHUDSwitch = false;
Sprite* ninjaFaceAnim = NULL;
Mesh* mushroomObject = NULL;
Texture* mushroomTex = NULL;
Mesh* treeObject = NULL;
Mesh* bushObject = NULL;
Texture* bushTex = NULL;

//GLOBALS: Weapons and collectables stuff
Mesh* shurikenWeaponMesh = NULL;
Mesh* shurikenCollectMesh = NULL;
Texture* shurikenTex = NULL;
Mesh* dartWeaponMesh = NULL;
Mesh* dartCollectMesh = NULL;
Texture* dartTex = NULL;
Mesh* bombWeaponMesh = NULL;
Mesh* bombCollectMesh = NULL;
Texture* bombTex = NULL;
Mesh* starCollectMesh = NULL;
Texture* starTex = NULL;

//GLOBALS: Weapon HUD Animations
Sprite* shurikenAnim = NULL;
Sprite* dartAnim = NULL;
Sprite* bombAnim = NULL;

//GLOBALS: Magic Fairy Fountain
Mesh* fountainMesh = NULL;
Texture* fountainTex = NULL;
Timer fountainTime;
bool fountainCutscene = false;
bool fountainActive = false;
float fountainScale = 0.01;
Sprite* letterBox = NULL;

//GameLogicUpdate vars
Timer logicTimer;
Timer damageHUDTimer;
bool displayDialogueBG = false;
bool startDialogue1 = false;
int d1Stage = 0; //dialogue text stage
int d2Stage = 0;
int d3Stage = 0; //malfunction text stage
bool lockPlayerControls = false;
int rnum = 0;//var for random number storage for d2 dialogue
bool enemyIsDead = false;
bool gotShurikens = false;
bool gotDarts = false;
bool hideMap = true;

//xbox controller vars
float delta=0;
int xboxbutton=0;
float rightTrigger = 0;
float leftTrigger = 0;
bool fireSwitch = false;
int ltVal = 0;
int rtVal = 0;
int leftX = 0;
int leftY = 0;
int rightX = 0;
int rightY = 0;

//GLOBALS: Vectors for common objects
std::vector<Entity*> sceneryBoxes;
std::vector<Entity*> sceneryBoxesBORDER;
std::vector<Entity*> sceneryBoxesMap;
std::vector<Entity*> collectables;
std::vector<Entity*> collectablesMap;
std::vector<Vector3> mushroomPositions;
std::vector<Vector3> treePositions;
std::vector<Texture*> treeTextures;
std::vector<Vector3> bushPositions;
std::vector<Weapon*> weapons; //holds all player-fired weapons
std::vector<Enemy*> enemies;
std::vector<Entity*> enemyShots;

//GLOBALS: Game mechanics values
int shurikens = 0;
int bombs = 0;
int darts = 0;
int pixiestars =  0;
int health = 0;
bool seeCollision = false;
int selectedWeapon = 4;
#define SELECT_SHURIKENS 1
#define SELECT_DARTS 2
#define SELECT_BOMBS 3
#define SELECT_NONE 4

//GLOBALS: Lighting
Vector3 lightDirection(1,0,0);
float lightRed = 1.0;
float lightGreen = 1.0;
float lightBlue = 1.0;
int lightState = 1;
float rotX = 0;
#define LIGHT_RED 1
#define LIGHT_GREEN 2
#define LIGHT_BLUE 3
void adjustLight();
Timer colourTimer;
Timer directionTimer;
bool lightDown = true;

//GLOBALS: Movement
Vector3 currentVelocity;
Vector3 goalVelocity;

EAngle characterToCamera;
Vector3 charMovement;
Vector3 vecForward;
Vector3 vecRight;
Vector3 vecUp;
Vector3 vecBehind;
Vector3 camTarg;
bool charMoving = false;

//GLOBALS: State Machines
int gameState;
#define GAME_PLAY_STATE 1
#define GAME_TITLE_STATE 2
#define GAME_WIN_STATE 3
#define GAME_OVER_STATE 4

//GLOBALS: Player State Mechanics
int playerState = 1;
#define PLAYER_OK 1
#define PLAYER_HURT 2
#define PLAYER_DIE 3
Timer playerStateTimer;

//GLOBALS: Screens
Sprite* titleScreen = NULL;
Sprite* winScreen = NULL;
Sprite* gameOverScreen = NULL;

//helper function prototypes
float Approach(float fGoal, float flCurrent, float dt);
Vector2 ObjectToMap(Vector3 obj);
void Fire();
void InitCollectablesAndEnemies();
void ReInit();

//dialogue window prototypes
void GameLogicUpdate();
void RenderDialogueWindowText();

//GLOBALS: Music and FX
//music
cSound* titleBGM = NULL;
cSound* levelBGM = NULL;
cSound* gameOverBGM = NULL;
cSound* winBGM = NULL;

cSound* splash = NULL;
cSound* electric = NULL;
cSound* punch = NULL;

cSound* speech1 = NULL;
cSound* speech2 = NULL;
cSound* speech3 = NULL;
cSound* speech4 = NULL;
cSound* speech5 = NULL;
cSound* speech6 = NULL;
cSound* speech7 = NULL;

cSound* malfunction = NULL;

//sound switches
bool speech1DoOnce = false;
bool speech2DoOnce = false;
bool speech3DoOnce = false;
bool speech4DoOnce = false;
bool speech5DoOnce = false;
bool speech6DoOnce = false;
bool speech7DoOnce = false;
bool malfunctionDoOnce = false;

//for reinit
HWND windowVal = NULL;

//preload elements
bool game_preload()
{
	g_engine->setAppTitle("HOUSE ROBOT");
	g_engine->setScreen(1600,900,32,false);
	return true;
}

//init elements
bool game_init(HWND hwnd)
{
	windowVal = hwnd;

	//seed random number generator
	srand(time(0));

	//init sound layer
	cSoundLayer::Create(g_engine->getWindowHandle());

	//init music
	titleBGM = new cSound("Audio/Music/TitleBGM.wav");
	levelBGM = new cSound("Audio/Music/LevelBGM.wav");
	gameOverBGM = new cSound("Audio/Music/GameOverBGM.wav");
	winBGM = new cSound("Audio/Music/WinBGM.wav");

	splash = new cSound("Audio/splash.wav");
	electric = new cSound("Audio/electric.wav");
	punch = new cSound("Audio/punch.wav");

	speech1 = new cSound("Audio/speech1.wav");
	speech2 = new cSound("Audio/speech2.wav");
	speech3 = new cSound("Audio/speech3.wav");
	speech4 = new cSound("Audio/speech4.wav");
	speech5 = new cSound("Audio/speech5.wav");
	speech6 = new cSound("Audio/speech6.wav");
	speech7 = new cSound("Audio/speech7.wav");

	malfunction = new cSound("Audio/malfunction.wav");

	//create skybox effect object
	skyboxEffect = new Effect();
	if(!skyboxEffect->Load("Shaders/skybox.fx"))
	{
		debug << "Error loading skybox effect" << std::endl;
		return false;
	}

	//create a skybox
	skybox = new Skybox();
	if(!skybox->Create("Skybox.png"))
	{
		debug << "Error loading skybox" << std::endl;
		return false;
	}

	//set the camera and perspective
	camera = new Camera();
	//this sets the target Euler angles
	characterToCamera.y = 90.0f;
	characterToCamera.p = 0.0f;
	characterToCamera.r = 0.0f;

	//set initial position and target
	//(looking along x axis)
	camera->setPosition(205,5,-230);
	camera->setTarget(1,0,0);
	camera->Update();

	//create invisible player collision mesh
	//(5x5x5 around camera position)
	playerCollision = new Mesh();
	playerCollision->createCube(4,5,4);
	playerCollision->setPosition(camera->getPosition());
	//Entity types set for collision detection
	playerCollision->setEntityType(ENTITY_PLAYER_MESH);
	playerCollision->setAlive(true);
	playerCollision->setCollidable(true);
	g_engine->addEntity(playerCollision);

	//create floor mesh from file
	floorBox = new Mesh();
	if(!floorBox->Load("Scenery/mainplane.x"))
	{
		debug << "Error loading floorbox file" << std::endl;
		return false;
	}

	//load effect file for model shading
	effect = new Effect();
	if(!effect->Load("Shaders/directional_textured.fx"))
	{
		debug << "Error loading effect file" << std::endl;
		return false;
	}

	//load terrain texture from file for floor
	terrainTex = new Texture();
	if(!terrainTex->Load("Scenery/newtex.jpg"))
	{
		debug << "Error loading terrain tex!" << std::endl;
		return false;
	}

	//create font object for debug text
	debugText = new Font("Arial Bold", 20);
	if(!debugText)
	{
		debug << "Error creating font" << std::endl;
		return false;
	}

	//create font object for dialogue text
	dialogueText = new Font("Arial Bold", 50);
	if(!dialogueText)
	{
		debug << "Error creating dialogue font" << std::endl;
		return false;
	}

	//create mini-map from BG from file
	mapBg = new Sprite();
	if(!mapBg->Load("HUD/Map/MapBGSprite.png"))
	{
		debug << "Error creating map" << std::endl;
		return false;
	}

	//set position
	mapBg->setPosition(1300,100);

	//create mini-character
	charMap = new Sprite();
	if(!charMap->Load("HUD/Map/ninjaMapSprite.png"))
	{
		debug << "error char map sprite" << std::endl;
		return false;
	}

	//set initial position on map
	charMap->setPivot(Vector2(5,5));
	charMap->setPosition(1302,102);

	//create HUD background
	HUDBg = new Sprite();
	if(!HUDBg->Load("HUD/HUDbgSprite.png"))
	{
		debug << "Error creating HUD Bg" << std::endl;
		return false;
	}

	//create dialogue window background
	dialogueWindow = new Sprite();
	if(!dialogueWindow->Load("HUD/dialogueWindow.png"))
	{
		debug << "Error creating dialogueWindow Bg" << std::endl;
		return false;
	}

	//create damage HUD
	damageHUD = new Sprite();
	if(!damageHUD->Load("HUD/damageHUD.png"))
	{
		debug << "Error creating damageHUD" << std::endl;
		return false;
	}

	//set position
	HUDBg->setPosition(0,800);
	dialogueWindow->setPosition(400,600);
	damageHUD->setPosition(0,0);

	//create ninja face hud
	ninjaFaceAnim = new Sprite();
	if(!ninjaFaceAnim->Load("HUD/NinjaFaceAnim.png"))
	{
		debug << "Error creating NinjaFaceAnim" << std::endl;
		return false;
	}

	//set position
	ninjaFaceAnim->setPosition(750,800);
	//animation properties
	ninjaFaceAnim->animationColumns=10;
	ninjaFaceAnim->currentFrame=0;
	ninjaFaceAnim->setSize(100,100);
	ninjaFaceAnim->lastFrame=3;
	ninjaFaceAnim->animationDirection=2;

	//gameState = GAME_PLAY_STATE;

	InitCollectablesAndEnemies();

	//BEGIN: TREE LOADING
	//create new mesh for the tree object
	/*treeObject = new Mesh();
	if(!treeObject->Load("Scenery/treecopy.x"))
	{
		debug << "Error loading treeObject!" << std::endl;
		return false;
	}
	treeObject->setPosition(250,0,-250);

	//load textures for trees (each one is a different colour scheme)
	treeTextures.push_back(new Texture());
	if(!treeTextures.back()->Load("Scenery/treeMap0.png"))
	{
		debug << "Error loading tree tex 0!" << std::endl;
		return false;
	}
	treeTextures.push_back(new Texture());
	if(!treeTextures.back()->Load("Scenery/treeMap1.png"))
	{
		debug << "Error loading tree tex 1!" << std::endl;
		return false;
	}
	treeTextures.push_back(new Texture());
	if(!treeTextures.back()->Load("Scenery/treeMap2.png"))
	{
		debug << "Error loading tree tex 2!" << std::endl;
		return false;
	}
	treeTextures.push_back(new Texture());
	if(!treeTextures.back()->Load("Scenery/treeMap3.png"))
	{
		debug << "Error loading tree tex 3!" << std::endl;
		return false;
	}

	//array of map positions
	Vector3 t[36] = {Vector3(25,0,-25), Vector3(75,0,-25), Vector3(125,0,-25),
					Vector3(175,0,-25), Vector3(225,0,-25), Vector3(275,0,-25),
					Vector3(325,0,-25), Vector3(375,0,-25), Vector3(425,0,-25),
					Vector3(475,0,-25), //top row
					//sides
					Vector3(25,0,-75), Vector3(475,0,-75),
					Vector3(25,0,-125), Vector3(475,0,-125),
					Vector3(25,0,-175), Vector3(475,0,-175),
					Vector3(25,0,-225), Vector3(475,0,-225),
					Vector3(25,0,-275), Vector3(475,0,-275),
					Vector3(25,0,-325), Vector3(475,0,-325),
					Vector3(25,0,-375), Vector3(475,0,-375),
					Vector3(25,0,-425), Vector3(475,0,-425),
					//bottom row
					Vector3(25,0,-475),
					Vector3(75,0,-475), Vector3(125,0,-475), Vector3(175,0,-475),
					Vector3(225,0,-475), Vector3(275,0,-475), Vector3(325,0,-475),
					Vector3(375,0,-475), Vector3(425,0,-475), Vector3(475,0,-475),
					};
	//assign array to vector
	treePositions.insert(treePositions.end(),t,t+36);
	//END: TREE LOADING*/

	//TOP BORDER
	{
		//create new Mesh and sprite for their respective scenery vectors
		sceneryBoxesBORDER.push_back(new Mesh());
		sceneryBoxesMap.push_back(new Sprite());
		Mesh* meshPtr = (Mesh*)sceneryBoxesBORDER.back();
		meshPtr->createCube(500,25,50);
		meshPtr->setPosition(250,1,-25);
	
		//create sprite pointer and load map icon
		Sprite* spritePtr = (Sprite*)sceneryBoxesMap.back();
		spritePtr->Load("HUD/Map/bushMap.png");
		//set origin to centre of sprite
		spritePtr->setPivot(Vector2(2,2));
		//set position to the map based on 3D position
		spritePtr->setPosition(ObjectToMap(meshPtr->getPosition()));
		//set entity properties for collision detection
		meshPtr->setCollidable(true);
		meshPtr->setAlive(true);
		meshPtr->setEntityType(ENTITY_SCENERY_MESH);
		g_engine->addEntity(meshPtr);
	}
	//ROOF
	{
		//create new Mesh and sprite for their respective scenery vectors
		sceneryBoxesBORDER.push_back(new Mesh());
		sceneryBoxesMap.push_back(new Sprite());
		Mesh* meshPtr = (Mesh*)sceneryBoxesBORDER.back();
		meshPtr->createCube(135,1,102);
		meshPtr->setPosition(107,14,-200);
	
		//create sprite pointer and load map icon
		Sprite* spritePtr = (Sprite*)sceneryBoxesMap.back();
		spritePtr->Load("HUD/Map/bushMap.png");
		//set origin to centre of sprite
		spritePtr->setPivot(Vector2(2,2));
		//set position to the map based on 3D position
		spritePtr->setPosition(ObjectToMap(meshPtr->getPosition()));
		//set entity properties for collision detection
		meshPtr->setCollidable(true);
		meshPtr->setAlive(true);
		meshPtr->setEntityType(ENTITY_SCENERY_MESH);
		g_engine->addEntity(meshPtr);
	}
	//HOUSE FLOOR
	{
		//create new Mesh and sprite for their respective scenery vectors
		sceneryBoxesBORDER.push_back(new Mesh());
		sceneryBoxesMap.push_back(new Sprite());
		Mesh* meshPtr = (Mesh*)sceneryBoxesBORDER.back();
		meshPtr->createCube(135,1,102);
		meshPtr->setPosition(107,0,-200);
	
		//create sprite pointer and load map icon
		Sprite* spritePtr = (Sprite*)sceneryBoxesMap.back();
		spritePtr->Load("HUD/Map/bushMap.png");
		//set origin to centre of sprite
		spritePtr->setPivot(Vector2(2,2));
		//set position to the map based on 3D position
		spritePtr->setPosition(ObjectToMap(meshPtr->getPosition()));
		//set entity properties for collision detection
		meshPtr->setCollidable(true);
		meshPtr->setAlive(true);
		meshPtr->setEntityType(ENTITY_SCENERY_MESH);
		g_engine->addEntity(meshPtr);
	}
	//MID WALL
	{
		//create new Mesh and sprite for their respective scenery vectors
		sceneryBoxesBORDER.push_back(new Mesh());
		sceneryBoxesMap.push_back(new Sprite());
		Mesh* meshPtr = (Mesh*)sceneryBoxesBORDER.back();
		meshPtr->createCube(150,25,1);
		meshPtr->setPosition(100,1,-250);
	
		//create sprite pointer and load map icon
		Sprite* spritePtr = (Sprite*)sceneryBoxesMap.back();
		spritePtr->Load("HUD/Map/bushMap.png");
		//set origin to centre of sprite
		spritePtr->setPivot(Vector2(2,2));
		//set position to the map based on 3D position
		spritePtr->setPosition(ObjectToMap(meshPtr->getPosition()));
		//set entity properties for collision detection
		meshPtr->setCollidable(true);
		meshPtr->setAlive(true);
		meshPtr->setEntityType(ENTITY_SCENERY_MESH);
		g_engine->addEntity(meshPtr);
	}
	//MID WALL 2
	{
		//create new Mesh and sprite for their respective scenery vectors
		sceneryBoxesBORDER.push_back(new Mesh());
		sceneryBoxesMap.push_back(new Sprite());
		Mesh* meshPtr = (Mesh*)sceneryBoxesBORDER.back();
		meshPtr->createCube(150,25,1);
		meshPtr->setPosition(100,1,-150);
	
		//create sprite pointer and load map icon
		Sprite* spritePtr = (Sprite*)sceneryBoxesMap.back();
		spritePtr->Load("HUD/Map/bushMap.png");
		//set origin to centre of sprite
		spritePtr->setPivot(Vector2(2,2));
		//set position to the map based on 3D position
		spritePtr->setPosition(ObjectToMap(meshPtr->getPosition()));
		//set entity properties for collision detection
		meshPtr->setCollidable(true);
		meshPtr->setAlive(true);
		meshPtr->setEntityType(ENTITY_SCENERY_MESH);
		g_engine->addEntity(meshPtr);
	}
	//MID WALL 3
	{
		//create new Mesh and sprite for their respective scenery vectors
		sceneryBoxesBORDER.push_back(new Mesh());
		sceneryBoxesMap.push_back(new Sprite());
		Mesh* meshPtr = (Mesh*)sceneryBoxesBORDER.back();
		meshPtr->createCube(1,25,50);
		meshPtr->setPosition(175,1,-175);
	
		//create sprite pointer and load map icon
		Sprite* spritePtr = (Sprite*)sceneryBoxesMap.back();
		spritePtr->Load("HUD/Map/bushMap.png");
		//set origin to centre of sprite
		spritePtr->setPivot(Vector2(2,2));
		//set position to the map based on 3D position
		spritePtr->setPosition(ObjectToMap(meshPtr->getPosition()));
		//set entity properties for collision detection
		meshPtr->setCollidable(true);
		meshPtr->setAlive(true);
		meshPtr->setEntityType(ENTITY_SCENERY_MESH);
		g_engine->addEntity(meshPtr);
	}
	//BOTTOM BORDER
	{
		//create new Mesh and sprite for their respective scenery vectors
		sceneryBoxesBORDER.push_back(new Mesh());
		sceneryBoxesMap.push_back(new Sprite());
		Mesh* meshPtr = (Mesh*)sceneryBoxesBORDER.back();
		meshPtr->createCube(500,25,50);
		meshPtr->setPosition(250,1,-475);

		//create sprite pointer and load map icon
		Sprite* spritePtr = (Sprite*)sceneryBoxesMap.back();
		spritePtr->Load("HUD/Map/bushMap.png");
		//set origin to centre of sprite
		spritePtr->setPivot(Vector2(2,2));
		//set position to the map based on 3D position
		spritePtr->setPosition(ObjectToMap(meshPtr->getPosition()));
		//set entity properties for collision detection
		meshPtr->setCollidable(true);
		meshPtr->setAlive(true);
		meshPtr->setEntityType(ENTITY_SCENERY_MESH);
		g_engine->addEntity(meshPtr);
	}
	//LEFT BORDER
	{
		//create new Mesh and sprite for their respective scenery vectors
		sceneryBoxesBORDER.push_back(new Mesh());
		sceneryBoxesMap.push_back(new Sprite());
		Mesh* meshPtr = (Mesh*)sceneryBoxesBORDER.back();
		meshPtr->createCube(50,25,500);
		meshPtr->setPosition(25,1,-250);
	
		//create sprite pointer and load map icon
		Sprite* spritePtr = (Sprite*)sceneryBoxesMap.back();
		spritePtr->Load("HUD/Map/bushMap.png");
		//set origin to centre of sprite
		spritePtr->setPivot(Vector2(2,2));
		//set position to the map based on 3D position
		spritePtr->setPosition(ObjectToMap(meshPtr->getPosition()));
		//set entity properties for collision detection
		meshPtr->setCollidable(true);
		meshPtr->setAlive(true);
		meshPtr->setEntityType(ENTITY_SCENERY_MESH);
		g_engine->addEntity(meshPtr);
	}
	//RIGHT BORDER
	{
		//create new Mesh and sprite for their respective scenery vectors
		sceneryBoxesBORDER.push_back(new Mesh());
		sceneryBoxesMap.push_back(new Sprite());
		Mesh* meshPtr = (Mesh*)sceneryBoxesBORDER.back();
		meshPtr->createCube(50,25,500);
		meshPtr->setPosition(475,1,-250);
	
		//create sprite pointer and load map icon
		Sprite* spritePtr = (Sprite*)sceneryBoxesMap.back();
		spritePtr->Load("HUD/Map/bushMap.png");
		//set origin to centre of sprite
		spritePtr->setPivot(Vector2(2,2));
		//set position to the map based on 3D position
		spritePtr->setPosition(ObjectToMap(meshPtr->getPosition()));
		//set entity properties for collision detection
		meshPtr->setCollidable(true);
		meshPtr->setAlive(true);
		meshPtr->setEntityType(ENTITY_SCENERY_MESH);
		g_engine->addEntity(meshPtr);
	}

	//BEGIN: BUSH LOADING
	//create new mesh for bush
	/*bushObject = new Mesh();
	bushObject->Load("Scenery/updatedBush.x");

	//load texture from file for bush
	bushTex = new Texture();
	if(!bushTex->Load("Scenery/bushUVtex.png"))
	{
		debug << "Error loading bush tex!" << std::endl;
		return false;
	}

	//array of map positions
	Vector3 b[93] = {Vector3(405,0,-75), Vector3(405,0,-85), Vector3(145,0,-95), Vector3(405,0,-95), Vector3(145,0,-105), Vector3(405,0,-105), Vector3(145,0,-115), Vector3(405,0,-115), Vector3(375,0,-115),
					 Vector3(385,0,-115), Vector3(390,0,-115), Vector3(405,0,-115), Vector3(145,0,-125), Vector3(375,0,-125), Vector3(105,0,-135), Vector3(115,0,-135), Vector3(125,0,-135), Vector3(135,0,-135),
					Vector3(145, 0, -135), Vector3(375, 0, -135), Vector3(175, 0, -145), Vector3(225, 0, -185), Vector3(235, 0, -185), Vector3(245, 0, -185), Vector3(255, 0, -185), Vector3(265, 0, -185), Vector3(275, 0, -185),
					Vector3(285, 0, -185), Vector3(285, 0, -195), Vector3(235, 0, -205), Vector3(255, 0, -205), Vector3(285, 0, -205), Vector3(295, 0, -205), Vector3(305,0,-205), Vector3(315,0,-205), Vector3(325,0,-205),
					Vector3(235,0,-215), Vector3(255,0,-215), Vector3(235,0,-225), Vector3(255,0,-225), Vector3(265,0,-225), Vector3(275,0,-225), Vector3(285,0,-225), Vector3(305,0,-225), Vector3(195,0,-235),
					Vector3(205,0,-235), Vector3(215,0,-235), Vector3(225,0,-235), Vector3(235,0,-235), Vector3(285,0,-235), Vector3(305,0,-235), Vector3(195,0,-245), Vector3(285,0,-245), Vector3(305,0,-245),
					Vector3(195,0,-255), Vector3(285,0,-255), Vector3(305,0,-255), Vector3(195,0,-265), Vector3(285,0,-265), Vector3(305,0,-265), Vector3(195,0,-275), Vector3(215,0,-275), Vector3(225,0,-275),
					Vector3(235,0,-275), Vector3(245,0,-275), Vector3(255,0,-275), Vector3(285,0,-275), Vector3(305,0,-275), Vector3(195,0,-285), Vector3(215,0,-285), Vector3(195,0,-295), Vector3(215,0,-295),
					Vector3(215,0,-305), Vector3(175,0,-315), Vector3(185,0,-315), Vector3(195,0,-315), Vector3(205,0,-315), Vector3(215,0,-315), Vector3(175,0,-325), Vector3(175,0,-335), Vector3(175,0,-345),
					Vector3(325,0,-365), Vector3(335,0,-365), Vector3(345,0,-365), Vector3(355,0,-365), Vector3(365,0,-365), Vector3(375,0,-365), Vector3(375,0,-375), Vector3(375,0,-385), Vector3(375,0,-395),
					Vector3(385,0,-395), Vector3(395,0,-395), Vector3(405,0,-395)};	

	//assign array to vector
	bushPositions.insert(bushPositions.end(),b,b+93);

	for(int i=0; i<bushPositions.size(); i++)
	{
		sceneryBoxes.push_back(new Mesh());
		sceneryBoxesMap.push_back(new Sprite());
		Mesh* meshPtr = (Mesh*)sceneryBoxes.back();
		meshPtr->createCube(10,5,10);
		meshPtr->setPosition(bushPositions[i]);

		//create sprite pointer and load map icons
		Sprite* spritePtr = (Sprite*)sceneryBoxesMap.back();
		spritePtr->Load("HUD/Map/bushMap.png");
		//set origin to centre of sprite
		spritePtr->setPivot(Vector2(2,2));
		//set position to the map based on 3D position
		spritePtr->setPosition(ObjectToMap(meshPtr->getPosition()));
		//set entity properties for collision detection
		meshPtr->setCollidable(true);
		meshPtr->setAlive(true);
		meshPtr->setEntityType(ENTITY_SCENERY_MESH);
		g_engine->addEntity(meshPtr);
	}*/
	//END: BUSH LOADING

	//BEGIN MUSHROOM LOADING
	//create new mesh and load mushroom model
	/*mushroomObject = new Mesh();
	mushroomObject->Load("Scenery/mushroom.x");

	//load mushroom texture
	mushroomTex = new Texture();
	if(!mushroomTex->Load("Scenery/Mushroom.png"))
	{
		debug << "Error loading mushroom tex!" << std::endl;
		return false;
	}

	//array of map positions
	Vector3 m[9] = {Vector3(100,0,-100), Vector3(250,0,-100), Vector3(350,0,-100),
					Vector3(200,0,-200), Vector3(150,0,-250), Vector3(260,0,-250),
					Vector3(380,0,-250), Vector3(300,0,-350), Vector3(80,0,-420)};
	//assign array to vector
	mushroomPositions.insert(mushroomPositions.end(), m, m+9);

	for(int i=0; i<mushroomPositions.size(); i++)
	{
		sceneryBoxes.push_back(new Mesh());
		sceneryBoxesMap.push_back(new Sprite());
		Mesh* meshPtr = (Mesh*)sceneryBoxes.back();
		meshPtr->createCube(20,5,20);
		meshPtr->setPosition(mushroomPositions[i]);

		//create sprite pointer and load map icon
		Sprite* spritePtr = (Sprite*)sceneryBoxesMap.back();
		spritePtr->Load("HUD/Map/bushMap.png");
		//set origin to centre of sprite
		spritePtr->setPivot(Vector2(2,2));
		//set position to the map based on 3D position
		spritePtr->setPosition(ObjectToMap(meshPtr->getPosition()));
		//set entity properties for collision detection
		meshPtr->setCollidable(true);
		meshPtr->setAlive(true);
		meshPtr->setEntityType(ENTITY_SCENERY_MESH);
		g_engine->addEntity(meshPtr);
	}*/
	//END: MUSHROOM LOADING

	//COLLECTABLES AND WEAPON LOADING -----------------------
	//SHURIKEN Weapon Loading
	shurikenWeaponMesh = new Mesh();
	shurikenWeaponMesh->Load("Weapons/waterglass.x");
	//SHURIKEN Collectable mesh loading
	shurikenCollectMesh = new Mesh();
	if(!shurikenCollectMesh->Load("Weapons/waterglass.x"))
	{
		debug << "Error loading shuriken mesh!" << std::endl;
		return false;
	}
	//load texture from file for SHURIKEN
	shurikenTex = new Texture();
	if(!shurikenTex->Load("Weapons/shurikenTex.png"))
	{
		debug << "Error loading shuriken tex!" << std::endl;
		return false;
	}

	//DART TEST
	//DART Weapon loading
	dartWeaponMesh = new Mesh();
	dartWeaponMesh->Load("Weapons/hairdryer.x");
	//DART Collectable loading
	dartCollectMesh = new Mesh();
	dartCollectMesh->Load("Weapons/hairdryer.x");
	//load texture from file for DART
	dartTex = new Texture();
	if(!dartTex->Load("Weapons/dartTex.png"))
	{
		debug << "Error loading dart tex!" << std::endl;
		return false;
	}

	//BOMB TEST
	//BOMB  Weapon loading
	bombWeaponMesh = new Mesh();
	bombWeaponMesh->Load("Weapons/bomb.x");
	//BOMB Weapon loading
	bombCollectMesh = new Mesh();
	bombCollectMesh->Load("Collectables/bomb.x");
	//load texture from file for BOMB
	bombTex = new Texture();
	if(!bombTex->Load("Weapons/bombTex.png"))
	{
		debug << "Error loading bomb tex!" << std::endl;
		return false;
	}

	//STAR TEST
	//STAR Weapon loading
	starCollectMesh = new Mesh();
	starCollectMesh->Load("Collectables/Star.x");
	//load texture from file for STAR
	starTex = new Texture();
	if(!starTex->Load("Collectables/starTex.png"))
	{
		debug << "Error loading star tex!" << std::endl;
		return false;
	}

	//END: COLLECTABLES AND WEAPON LOADING

	//WEAPONS HUD ===============================================

	shurikenAnim = new Sprite();
	if(!shurikenAnim->Load("HUD/ShurikenThrowSheet.png"))
	{
		debug << "Error creating ShurikenThrowSheet" << std::endl;
		return false;
	}
	//set up size, position and animation settings
	shurikenAnim->setPosition(350,450);
	shurikenAnim->animationColumns=4;
	shurikenAnim->currentFrame=0;
	shurikenAnim->setSize(900,350);
	shurikenAnim->lastFrame=0;
	shurikenAnim->animationDirection=12;

	dartAnim = new Sprite();
	if(!dartAnim->Load("HUD/DartThrowSheet.png"))
	{
		debug << "Error creating DartThrowSheet" << std::endl;
		return false;
	}
	//set up size, position and animation settings
	dartAnim->setPosition(350,450);
	dartAnim->animationColumns=4;
	dartAnim->currentFrame=0;
	dartAnim->setSize(900,350);
	dartAnim->lastFrame=0;
	dartAnim->animationDirection=12;

	bombAnim = new Sprite();
	if(!bombAnim->Load("HUD/BombThrowSheet.png"))
	{
		debug << "Error creating BombThrowSheet" << std::endl;
		return false;
	}
	//set up size, position and animation settings
	bombAnim->setPosition(350,450);
	bombAnim->animationColumns=4;
	bombAnim->currentFrame=0;
	bombAnim->setSize(900,350);
	bombAnim->lastFrame=0;
	bombAnim->animationDirection=12;

	//END: WEAPONS HUD ==========================================

	//FOUNTAIN ======================================================
	//create new mesh and sprite for their respective scenery vectors
	/*fountainMesh = new Mesh();
	fountainMesh->Load("Objectives/fountain.x");
	fountainMesh->setPosition(380,0,-220);
	fountainMesh->setScale(0.01);
	fountainMesh->Transform();

	//load fountain texture
	fountainTex = new Texture();
	if(!fountainTex->Load("Objectives/fountainTex2.png"))
	{
		debug << "Error loading fountainTex2 tex!" << std::endl;
		return false;
	}*/
	//END: FOUNTAIN =================================================

	//SCREENS AND OVERLAYS ==========================================
	//create title background
	titleScreen = new Sprite();
	if(!titleScreen->Load("Screens/TitleScreenCropped.png"))
	{
		debug << "Error creating title screen Bg" << std::endl;
		return false;
	}
	//set position
	titleScreen->setPosition(0,0);

	//create win background
	winScreen = new Sprite();
	if(!winScreen->Load("Screens/WinScreenCropped.png"))
	{
		debug << "Error creating title screen Bg" << std::endl;
		return false;
	}
	//set position
	winScreen->setPosition(0,0);

	//create game over background
	gameOverScreen = new Sprite();
	if(!gameOverScreen->Load("Screens/GameOverScreenCropped.png"))
	{
		debug << "Error creating game over screen Bg" << std::endl;
		return false;
	}
	//set position
	gameOverScreen->setPosition(0,0);

	//create letterbox overlay
	letterBox = new Sprite();
	if(!letterBox->Load("Screens/LetterBox.png"))
	{
		debug << "Error creating letterbox" << std::endl;
		return false;
	}
	//set position
	letterBox->setPosition(0,0);
	//END: SCREENS AND OVERLAYS ======================================

	//set default values for mechanic stuff
	shurikens = 0;
	bombs = 0;
	darts = 0;
	pixiestars = 0;
	health = 100;

	gameState = GAME_TITLE_STATE;
	titleBGM->Play(true);

	return true;
}

//release memory
void game_end()
{
	if(letterBox) delete letterBox;
	if(gameOverScreen) delete gameOverScreen;
	if(winScreen) delete winScreen;
	if(titleScreen) delete titleScreen;
	if(fountainTex) delete fountainTex;
	if (fountainMesh) delete fountainMesh;

	if(bombAnim) delete bombAnim;
	if(dartAnim) delete dartAnim;
	if(shurikenAnim) delete shurikenAnim;
	if(starTex) delete starTex;
	if(starCollectMesh) delete starCollectMesh;
	if(bombTex) delete bombTex;
	if(bombCollectMesh) delete bombCollectMesh;
	if(bombWeaponMesh) delete bombWeaponMesh;
	if(dartTex) delete dartTex;
	if(dartCollectMesh) delete dartCollectMesh;
	if(dartWeaponMesh) delete dartWeaponMesh;
	if(shurikenTex) delete shurikenTex;
	if(shurikenCollectMesh) delete shurikenCollectMesh;
	if(shurikenWeaponMesh) delete shurikenWeaponMesh;

	if(mushroomTex) delete mushroomTex;
	if(mushroomObject) delete mushroomObject;
	if(bushTex) delete bushTex;
	if(bushObject) delete bushObject;
	if(treeObject) delete treeObject;
	for(int i=0; i<treeTextures.size(); i++) //release all treeTextures
	{
		if(treeTextures[i]) delete treeTextures[i];
	}

	for(int i=0; i<collectables.size(); i++)	//release all collectables
	{
		if(collectablesMap[i]) delete collectablesMap[i];
		Collectable* colPtr = (Collectable*)collectables[i];
		colPtr->setMap(NULL);
		if(collectables[i]) delete collectables[i];
	}

	for(int i=0; i<sceneryBoxesMap.size(); i++)
	{
		if(sceneryBoxesMap[i]) delete sceneryBoxesMap[i];
	}
	for(int i=0; i<sceneryBoxesBORDER.size(); i++)
	{
		if(sceneryBoxesBORDER[i]) delete sceneryBoxesBORDER[i];
	}
	for(int i=0; i<sceneryBoxes.size(); i++)
	{
		if(sceneryBoxes[i]) delete sceneryBoxes[i];
	}
	if(charMap) delete charMap;
	if(mapBg) delete mapBg;
	if(debugText) delete debugText;
	if(terrainTex) delete terrainTex;
	if(effect) delete effect;
	if(floorBox) delete floorBox;
	if(playerCollision) delete playerCollision;
	if(camera) delete camera;
	if(skybox) delete skybox;
	if(skyboxEffect) delete skyboxEffect;

	if(winBGM) delete winBGM;
	if(gameOverBGM) delete gameOverBGM;
	if(levelBGM) delete levelBGM;
	if(titleBGM) delete titleBGM;

	if(splash) delete splash;
	if(electric) delete electric;
	if(punch) delete punch;

	if(speech1) delete speech1;
	if(speech2) delete speech2;
	if(speech3) delete speech3;
	if(speech4) delete speech4;
	if(speech5) delete speech5;
	if(speech6) delete speech6;
	if(speech7) delete speech7;

	if(malfunction) delete malfunction;
}

void game_update(float deltaTime)
{
	//for xbox controller support
	delta = deltaTime;

	//play state update block
	if(gameState == GAME_PLAY_STATE)
	{
		//create a vector for the camera target based on the Euler angles
		camTarg = characterToCamera.ToVector();
		//take the current camera position and add target vector
		camera->setPosition(playerCollision->getPosition());
		camera->setTarget(camera->getPosition()+camTarg);

		//interpolated movement on X and Y
		//(sends goal velocity (input or lack), current character movement and time difference)
		charMovement.setX(Approach(goalVelocity.getX(),charMovement.getX(),deltaTime*100));
		charMovement.setZ(Approach(goalVelocity.getZ(),charMovement.getZ(),deltaTime*100));

		//determine forward vector from camera target direction
		//set y to 0 and get x/z vector and normalize
		vecForward = characterToCamera.ToVector();
		vecForward.y = 0;
		vecForward = Math::Normal(vecForward);
		vecBehind = vecForward*-1;

		//set up as positive on the y
		vecUp = Vector3(0,1,0);

		//calculate direction to the right of where the camera is facing using cross product
		vecRight = Math::CrossProduct(vecUp, vecForward);
		//using the current velocity vector, calculate the direction to move on x and z
		//the directions are negative due to the world positioning
		currentVelocity = vecForward * -charMovement.z + vecRight * -charMovement.x;

		//set new camera position by passing the new current velocity (* time difference)
		camera->setPosition(camera->getPosition().getX() + currentVelocity.getX() * deltaTime,
							camera->getPosition().getY() + currentVelocity.getY() * deltaTime,
							camera->getPosition().getZ() + currentVelocity.getZ() * deltaTime);

		//set camera target AFTER setting position
		camera->setTarget(camera->getPosition()+camTarg);

		//set the collision mesh to move with the camera
		playerCollision->setPosition(camera->getPosition());
		playerCollision->Update(deltaTime);

		//check if player is ok and update hud accordingly
		if(playerState==PLAYER_OK)
		{
			//set flag for if character is moving
			if(currentVelocity.x == 0 && currentVelocity.z == 0)
			{
				charMoving = false;
				ninjaFaceAnim->firstFrame=0;
				ninjaFaceAnim->lastFrame=3;
			}
			else
			{
				charMoving = true;
				ninjaFaceAnim->firstFrame=3;
				ninjaFaceAnim->lastFrame=6;
			}
		}

		//set player's icon position on map
		charMap->setPosition(ObjectToMap(playerCollision->getPosition()));

		//update ninja face animation
		ninjaFaceAnim->Update(deltaTime);

		//border updates
		for(int i=0; i<sceneryBoxesBORDER.size(); i++)
		{
			//get mesh pointer and update
			Mesh* boxPtr = (Mesh*)sceneryBoxesBORDER[i];
			boxPtr->Update(deltaTime);

			//get sprite pointer and set map position
			Sprite* sprPtr = (Sprite*)sceneryBoxesMap[i];
			sprPtr->setPosition(ObjectToMap(boxPtr->getPosition()));
		}
		//END: border updates

		//scenery map position and box updates
		for(int i=0; i<sceneryBoxes.size(); i++)
		{
			//get mesh pointer and update
			Mesh* boxPtr = (Mesh*)sceneryBoxes[i];
			boxPtr->Update(deltaTime);

			//if not a border
				//adjust collision setting based on distance from player
				if(boxPtr->getPosition().x > playerCollision->getPosition().x + 50 ||
					boxPtr->getPosition().x < playerCollision->getPosition().x - 50 ||
					boxPtr->getPosition().z > playerCollision->getPosition().z + 50 ||
					boxPtr->getPosition().z < playerCollision->getPosition().z - 50 )
				{
					boxPtr->setCollidable(false);
				}
				else
				{
					boxPtr->setCollidable(true);
				}

			//get sprite pointer and set map position
			Sprite* sprPtr = (Sprite*)sceneryBoxesMap[i];
			sprPtr->setPosition(ObjectToMap(boxPtr->getPosition()));
		}
		//END: Scenery map position

		//Collectables iterator
		for(int i=0; i<collectables.size();)
		{
			//get mesh pointer and update
			Collectable* colPtr = (Collectable*)collectables[i];
			colPtr->Update(deltaTime);
			//get sprite pointer and set map position
			Sprite* sprPtr = (Sprite*)collectablesMap[i];
			sprPtr->setPosition(ObjectToMap(colPtr->getVisibleMesh()->getPosition()));

			//if invisible mesh has collided
			if(colPtr->getInvisibleMesh()->isCollided())
			{
				//check collectable type and update values accordingly
				switch(colPtr->getType())
				{
				case COLLECT_SHURIKENS:
					shurikens+=10;
					gotShurikens=true;
					break;
				case COLLECT_DARTS:
					darts+=10;
					gotDarts=true;
					break;
				case COLLECT_BOMBS:
					//bombs+=10;
					break;
				}

				//remove collectable and map icon
				collectables.erase(collectables.begin()+i);
				collectablesMap.erase(collectablesMap.begin()+i);
			}
			else
			{
				//if no collision, update iteration counter
				i++;
			}
		}
		//END: Collectables iterator

		//ENEMIES ITERATOR ======================================
		for(int i=0; i<enemies.size();)
		{
			//set player position and update
			enemies[i]->setPlayerPos(camera->getPosition());
			enemies[i]->Update(deltaTime);
			//set pointer
			Enemy* emyPtr = (Enemy*)enemies[i];

			//JUST SHOT CHECK -----------------------------------
			//if just shot flag set (create new enemy shot)
			if(emyPtr->getJustShot())
			{
				//create new ray and get start position
				enemyShots.push_back(new Ray());
				Ray* shotPtr = (Ray*)enemyShots.back();
				Vector3 startPos = camera->getPosition();
				startPos.y = startPos.y+1;
				//calculate direction by subtracting enemy position
				//from camera position
				Vector3 hDirection =
				Math::Normal(camera->getPosition() -
					emyPtr->getVisibleMesh()->getPosition());
				//init ray with start, direction
				shotPtr->Init(startPos,hDirection,false);

				//set entity properties
				shotPtr->setCollidable(true);
				shotPtr->setAlive(true);
				shotPtr->setEntityType(ENTITY_RAY);
				shotPtr->setLifetime(500);
				g_engine->addEntity(shotPtr);

				//play FX
			}
			//END: JUST SHOT CHECK ------------------------------

			//COLLISION CHECK -----------------------------------
			if(emyPtr->getInvisibleMesh()->isCollided())
			{
				//get pointer to weapon mesh
				Mesh* wpPtr =
					(Mesh*)emyPtr->getInvisibleMesh()->getCollideBuddy();
				int dmg = 0;
				//loop through weapons
				for(int j=0; j<weapons.size(); j++)
				{
					//check if current weapon position matches the
					//position of the collided weapon
					if(weapons[j]->getInvisibleMesh()->getPosition() ==
						wpPtr->getPosition())
					{
						//set hit state
						weapons[j]->setState(WEAPON_HIT_STATE);
						//calculate relevant damage based on type
						switch(weapons[j]->getWeaponType())
						{
							case WEAPON_SHURIKEN: dmg = 20; break;
							case WEAPON_DART: dmg = 30; break;
							case WEAPON_BOMB: dmg = 40; break;
						}
					}
				}
				//send dmg value to TakeDamage method
				emyPtr->TakeDamage(dmg);
				if(emyPtr->getHealth()>0)
				{
					//play FX
				}
				else
				{
					//play FX
				}
			}
			//END: COLLISION CHECK ------------------------------

			//DEATH CHECK ---------------------------------------
			if(emyPtr->getLifeState()==ENEMY_DEAD)
			{
				//erase enemy and set entity as not alive
				emyPtr->getInvisibleMesh()->setAlive(false);
				enemies.erase(enemies.begin()+i);
				enemyIsDead = true;
				displayDialogueBG = false;
			}
			else
			{
				//otherwise increase counter for iterator loop
				i++;
			}
			//END: DEATH CHECK
		}
		//END: ENEMIES ITERATOR

		//ENEMY SHOTS ITERATOR ==================================
		for(int i=0; i<enemyShots.size();)
		{
			//update the shot
			enemyShots[i]->Update(deltaTime);

			//only need to check if collided as only
			//check is against player
			if(enemyShots[i]->isCollided() && playerState==PLAYER_OK)
			{
				//decrease health
				health-=20;
				//play hit audio
				punch->Play(false);
				//display damageHUD
				if(!damageHUDSwitch)
				{
					damageHUDSwitch = true;
					damageHUDTimer.Reset();
				}
				//if health remaining...
				if(health>0)
				{
					playerState=PLAYER_HURT;
					//reset timer and set HUD frames
					playerStateTimer.Reset();
					ninjaFaceAnim->firstFrame = 7;
					ninjaFaceAnim->lastFrame = 8;
					//play FX
				}
				else
				{
					playerState=PLAYER_DIE;
					//reset timer and set HUD frames
					playerStateTimer.Reset();
					ninjaFaceAnim->firstFrame = 9;
					ninjaFaceAnim->lastFrame = 9;
					//play FX
				}
				//set shot as dead and remove
				enemyShots[i]->setAlive(false);
				enemyShots.erase(enemyShots.begin()+i);
			}
			else
			{
				//otherwise, increment loop counter
				i++;
			}
		}
		//END: ENEMY SHOTS ITERATOR =============================

		//PLAYER STATE UPDATES ==================================
		if(playerStateTimer.StopWatch(1000))
		{
			//if hurt, set to OK
			if(playerState==PLAYER_HURT)
			{
				playerState=PLAYER_OK;
			}
			else if(playerState==PLAYER_DIE)
			{
				//if dead, set to game over
				gameState=GAME_OVER_STATE;
				//update music
				levelBGM->Stop();
				levelBGM->setPosition(0);
				gameOverBGM->Play();
			}
		}
		//END: PLAYER STATE UPDATES =============================

		if(!enemyIsDead)
			GameLogicUpdate();

		//FOUNTAIN CUTSCENE =====================================
		//update each frame (60fps)
		/*if(fountainCutscene && fountainTime.StopWatch(16))
		{
			//small increase in scale
			fountainScale+=0.005;
			fountainMesh->setScale(fountainScale);
			fountainMesh->Transform();
			//if scale has reached 0.5
			if(fountainScale>=0.5)
			{
				//unset cutscene flag, set active flag
				fountainCutscene=false;
				fountainActive=true;
				//set collidable and entity properties
				fountainMesh->setAlive(true);
				fountainMesh->setCollidable(true);
				fountainMesh->setEntityType(ENTITY_SCENERY_MESH);
				g_engine->addEntity(fountainMesh);
			}
		}*/
		//END: FOUNTAIN CUTSCENE ================================

		//FOUNTAIN COLLISION ====================================
		/*if(fountainMesh->isCollided())
		{
			gameState=GAME_WIN_STATE;
			levelBGM->Stop();
			levelBGM->setPosition(0);
			winBGM->Play(true);
		}*/
		//END: FOUNTAIN COLLISION ===============================

		//BEGIN: WEAPONS UPDATE ITERATOR ========================
		for(int i=0; i<weapons.size();)
		{
			weapons[i]->Update(deltaTime);
			//if weapon in hit state, prepare removal
			if(weapons[i]->getState()==WEAPON_HIT_STATE)
			{
				if(weapons[i]->getWeaponType()==WEAPON_BOMB)
				{
					//you could create an explosion fx here
				}
				//update entity as dead and remove weapon from vector
				weapons[i]->getInvisibleMesh()->setAlive(false);
				weapons.erase(weapons.begin()+i);
			}
			else
			{
				i++; //otherwise, update iteration counter
			}
		}
		//END: WEAPONS UPDATE ITERATOR ==========================

		//WEAPON HUD UPDATES ====================================
		shurikenAnim->Update(deltaTime);
		dartAnim->Update(deltaTime);
		bombAnim->Update(deltaTime);

		//if nearly played out final animation frame, reset sprite animation
		if(shurikenAnim->currentFrame>3.7) { shurikenAnim->lastFrame=0; }
		if(dartAnim->currentFrame>3.7) { dartAnim->lastFrame=0; }
		if(bombAnim->currentFrame>3.7) { bombAnim->lastFrame=0; }
		//END: WEAPON HUD UPDATES ===============================

		//perform camera update and floor transform
		camera->Update();
		floorBox->Transform();

	}
	//END: PLAY STATE UPDATE BLOCK
}

//method for interpolated movement
float Approach(float flGoal, float flCurrent, float dt)
{
	//work out difference; Goal velocity - current velocity
	float flDifference = flGoal - flCurrent;

	//if difference is greater than delta time
	if(flDifference > dt)
		return flCurrent + dt;
	if(flDifference < -dt)
		return flCurrent - dt;

	return flGoal;
}

//convert from 3D position to 2D position
Vector2 ObjectToMap(Vector3 obj)
{
	Vector2 newPos;

	//convert 3D X to 2D X
	//(get map position)
	//the conversion divides by 500 (3D map units) the x position
	//and multiples by 200 (map pixels) to determine position
	newPos.x = mapBg->getPosition().x+(floor(obj.x/500*200));

	//the conversion divides by 500 (3D map units) the z position
	//and multiples by 200 (map pixels) to determine position
	//it is negated to flip axis direction
	newPos.y = mapBg->getPosition().y+(-floor(obj.z/500*200));

	return newPos;
}

void game_render3d()
{
	if(gameState==GAME_PLAY_STATE){

	//if in cutscene mode
	/*if(fountainCutscene)
	{
		//set new camera position and point at fountain
		camera->setPosition(475,4,-250);
		camera->setTarget(
			Math::Normal(fountainMesh->getPosition()-camera->getPosition()));
		camera->Update();
	}*/

	//adjust lighting
	adjustLight();
	//send lighting values to shader
	effect->setParam("LightColor", D3DXVECTOR4(lightRed,lightGreen,lightBlue,1));
	effect->setParam("LightVector", lightDirection.ToD3DXVECTOR3());

	//send transforms to shader
	Matrix matWorld;
	matWorld.setIdentity();
	//set matrices for skybox getting view and projection from camera
	skyboxEffect->setWorldMatrix(matWorld);
	skyboxEffect->setViewMatrix(camera->getViewMatrix());
	skyboxEffect->setProjectionMatrix(camera->getProjMatrix());

	//render the skybox
	skyboxEffect->Begin();
	skybox->Render();
	skyboxEffect->End();

	//set matrices and texture for floorbox to render
	effect->setProjectionMatrix(camera->getProjMatrix());
	effect->setViewMatrix(camera->getViewMatrix());
	effect->setWorldMatrix(floorBox->getMatrix());
	effect->setParam("Texture",terrainTex->getTexture());

	//calculate combined inverse transpose matrix (for lighting)
	D3DXMATRIX inverse, wit;
	D3DXMatrixInverse(&inverse,0,(D3DXMATRIX*) &floorBox->getMatrix());
	D3DXMatrixTranspose(&wit, &inverse);
	effect->setParam("WorldInverseTranspose",wit);

	floorBox->Render(effect);

	//iterate through border
	for(int i=0; i<sceneryBoxesBORDER.size(); i++)
	{
		Mesh* boxPtr = (Mesh*)sceneryBoxesBORDER[i];
		effect->setProjectionMatrix(camera->getProjMatrix());
		effect->setViewMatrix(camera->getViewMatrix());
		effect->setWorldMatrix(boxPtr->getMatrix());
		boxPtr->Render(effect);
	}

	if(seeCollision)
	{
		//iterate through scenery boxes, set matrices and effect to render
		for(int i=0; i<sceneryBoxes.size(); i++)
		{
			Mesh* boxPtr = (Mesh*)sceneryBoxes[i];
			effect->setProjectionMatrix(camera->getProjMatrix());
			effect->setViewMatrix(camera->getViewMatrix());
			effect->setWorldMatrix(boxPtr->getMatrix());
			boxPtr->Render(effect);
		}
	}

	/*for(int i=0; i<mushroomPositions.size(); i++)
	{
		mushroomObject->setPosition(mushroomPositions[i]);
		mushroomObject->Transform();
		effect->setProjectionMatrix(camera->getProjMatrix());
		effect->setViewMatrix(camera->getViewMatrix());
		effect->setWorldMatrix(mushroomObject->getMatrix());
		effect->setParam("texture",mushroomTex->getTexture());

		//calculate combined inverse transpose matrix
		D3DXMATRIX inverse, wit;
		D3DXMatrixInverse(&inverse, 0, (D3DXMATRIX*) &mushroomObject->getMatrix());
		D3DXMatrixTranspose(&wit, &inverse);
		effect->setParam("WorldInverseTranspose",wit);

		mushroomObject->Render(effect);
	}*/

	/*for(int i=0; i<bushPositions.size(); i++)
	{
		bushObject->setPosition(bushPositions[i]);
		bushObject->Transform();
		effect->setProjectionMatrix(camera->getProjMatrix());
		effect->setViewMatrix(camera->getViewMatrix());
		effect->setWorldMatrix(bushObject->getMatrix());
		effect->setParam("Texture",bushTex->getTexture());

		//calculate combined inverse transpose matrix
		D3DXMATRIX inverse, wit;
		D3DXMatrixInverse(&inverse,0,(D3DXMATRIX*)&bushObject->getMatrix());
		D3DXMatrixTranspose(&wit,&inverse);
		effect->setParam("WorldInverseTranspose",wit);

		bushObject->Render(effect);
	}*/

	/*for(int i=0; i<treePositions.size(); i++)
	{
		treeObject->setPosition(treePositions[i]);
		treeObject->Transform();
		effect->setProjectionMatrix(camera->getProjMatrix());
		effect->setViewMatrix(camera->getViewMatrix());
		effect->setWorldMatrix(treeObject->getMatrix());

		int modulo = i % 4; //update texture based on remainder - pattern of four

		effect->setParam("Texture",treeTextures[modulo]->getTexture());

		//calculate combined inverse transpose matrix
		D3DXMATRIX inverse, wit;
		D3DXMatrixInverse(&inverse, 0, (D3DXMATRIX*)&treeObject->getMatrix());
		D3DXMatrixTranspose(&wit, &inverse);
		effect->setParam("WorldInverseTranspose",wit);

		treeObject->Render(effect);
	}*/

	//iterate through collectables, set matrices and effect to render
	for(int i=0; i<collectables.size(); i++)
	{
		Collectable* colPtr = (Collectable*)collectables[i];
		effect->setProjectionMatrix(camera->getProjMatrix());
		effect->setViewMatrix(camera->getViewMatrix());
		effect->setWorldMatrix(colPtr->getVisibleMesh()->getMatrix());

		//calculate combined inverse transpose matrix
		D3DXMATRIX inverse, wit;
		D3DXMatrixInverse(&inverse, 0, (D3DXMATRIX*)&colPtr->getVisibleMesh()->getMatrix());
		D3DXMatrixTranspose(&wit, &inverse);
		effect->setParam("WorldInverseTranspose",wit);

		//get visible mesh properties and set to relevant weapon mesh with texture
		switch(colPtr->getType())
		{
			case COLLECT_SHURIKENS:
				shurikenCollectMesh->setMatrix(colPtr->getVisibleMesh()->getMatrix());
				effect->setParam("Texture",shurikenTex->getTexture());
				shurikenCollectMesh->Render(effect);
			break;
			case COLLECT_DARTS:
				dartCollectMesh->setMatrix(colPtr->getVisibleMesh()->getMatrix());
				effect->setParam("Texture",dartTex->getTexture());
				dartCollectMesh->Render(effect);
			break;
			case COLLECT_BOMBS:
				bombCollectMesh->setMatrix(colPtr->getVisibleMesh()->getMatrix());
				effect->setParam("Texture",bombTex->getTexture());
				bombCollectMesh->Render(effect);
			break;
			case COLLECT_STARS:
				starCollectMesh->setMatrix(colPtr->getVisibleMesh()->getMatrix());
				effect->setParam("Texture",starTex->getTexture());
				starCollectMesh->Render(effect);
			break;
		}
		//colPtr->getVisibleMesh()->Render(effect);
	}

	//iterate through weapons, set matrices and effect to render
	for(int i=0; i<weapons.size(); i++)
	{
		Weapon* wPtr = (Weapon*)weapons[i];

		//set visible model mesh properties
		effect->setProjectionMatrix(camera->getProjMatrix());
		effect->setViewMatrix(camera->getViewMatrix());
		effect->setWorldMatrix(wPtr->getVisibleMesh()->getMatrix());

		//calculate combined inverse transpose matrix (for lighting)
		D3DXMATRIX inverse, wit;
		D3DXMatrixInverse(&inverse,0,(D3DXMATRIX*)&wPtr->getVisibleMesh()->getMatrix());
		D3DXMatrixTranspose(&wit,&inverse);
		effect->setParam("WorldInverseTranspose",wit);

		//get visible mesh properties and set to relevant weapon mesh with texture
		switch(wPtr->getWeaponType())
		{
			case WEAPON_SHURIKEN:
				shurikenWeaponMesh->setMatrix(wPtr->getVisibleMesh()->getMatrix());
				effect->setParam("Texture",shurikenTex->getTexture());
				shurikenWeaponMesh->Render(effect);
			break;
			case WEAPON_DART:
				dartWeaponMesh->setMatrix(wPtr->getVisibleMesh()->getMatrix());
				effect->setParam("Texture",dartTex->getTexture());
				dartWeaponMesh->Render(effect);
			break;
			case WEAPON_BOMB:
				bombWeaponMesh->setMatrix(wPtr->getVisibleMesh()->getMatrix());
				effect->setParam("Texture",bombTex->getTexture());
				bombWeaponMesh->Render(effect);
			break;
		}
		//if collision viewing on, show collision mesh
		if(seeCollision)
		{
			effect->setWorldMatrix(wPtr->getInvisibleMesh()->getMatrix());
			wPtr->getInvisibleMesh()->Render(effect);
		}
	}

	//FOUNTAIN ==================================================
	//set matrices and texture for FOUNTAIN to render
	/*effect->setProjectionMatrix(camera->getProjMatrix());
	effect->setViewMatrix(camera->getViewMatrix());
	effect->setWorldMatrix(fountainMesh->getMatrix());
	effect->setParam("Texture",fountainTex->getTexture());
	{
		//calculate combined inverse transpose matrix
		D3DXMATRIX inverse, wit;
		D3DXMatrixInverse(&inverse, 0, (D3DXMATRIX*) &fountainMesh->getMatrix());
		D3DXMatrixTranspose(&wit, &inverse);
		effect->setParam("WorldInverseTranspose", wit);
	}
	fountainMesh->Render(effect);*/
	//END: FOUNTAIN =============================================

	//ENEMIES ===================================================
	for(int i=0; i<enemies.size(); i++)
	{
		Enemy* emyPtr = (Enemy*)enemies[i];
		effect->setProjectionMatrix(camera->getProjMatrix());
		effect->setViewMatrix(camera->getViewMatrix());
		effect->setWorldMatrix(emyPtr->getVisibleMesh()->getMatrix());

		//calculate combined inverse tranpose matrix
		D3DXMATRIX inverse, wit;
		D3DXMatrixInverse(&inverse, 0, (D3DXMATRIX*)
			&emyPtr->getVisibleMesh()->getMatrix());
		D3DXMatrixTranspose(&wit, &inverse);
		effect->setParam("WorldInverseTranspose", wit);

		emyPtr->getVisibleMesh()->Render(effect);

		if(seeCollision)
		{
			effect->setWorldMatrix(emyPtr->getInvisibleMesh()->getMatrix());
			emyPtr->getInvisibleMesh()->Render(effect);
		}
	}
	//END: ENEMIES ==============================================

	} //END: CHECK PLAY STATE
}

//2D Rendering
void game_render2d()
{
	if(gameState==GAME_PLAY_STATE) {

	std::ostringstream debugInfo;

	/*debugInfo << "Camera Position: x " << camera->getPosition().x
		<< "| y " << camera->getPosition().y
		<< "| z " << camera->getPosition().z << std::endl;
	debugInfo << "Forward Vector: x " << vecForward.x
		<< "| y " << vecForward.y
		<< "| z " << vecForward.z << std::endl;
	debugInfo << "Right Vector: x " << vecRight.x
		<< "| y " << vecRight.y
		<< "| z " << vecRight.z << std::endl;
	debugInfo << "Current Velocity: x " << currentVelocity.x
		<< "| y " << currentVelocity.y
		<< "| z " << currentVelocity.z << std::endl;
	debugInfo << "E Angle: r " << characterToCamera.r
		<< "| p " << characterToCamera.p
		<< "| y " << characterToCamera.y << std::endl;
	debugInfo << "Player Position: x " << playerCollision->getPosition().x
		<< "| y " << playerCollision->getPosition().y
		<< "| z " << playerCollision->getPosition().z << std::endl;
	debugInfo << "Player Collided? " << playerCollision->isCollided();
	debugInfo << "Player BBOX: max x: " << playerCollision->getBoundingBox().max.x
		<< " - y: " << playerCollision->getBoundingBox().max.y
		<< " - y: " << playerCollision->getBoundingBox().max.z
		<< std::endl;
	debugInfo << "Player BBOX: min x: " << playerCollision->getBoundingBox().min.x
		<< " - y: " << playerCollision->getBoundingBox().min.y
		<< " - y: " << playerCollision->getBoundingBox().min.z
		<< std::endl;
	debugInfo << "light: r: " << lightRed
		<< " - g: " << lightGreen
		<< " - b: " << lightBlue
		<< std::endl;
	debugInfo << "light direction x: " << lightDirection.x
		<< " - y: " << lightDirection.y
		<< " - z: " << lightDirection.z
		<< std::endl;*/

	//if cutscene, render letterbox
	if(fountainCutscene)
	{
		letterBox->Render();
	}
	else
	{
		//Loads of debug info
		debugInfo << "Health: " << health
		<< std::endl;
		//print debug info
		debugText->Print(10,10,debugInfo.str());
	} //END: Cutscene check

	//render map background and character icon
	if(!hideMap)
	{
		mapBg->Render();
		charMap->Render();

		//iterate through border and render map icons
		for(int i=0; i<sceneryBoxesBORDER.size(); i++)
		{
			Sprite* sprPtr = (Sprite*)sceneryBoxesMap[i];
			sprPtr->Render();
		}

		//iterate through sceneryboxes and render map icons
		//for(int i=0; i<sceneryBoxes.size(); i++)
		//{
		//	Sprite* sprPtr = (Sprite*)sceneryBoxesMap[i];
		//	sprPtr->Render();
		//}

		//iterate through sceneryboxes and render map icons
		for(int i=0; i<collectables.size(); i++)
		{
			Sprite* sprPtr = (Sprite*)collectablesMap[i];
			sprPtr->Render();
		}
	}

	//HUD
	HUDBg->Render();
	ninjaFaceAnim->Render();


	//HUD info
	//shurikens
	debugInfo.clear();
	debugInfo.str("");
	debugInfo << shurikens;
	debugText->Print(120,865,debugInfo.str());
	//darts
	debugInfo.clear();
	debugInfo.str("");
	debugInfo << darts;
	debugText->Print(340,865,debugInfo.str());
	//bombs
	/*debugInfo.clear();
	debugInfo.str("");
	debugInfo << bombs;
	debugText->Print(525,865,debugInfo.str());
	//pixie stars
	debugInfo.clear();
	debugInfo.str("");
	debugInfo << pixiestars;
	debugText->Print(1230,830,debugInfo.str());*/

	//create colour value that matches lighting values for 3D
	//world; use this to then set colour of weapon HUD animations
	//(consistent with the world)
	Color tint = Color(lightRed, lightGreen, lightBlue, 1.0);

	//render the current selected weapon
	switch(selectedWeapon)
	{
		case SELECT_SHURIKENS:
			shurikenAnim->setColor(tint);
			shurikenAnim->Render();
		break;
		case SELECT_DARTS:
			dartAnim->setColor(tint);
			dartAnim->Render();
		break;
		case SELECT_BOMBS:
			bombAnim->setColor(tint);
			bombAnim->Render();
		break;
	}

	if(d1Stage<12)
		letterBox->Render();

	//dialogue window
	if((displayDialogueBG) && (!enemyIsDead))
		dialogueWindow->Render();

	//dialogue window text
	if(!enemyIsDead)
		RenderDialogueWindowText();

	//damageHUD render
	if(damageHUDSwitch)
		damageHUD->Render();

	}
	else if(gameState==GAME_TITLE_STATE)
	{
		titleScreen->Render();
	}
	else if(gameState==GAME_WIN_STATE)
	{
		winScreen->Render();
	}
	else if(gameState==GAME_OVER_STATE)
	{
		gameOverScreen->Render();
	}
}

//event handling
void game_event(IEvent* e)
{
	if(!lockPlayerControls)
	{//check if player controls are locked

	switch(e->getID())
	{
		//keypress event
		case EVENT_KEYPRESS:
			{
				KeyPressEvent* kpe = (KeyPressEvent*) e;
				int keypresscode = kpe->keycode;
				if(keypresscode == DIK_ESCAPE)
					g_engine->Shutdown();
				if(gameState==GAME_PLAY_STATE && !fountainCutscene)
				{
					//these set new goal velocities for movement
					if(keypresscode == DIK_W)
						goalVelocity.setZ(-50);
					if(keypresscode == DIK_S)
						goalVelocity.setZ(50);
					if(keypresscode == DIK_A)
						goalVelocity.setX(50);
					if(keypresscode == DIK_D)
						goalVelocity.setX(-50);

					break;
				}
				if(gameState==GAME_TITLE_STATE)
				{
					if(keypresscode==DIK_RETURN)
					{
						gameState=GAME_PLAY_STATE;
						titleBGM->Stop();
						titleBGM->setPosition(0);
						levelBGM->setPosition(0);
						levelBGM->Play(true);
						logicTimer.Reset();
						lockPlayerControls=true;
					}
				}
				else if(gameState==GAME_WIN_STATE ||
					gameState==GAME_OVER_STATE)
				{
					if(keypresscode==DIK_RETURN)
					{
						//InitCollectablesAndEnemies();
						gameOverBGM->Stop(); gameOverBGM->setPosition(0);
						winBGM->Stop(); winBGM->setPosition(0);
						ReInit();
						gameState=GAME_TITLE_STATE;
						titleBGM->Play(true);
					}
				}
			}
			break;
		case EVENT_KEYRELEASE:
			{
				KeyReleaseEvent* kre = (KeyReleaseEvent*) e;
				int keyreleasecode = kre->keycode;
					if(gameState == GAME_PLAY_STATE)
					{
						//these reset goal velocities for movement
						if(keyreleasecode == DIK_W)
							goalVelocity.setZ(0);
						if(keyreleasecode == DIK_S)
							goalVelocity.setZ(0);
						if(keyreleasecode == DIK_A)
							goalVelocity.setX(0);
						if(keyreleasecode == DIK_D)
							goalVelocity.setX(0);
						if(keyreleasecode == DIK_C)
						{
							if(seeCollision)
								seeCollision=false;
							else
								seeCollision=true;
						}
						if(keyreleasecode == DIK_B)
						{
							g_engine->setGlobalCollisions(false);
						}
						if(keyreleasecode == DIK_V)
						{
							g_engine->setGlobalCollisions(true);
						}
						if(keyreleasecode == DIK_1 && shurikens>0)
						{
							selectedWeapon = SELECT_SHURIKENS;
						}
						if(keyreleasecode == DIK_2 && darts>0)
						{
							selectedWeapon = SELECT_DARTS;
						}
						if(keyreleasecode == DIK_3 && bombs>0)
						{
							selectedWeapon = SELECT_BOMBS;
						}
						if(keyreleasecode == DIK_4)
						{
							selectedWeapon = SELECT_NONE;
						}
						if(keyreleasecode == DIK_SPACE)
						{
							Fire();
						}
						break;
					}
			}
			break;
		case EVENT_MOUSEMOTION:
			{
				MouseMotionEvent* mme = (MouseMotionEvent*) e;
				float movex = mme->deltax;
				float movey = mme->deltay;

				if(gameState==GAME_PLAY_STATE)
				{
					//set euler angles for camera direction
					characterToCamera.y -= movex*0.0075;
					characterToCamera.p -= movey*0.0075;

					//set restrictions on target movement
					if(characterToCamera.p >= 1.565)
						characterToCamera.p = 1.565;
					if(characterToCamera.p <= -1.565)
						characterToCamera.p = -1.565;

					characterToCamera.Normalize();
				}
			}
			break;
		case EVENT_XBUTTON:
			{
				XButtonEvent* xbe = (XButtonEvent*) e;
				xboxbutton = xbe->PadState.Gamepad.wButtons;
				leftTrigger = (float)xbe->PadState.Gamepad.bLeftTrigger;
				rightTrigger = (float)xbe->PadState.Gamepad.bRightTrigger;
				//get analogue stick values
				leftX = xbe->PadState.Gamepad.sThumbLX;
				leftY = xbe->PadState.Gamepad.sThumbLY;
				rightX = xbe->PadState.Gamepad.sThumbRX;
				rightY = xbe->PadState.Gamepad.sThumbRY;

				if(gameState==GAME_PLAY_STATE)
				{
					if(rightTrigger>0)
					{
						if(!fireSwitch)
						{
							Fire();
							fireSwitch=true;
						}
					}
					else
					{
						fireSwitch=false;
					}

					if(xboxbutton & XINPUT_GAMEPAD_DPAD_UP)
					{
					
					}
					if(xboxbutton & XINPUT_GAMEPAD_DPAD_DOWN)
					{
						selectedWeapon = SELECT_NONE;
					}
					if(xboxbutton & XINPUT_GAMEPAD_DPAD_LEFT)
					{
						selectedWeapon = SELECT_SHURIKENS;
					}
					if(xboxbutton & XINPUT_GAMEPAD_DPAD_RIGHT)
					{
						selectedWeapon = SELECT_DARTS;
					}


						/*LEFT THUMB; CHARACTER MOVEMENT */
					/***********************************************/
					float magnitude = sqrt((float)leftX*leftX + leftY*leftY);

					//determine the direction the controller is pushed
					float normalizedLX = leftX / magnitude;
					float normalizedLY = leftY / magnitude;

					//this will hold the calculated final normalised direction
					float normalizedMagnitude = 0;

					//check if the controller is outside a circular dead zone
					if(magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
					{
						//clip the magnitude at its expected maximum value
						if(magnitude > 32767) magnitude = 32767;

						//adjust magnitude relative to the end of the dead zone
						magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

						//optionally normalize the magnitude with respect to its extended range
						//giving a magnitude value of 0.0 to 1.0
						normalizedMagnitude =
							magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
					}
					else //if the controller is in the deadzone zero out the magnitude
					{
						magnitude = 0.0;
						normalizedMagnitude = 0.0;
					}

					//update character goal velocity
					goalVelocity.setX(-(normalizedLX * normalizedMagnitude * 50));
					goalVelocity.setZ(-(normalizedLY * normalizedMagnitude * 50));


						/*RIGHT THUMB; CHARACTER MOVEMENT */
					/********************************************************/

					//determine how far the controller is pushed
					magnitude = sqrt((float)rightX*rightX + rightY*rightY);

					//determine the direction the controller is pushed
					float normalizedRX = rightX / magnitude;
					float normalizedRY = rightY / magnitude;

					//this will hold the calculated final normalized direction
					normalizedMagnitude = 0;

					//check if the controller is outside a circular dead zone
					if(magnitude > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
					{
						//clip the magnitude at its expected maximum value
						if(magnitude > 32767) magnitude = 32767;

						//adjust magnitude relative to the end of the dead zone
						magnitude -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

						//optionally normalize the magnitude with respect to its extended range
						//giving a magnitude value of 0.0 to 1.0
						normalizedMagnitude =
							magnitude / (32767 - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
					}
					else
					{
						magnitude = 0.0;
						normalizedMagnitude = 0.0;
					}

					//update camera angle
					characterToCamera.p += normalizedRY * normalizedMagnitude * 0.05;
					characterToCamera.y += -(normalizedRX * normalizedMagnitude * 0.05);

					//normalized camera angle
					characterToCamera.Normalize();


				}

				if(xboxbutton & XINPUT_GAMEPAD_START)
				{
					if(gameState==GAME_TITLE_STATE)
					{
						gameState=GAME_PLAY_STATE;
						titleBGM->Stop();
						titleBGM->setPosition(0);
						levelBGM->setPosition(0);
						levelBGM->Play(true);
						logicTimer.Reset();
						lockPlayerControls=true;
					}
					else if(gameState==GAME_WIN_STATE ||
					gameState==GAME_OVER_STATE)
					{
						//InitCollectablesAndEnemies();
						gameOverBGM->Stop(); gameOverBGM->setPosition(0);
						winBGM->Stop(); winBGM->setPosition(0);
						ReInit();
						gameState=GAME_TITLE_STATE;
						titleBGM->Play(true);
					}
				}
				if(xboxbutton & XINPUT_GAMEPAD_BACK)
				{
					
				}
				if(xboxbutton & XINPUT_GAMEPAD_LEFT_THUMB)
				{
					
				}
				if(xboxbutton & XINPUT_GAMEPAD_RIGHT_THUMB)
				{
					
				}
				if(xboxbutton & XINPUT_GAMEPAD_LEFT_SHOULDER)
				{
					
				}
				if(xboxbutton & XINPUT_GAMEPAD_RIGHT_SHOULDER)
				{
					
				}
				if(xboxbutton & XINPUT_GAMEPAD_A)
				{
					
				}
				if(xboxbutton & XINPUT_GAMEPAD_B)
				{
					
				}
				if(xboxbutton & XINPUT_GAMEPAD_X)
				{
					
				}
				if(xboxbutton & XINPUT_GAMEPAD_Y)
				{
					
				}

				//vibration test
				//if(xboxbutton==256)
				//{
				//	xbe->padPointer->Vibrate(65535,0);
				//}
				//if(xboxbutton==512)
				//{
				//	xbe->padPointer->Vibrate(0,65535);
				//}
				//if(xboxbutton==768)
				//{
				//	xbe->padPointer->Vibrate(65535,65535);
				//}
				break;
			}
	}

	}//end: lockplayercontrols check
}

void adjustLight()
{
	//every half second
	if(colourTimer.StopWatch(500))
	{
		//check light state
		switch(lightState)
		{
			case LIGHT_RED:
				//adjust colour values up or down
				if(lightDown)
				{
					lightBlue-=0.005;
					lightGreen-=0.005;
				}
				else
				{
					lightBlue+=0.005;
					lightGreen+=0.005;
				}
				//if down so far, reset flag
				if(lightGreen<0.5)
				{
					lightDown=false;
				}
				//if up so far, set flat and change state
				if(lightGreen==1.0)
				{
					lightState = LIGHT_GREEN;
					lightDown=true;
				}
			break;
			case LIGHT_GREEN:
				//adjust colour values up or down
				if(lightDown)
				{
					lightBlue-=0.005;
					lightRed-=0.005;
				}
				else
				{
					lightBlue+=0.005;
					lightRed+=0.005;
				}
				//if down so far, reset flag
				if(lightRed<0.5)
				{
					lightDown=false;
				}
				//if up so far, set flat and change state
				if(lightRed==1.0)
				{
					lightState = LIGHT_BLUE;
					lightDown=true;
				}
			break;
			case LIGHT_BLUE:
				//adjust colour values up or down
				if(lightDown)
				{
					lightRed-=0.005;
					lightGreen-=0.005;
				}
				else
				{
					lightRed+=0.005;
					lightGreen+=0.005;
				}
				//if down so far, reset flag
				if(lightGreen<0.5)
				{
					lightDown=false;
				}
				//if up so far, set flat and change state
				if(lightGreen==1.0)
				{
					lightState = LIGHT_RED;
					lightDown=true;
				}
			break;
		}
	}

	//every second
	if(directionTimer.StopWatch(1000))
	{
		//increase rotation value (wrap 360)
		rotX+=0.05;
		if(rotX==360)
			rotX = 0;

		//set rotation matrix
		double x = D3DXToRadian(rotX);
		double y = 0;
		double z = 0;
		D3DXMATRIX matRotate;

		//calculate rotation matrix
		D3DXMatrixRotationYawPitchRoll(&matRotate,(float)x,(float)y,(float)z);

		//multiply light direction by rotation matrix to update direction
		D3DXVECTOR3 newLight = lightDirection.ToD3DXVECTOR3();
		D3DXVec3TransformCoord(&newLight,&newLight,&matRotate);

		//set new direction
		lightDirection = newLight;
	}
}

void Fire()
{
	//to hold weapon type
	int weaponType = 0;
	bool hasFired = false;

	//check weapon type and if ammo in supply
	if(selectedWeapon==SELECT_SHURIKENS && shurikens>0)
	{
		//update animation, ammo and weapon type
		shurikenAnim->lastFrame=3;
		shurikens--;
		weaponType = WEAPON_SHURIKEN;
		hasFired = true;
		splash->Play(false);
	}
	if(selectedWeapon==SELECT_DARTS && darts>0)
	{
		//update animation, ammo and weapon type
		dartAnim->lastFrame=3;
		darts--;
		weaponType = WEAPON_DART;
		hasFired = true;
		electric->Play(false);
	}
	if(selectedWeapon==SELECT_BOMBS && bombs>0)
	{
		//update animation, ammo and weapon type
		bombAnim->lastFrame=3;
		bombs--;
		weaponType = WEAPON_BOMB;
		hasFired = true;
	}
	//if hasFired flag set
	if(hasFired)
	{
		//add new weapon to weapons vector
		weapons.push_back(new Weapon());
		//send forward direction, weapon type, current position (spawn)
		weapons.back()->Init(Math::Normal(characterToCamera.ToVector()),
									weaponType,camera->getPosition());
		//get the invisible mesh from the new weapon and set entity properties
		weapons.back()->getInvisibleMesh()->setCollidable(true);
		weapons.back()->getInvisibleMesh()->setAlive(true);
		weapons.back()->getInvisibleMesh()->setEntityType(ENTITY_WEAPON_MESH);
		g_engine->addEntity(weapons.back()->getInvisibleMesh());
	}

	if(((shurikens<1) && (gotShurikens==true)) || ((darts<1) && (gotDarts==true)))
	{
		//if out of ammo, set to game over
		gameState=GAME_OVER_STATE;
		//update music
		levelBGM->Stop();
		levelBGM->setPosition(0);
		gameOverBGM->Play();
	}
}

void ReInit()
{
	//GLOBALS: Core object pointers
	camera->setPosition(205,5,-230);
	camera->setTarget(1,0,0);
	camera->Update();
	playerCollision->setPosition(camera->getPosition());
	ninjaFaceAnim->currentFrame=0;
	ninjaFaceAnim->lastFrame=3;
	shurikenAnim->currentFrame=0;
	shurikenAnim->lastFrame=0;
	dartAnim->currentFrame=0;
	dartAnim->lastFrame=0;
	bombAnim->currentFrame=0;
	bombAnim->lastFrame=0;

	//magic fountain
	/*fountainTime.Reset();
	fountainCutscene = false;
	fountainActive = false;
	fountainScale = 0.1;
	fountainMesh->setCollidable(false);
	fountainMesh->setAlive(false);
	fountainMesh->setScale(fountainScale);
	fountainMesh->Transform();*/

	//gamelogicupdate vars
	logicTimer.Reset();

	//GLOBALS: Vectors for common objects
	for(int i=0; i<enemyShots.size(); i++)
	{
		enemyShots[i]->setAlive(false);
		enemyShots.erase(enemyShots.begin()+i);
	}
	for(int i=0; i<weapons.size(); i++)
	{
		weapons[i]->getInvisibleMesh()->setAlive(false);
		weapons.erase(weapons.begin()+i);
	}

	//GLOBALS: Lighting
	lightDirection = Vector3(1,0,0);
	lightRed = 1.0;
	lightGreen = 1.0;
	lightBlue = 1.0;
	lightState = 1;
	rotX = 0;
	colourTimer.Reset();
	directionTimer.Reset();
	lightDown = true;

	//GLOBALS: Game mechanics values
	//set default values for mechanic stuff
	shurikens = 0;
	bombs = 0;
	darts = 0;
	pixiestars = 0;
	health = 100;
	seeCollision = false;
	selectedWeapon = 4;

	//GLOBALS: Player state mechanics
	playerState = 1;
	playerStateTimer.Reset();

	//GLOBALS: Movement
	currentVelocity = Vector3(0,0,0);
	goalVelocity = Vector3(0,0,0);
	characterToCamera.y = 0;
	characterToCamera.p = 0;
	characterToCamera.r = 0;
	charMovement = Vector3(0,0,0);
	vecForward = Vector3(0,0,0);
	vecRight = Vector3(0,0,0);
	vecUp = Vector3(0,0,0);
	vecBehind = Vector3(0,0,0);
	charMoving = false;
	Vector3 camTarg = Vector3(0,0,0);

	//GameLogicUpdate vars
	logicTimer.Reset();
	displayDialogueBG = false;
	startDialogue1 = false;
	d1Stage = 0; //dialogue text stage
	d2Stage = 0;
	d3Stage = 0; //malfunction text stage

	//sound switches
	speech1DoOnce = false;
	speech2DoOnce = false;
	speech3DoOnce = false;
	speech4DoOnce = false;
	speech5DoOnce = false;
	speech6DoOnce = false;
	speech7DoOnce = false;
	malfunctionDoOnce = false;

	lockPlayerControls = false;
	rnum = 0;//var for random number storage for d2 dialogue
	enemyIsDead = false;
	damageHUDSwitch = false;

	gotShurikens = false;
	gotDarts = false;
	hideMap = true;

	//remove dead entities
	g_engine->buryEntities();

	game_init(windowVal);
}

void InitCollectablesAndEnemies()
{
	//REMOVE EXISTING ===========================================

	for(int i=0; i<collectables.size(); i++)
	{
		if(collectablesMap[i]) delete collectablesMap[i];
		Collectable* colPtr = (Collectable*)collectables[i];
		colPtr->getInvisibleMesh()->setAlive(false);
		collectables.erase(collectables.begin()+i);
		collectablesMap.erase(collectablesMap.begin()+i);
	}

	for(int i=0; i<enemies.size(); i++)
	{
		enemies[i]->getInvisibleMesh()->setAlive(false);
		enemies.erase(enemies.begin()+i);
	}
	//END: REMOVE EXISTING ======================================

	//collectables loading =====================================

	{
		//shuriken (water bottle)
		//add new collectable to vector and cast to pointer
		collectables.push_back(new Collectable(COLLECT_SHURIKENS));
		Collectable* colPtr = (Collectable*)collectables.back();
		//set position of invisible and visible mesh
		colPtr->getInvisibleMesh()->setPosition(Vector3(130,2,-200));
		colPtr->getVisibleMesh()->setPosition(Vector3(130,2,-200));

		//see collidable / entity properties for invisible mesh
		colPtr->getInvisibleMesh()->setCollidable(true);
		colPtr->getInvisibleMesh()->setEntityType(ENTITY_COLLECTABLE_MESH);
		g_engine->addEntity(colPtr->getInvisibleMesh());
		//put map icon in map vector
		collectablesMap.push_back(colPtr->getMap());
	}

	{
		//dart (hair dryer)
		//add new collectable to vector and cast to pointer
		collectables.push_back(new Collectable(COLLECT_DARTS));
		Collectable* colPtr = (Collectable*)collectables.back();
		//set position of invisible and visible mesh
		colPtr->getInvisibleMesh()->setPosition(Vector3(100,2,-180));
		colPtr->getVisibleMesh()->setPosition(Vector3(100,2,-180));

		//see collidable / entity properties for invisible mesh
		colPtr->getInvisibleMesh()->setCollidable(true);
		colPtr->getInvisibleMesh()->setEntityType(ENTITY_COLLECTABLE_MESH);
		g_engine->addEntity(colPtr->getInvisibleMesh());
		//put map icon in map vector
		collectablesMap.push_back(colPtr->getMap());
	}

	//END: COLLECTABLES LOADING =================================

	//ENEMIES ---------------------------------------------------
	//Enemy1
	{
		std::vector<Vector3> waypoints;
		Vector3 w[4] = {Vector3(150,1,-220), Vector3(100,1,-220), Vector3(100,1,-170),
			Vector3(150,1,-170)};
		//assign array to vector
		waypoints.insert(waypoints.end(),w,w+4);
		//create enemy
		enemies.push_back(new Enemy());
		Enemy* emyPtr = (Enemy*)enemies.back();
		//set waypoints
		emyPtr->setWaypoints(waypoints);
		emyPtr->getInvisibleMesh()->setPosition(waypoints[0]);
		//set entity properties
		emyPtr->getInvisibleMesh()->setCollidable(true);
		emyPtr->getInvisibleMesh()->setAlive(true);
		emyPtr->getInvisibleMesh()->setEntityType(ENTITY_ENEMY_MESH);
		g_engine->addEntity(emyPtr->getInvisibleMesh());
	}
	//Enemy 2
	/*{
		std::vector<Vector3> waypoints;
		Vector3 w[4] = {Vector3(105,0,-165), Vector3(435,0,-165), Vector3(435,0,-425),
			Vector3(105,0,-425)};
		//assign array to vector
		waypoints.insert(waypoints.end(),w,w+4);
		//create enemy
		enemies.push_back(new Enemy());
		Enemy* emyPtr = (Enemy*)enemies.back();
		//set waypoints
		emyPtr->setWaypoints(waypoints);
		emyPtr->getInvisibleMesh()->setPosition(waypoints[0]);
		//set entity properties
		emyPtr->getInvisibleMesh()->setCollidable(true);
		emyPtr->getInvisibleMesh()->setAlive(true);
		emyPtr->getInvisibleMesh()->setEntityType(ENTITY_ENEMY_MESH);
		g_engine->addEntity(emyPtr->getInvisibleMesh());
	}
	//Enemy 3
	{
		std::vector<Vector3> waypoints;
		Vector3 w[4] = {Vector3(275,0,-335), Vector3(415,0,-335), Vector3(415,0,-405),
			Vector3(275,0,-405)};
		//assign array to vector
		waypoints.insert(waypoints.end(),w,w+4);
		//create enemy
		enemies.push_back(new Enemy());
		Enemy* emyPtr = (Enemy*)enemies.back();
		//set waypoints
		emyPtr->setWaypoints(waypoints);
		emyPtr->getInvisibleMesh()->setPosition(waypoints[0]);
		//set entity properties
		emyPtr->getInvisibleMesh()->setCollidable(true);
		emyPtr->getInvisibleMesh()->setAlive(true);
		emyPtr->getInvisibleMesh()->setEntityType(ENTITY_ENEMY_MESH);
		g_engine->addEntity(emyPtr->getInvisibleMesh());
	}
	//Enemy 4
	{
		std::vector<Vector3> waypoints;
		Vector3 w[16] = {Vector3(155,0,-355), Vector3(155,0,-305), Vector3(205,0,-305),
			Vector3(205,0,-245), Vector3(245,0,-265), Vector3(275,0,-265),
			Vector3(275,0,-235), Vector3(245,0,-235), Vector3(245,0,-195),
			Vector3(275,0,-195), Vector3(275,0,-215), Vector3(335,0,-215),
			Vector3(335,0,-295), Vector3(235,0,-295), Vector3(235,0,-355),
			Vector3(185,0,-365)};
		//assign array to vector
		waypoints.insert(waypoints.end(),w,w+16);
		//create enemy
		enemies.push_back(new Enemy());
		Enemy* emyPtr = (Enemy*)enemies.back();
		//set waypoints
		emyPtr->setWaypoints(waypoints);
		emyPtr->getInvisibleMesh()->setPosition(waypoints[0]);
		//set entity properties
		emyPtr->getInvisibleMesh()->setCollidable(true);
		emyPtr->getInvisibleMesh()->setAlive(true);
		emyPtr->getInvisibleMesh()->setEntityType(ENTITY_ENEMY_MESH);
		g_engine->addEntity(emyPtr->getInvisibleMesh());
	}*/
	//END: ENEMIES ----------------------------------------------
}

void GameLogicUpdate()
{
	//display damageHUD
	if(damageHUDSwitch)
	{
		if(damageHUDTimer.StopWatch((16*60)*1)) //run every 1 secs
		{
			damageHUDSwitch = false;
		}
	}

	//set enemy random number
	enemies[0]->SetRand(rand()%9+1);

	//update player directional vectors
	if(!enemies[0]->GetIsDead())
		enemies[0]->SetCamTarg(camTarg);

	if(enemies[0]->GetIsDead())
	{
		if(logicTimer.StopWatch((16*60)*3)) //run every 3 secs
		{
			gameState=GAME_WIN_STATE;
			levelBGM->Stop();
			levelBGM->setPosition(0);
			winBGM->Play(true);

			displayDialogueBG=false;
			d2Stage=99;
			d3Stage=99;
		}
	}

	//start d2 dialogue when enemy starts chasing player
	if(enemies[0]->GetChaseStarted())
	{
		d2Stage=1;
		displayDialogueBG=true;
	}

	//start dialogue 3 secs after game starts
	if(d1Stage==0)
	{
		if(logicTimer.StopWatch((16*60)*3)) //run every 3 secs
		{
			displayDialogueBG = true;
			startDialogue1 = true;
			d1Stage = 1;
			logicTimer.Reset();
		}
	}

	//increment dialogue stage every 5 secs
	if(displayDialogueBG)
	{
		if(startDialogue1)
		{
			if(logicTimer.StopWatch((16*60)*4)) //run every 4 secs
			{
				if(d1Stage==3)//end dialogue
				{
					//end dialogue
					startDialogue1=false;
					displayDialogueBG=false;
					enemies[0]->SetMovementOn(true);
					d1Stage = 11;
					logicTimer.Reset();
				}
				else d1Stage++;
			}
		}
	}

	//after dialogue 1, restore player controls after 2 seconds
	if(d1Stage==11)
	{
		if(logicTimer.StopWatch((16*60)*2)) //run every 2 secs
		{
			lockPlayerControls=false;
			hideMap=false;

			d1Stage = 12;
			logicTimer.Reset();
		}
	}

	//after restoring player controls, restore enemy's attack after 5 seconds
	if(d1Stage==12)
	{
		if(logicTimer.StopWatch((16*60)*5)) //run every 5 secs
		{
			enemies[0]->SetCanAttack(true);

			d1Stage = 13;
			logicTimer.Reset();
		}
	}

	//if d2 dialogue has been activated
	if((d2Stage==1) && (d3Stage==0))
	{
		if(logicTimer.StopWatch((16*60)*5)) //run every 5 secs
		{
			d2Stage = 0;
			rnum=0;
			logicTimer.Reset();
			displayDialogueBG=false;

			//reset speech audio switches
			speech4DoOnce = false;
			speech5DoOnce = false;
			speech6DoOnce = false;
			speech7DoOnce = false;
		}
	}

	//malfunction
	if(d3Stage==0)
	{
		if(enemies[0]->getLifeState()==ENEMY_DYING)
		{
			d3Stage=1;
			displayDialogueBG=true;
			logicTimer.Reset();
		}
	}
	else if(d3Stage==1)
	{
		if(logicTimer.StopWatch((16*60)*5)) //run every 5 secs
		{
			d3Stage = 2;
			logicTimer.Reset();
			displayDialogueBG=false;

			//reset speech audio switches
			malfunctionDoOnce = false;
		}
	}
	else if(d3Stage==2)
	{
		if(logicTimer.StopWatch((16*60)*10)) //run every 10 secs
		{
			d3Stage = 0;//reset
			logicTimer.Reset();
		}
	}
}

void RenderDialogueWindowText()
{
	std::ostringstream debugInfo;

	if(displayDialogueBG)
	{
		if(startDialogue1)
		{
			if(d1Stage == 1)
			{
				debugInfo.clear();
				debugInfo.str("");
				debugInfo << "You must not leave the house";
				dialogueText->Print(450,650,debugInfo.str());

				debugInfo.clear();
				debugInfo.str("");
				debugInfo << " today, master.";
				dialogueText->Print(450,700,debugInfo.str());

				if(!speech1DoOnce)
				{
					speech1->Play(false);
					speech1DoOnce=true;
				}
			}
			else if(d1Stage == 2)
			{
				debugInfo.clear();
				debugInfo.str("");
				debugInfo << "Leave everything to me.";
				dialogueText->Print(450,650,debugInfo.str());

				debugInfo.clear();
				debugInfo.str("");
				debugInfo << "You need your rest.";
				dialogueText->Print(450,700,debugInfo.str());

				if(!speech2DoOnce)
				{
					speech2->Play(false);
					speech2DoOnce=true;
				}
			}
			else if(d1Stage == 3)
			{
				debugInfo.clear();
				debugInfo.str("");
				debugInfo << "While your wife is away";
				dialogueText->Print(450,650,debugInfo.str());

				debugInfo.clear();
				debugInfo.str("");
				debugInfo << "I will take care of everything.";
				dialogueText->Print(450,700,debugInfo.str());

				if(!speech3DoOnce)
				{
					speech3->Play(false);
					speech3DoOnce=true;
				}
			}
		}

		if((d2Stage==1) && (d3Stage==0))
		{
			if(rnum==0)
			{
				rnum = rand()%10+1;
				enemies[0]->SetChaseStarted(false);
			}

			if(rnum>=8)
			{
				debugInfo.clear();
				debugInfo.str("");
				debugInfo << "Please leave this room";
				dialogueText->Print(450,650,debugInfo.str());

				debugInfo.clear();
				debugInfo.str("");
				debugInfo << "you may hurt yourself.";
				dialogueText->Print(450,700,debugInfo.str());

				if(!speech4DoOnce)
				{
					speech4->Play(false);
					speech4DoOnce=true;
				}
			}
			else if(rnum>=6)
			{
				debugInfo.clear();
				debugInfo.str("");
				debugInfo << "Don't make me call your";
				dialogueText->Print(450,650,debugInfo.str());

				debugInfo.clear();
				debugInfo.str("");
				debugInfo << "wife, sir.";
				dialogueText->Print(450,700,debugInfo.str());

				if(!speech5DoOnce)
				{
					speech5->Play(false);
					speech5DoOnce=true;
				}
			}
			else if(rnum>=4)
			{
				debugInfo.clear();
				debugInfo.str("");
				debugInfo << "I'm doing this for your";
				dialogueText->Print(450,650,debugInfo.str());

				debugInfo.clear();
				debugInfo.str("");
				debugInfo << "own good, master.";
				dialogueText->Print(450,700,debugInfo.str());

				if(!speech6DoOnce)
				{
					speech6->Play(false);
					speech6DoOnce=true;
				}
			}
			else
			{
				debugInfo.clear();
				debugInfo.str("");
				debugInfo << "Please address any complaints";
				dialogueText->Print(450,650,debugInfo.str());

				debugInfo.clear();
				debugInfo.str("");
				debugInfo << "to your wife, sir.";
				dialogueText->Print(450,700,debugInfo.str());

				if(!speech7DoOnce)
				{
					speech7->Play(false);
					speech7DoOnce=true;
				}
			}
		}

		if(d3Stage==1)
		{
			debugInfo.clear();
			debugInfo.str("");
			debugInfo << "BZZZZ... ERROR ...BZZZZ";
			dialogueText->Print(450,650,debugInfo.str());

			debugInfo.clear();
			debugInfo.str("");
			debugInfo << "UNIT MALFUNCTIONING... BZZZ";
			dialogueText->Print(450,700,debugInfo.str());

			if(!malfunctionDoOnce)
			{
				malfunction->Play(false);
				malfunctionDoOnce=true;
			}
		}
	}
}