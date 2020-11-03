#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Quad.h"
#include "TexturedQuad.h"
#include "Text.h"
#include "Player.h"
#include "Ball.h"
#include <irrKlang.h>
#include <sstream>
#include "Boss.h"


#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	void setSoundEngine(irrklang::ISoundEngine* eng);
	void catchKey();

	void nextRoom();
	void previousRoom();

	void reloadMoney();

	void reloadPoints();

	void reloadLives();

	void reloadRoom();

	void reloadBank();

	void loseLife();
	void resetPlayer();
	int getCurrentRoom();
	void startBossFight();
	int getState();
	void win();
	
	void startAnim();
private:
	void initShaders();

private:
	Texture arrowsTexture;
	TexturedQuad *arrows;
	ShaderProgram simpleProgram, texProgram;
	float currentTime;
	glm::mat4 projection;
	Text text;
	Player *player;
	TileMap *map[5];
	bool haveKey[5];
	Ball *ball;
	Texture meshTexture, lettersTexture, endLevelTexture;
	TexturedQuad *mesh, *letters, *endLevel;
	int currentRoom;
	stringstream pointsStr, moneyStr, livesStr, bankStr, roomStr;
	Boss *boss;

	irrklang::ISoundEngine* soundEngine;
	float loseTime, startBossTime;
	int lives;
	Quad *blackBackground;
	
	Quad *bossLifeBar;
	int currentBank;
};


#endif // _SCENE_INCLUDE

