#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include <irrKlang.h>
#include "Sprite.h"
#include "TileMap.h"
#include "ShotPlayer.h"
#include "ShaderProgram.h"




class Player
{
public:
	Player();
	~Player();

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	void stop();

	void die();

	void reset(const glm::vec2 & pos);

	glm::ivec2 getPosition();

	void setSpeed(int spd);

	void checkAnimation(const glm::vec2 &posBall);

	void setAnimationPlayer();

	bool getLargePlayer();

	void setLargePlayer(bool large);

	void setStickyPlayer(bool sticky);

	void setFirePlayer(bool fire);
	void setSoundEngine(irrklang::ISoundEngine * eng);

	void setShotCreated(bool created);

private:

	glm::ivec2 tileMapDispl, posPlayer;
	Texture spritesheet;
	Sprite *sprite, *spriteDouble, *spriteSticky, *spriteFire;
	TileMap *map;
	ShotPlayer *shot1;
	int speed;
	bool animation, largePlayer, stickyPlayer, firePlayer;
	int tiempo;
	ShaderProgram texProgram;
	irrklang::ISoundEngine* soundEngine;
	
	bool shotCreated;

};
#endif

