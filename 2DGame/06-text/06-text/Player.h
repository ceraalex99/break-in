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

private:
	glm::ivec2 tileMapDispl, posPlayer;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
};
#endif

