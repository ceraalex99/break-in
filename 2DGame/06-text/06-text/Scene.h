#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Quad.h"
#include "TexturedQuad.h"
#include "Text.h"
#include "Player.h"
#include "Princess.h"
#include "Ball.h"
#include "Vigilant.h"
#include <irrKlang.h>
#include <sstream>
#include "Boss.h"
#include "Powerup.h"

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
	void endBossFight();
	int getState();
	void win();

	Player * getPlayer();

	TileMap * getTileMap();

	void stopBossShooting();

	void alarmOn();

	void alarmOff();

	void catchPowerup(int color);


	void stopPowerUps();

	void tripleBall();

	void loseBall();

	int getNumberBalls();

	void setTelefono();

	void countPoints();

	void setShotCreated(bool created);

	void gainMoney();
	
	void startAnimFinalBank();
	void startAnimFinalGame();
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
	Princess *princess;
	TileMap *map[6];
	bool haveKey[6];
	Ball *ball, *ball2, *ball3;
	Vigilant *vigilant;
	Texture meshTexture, lettersTexture, endLevelTexture, pointsTexture;
	TexturedQuad *mesh, *letters, *endLevel, *points;
	int currentRoom;
	stringstream pointsStr, moneyStr, livesStr, bankStr, roomStr;
	Boss *boss;

	irrklang::ISoundEngine* soundEngine;
	float loseTime, startBossTime;
	int lives;
	Quad *blackBackground;
	
	Quad *bossLifeBar;
	int currentBank;
	int powerupTimer;
	Powerup *powerup;
	bool powerupIsActive, powerActive;
	bool bonusActive;
	bool dead, bossDefeated;

	int balls;
	bool startTripleBall;

	bool telefono;
	int currentMoney;
	Texture largeTex, smallTex, batgunTex, stickyTex;
	TexturedQuad *powerupQuad;
};


#endif // _SCENE_INCLUDE

