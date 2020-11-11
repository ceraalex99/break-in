#ifndef _PRINCESS_INCLUDE
#define _PRINCESS_INCLUDE

#include <irrKlang.h>
#include "Sprite.h"
#include "Player.h"
#include "TileMap.h"
#include "ShaderProgram.h"

class Princess
{
public:
	Princess();
	~Princess();

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setPlayer(Player *player);

	void setAnimation(bool anim);

	bool collisionPlayer();

private:
	glm::ivec2 tileMapDispl, posPrincess;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	Player *player;
	bool animation;
	int tiempo;
	ShaderProgram texProgram;
};
#endif
