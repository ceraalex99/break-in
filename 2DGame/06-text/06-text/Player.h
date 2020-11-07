#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "Sprite.h"
#include "TileMap.h"




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

private:

	glm::ivec2 tileMapDispl, posPlayer;
	Texture spritesheet;
	Sprite *sprite, *spriteDouble, *spriteSticky, *spriteFire;
	TileMap *map;
	int speed;
	bool animation, largePlayer, stickyPlayer, firePlayer;
	int tiempo;
	

};
#endif

