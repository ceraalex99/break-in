#ifndef _SHOTPLAYER_INCLUDE
#define _SHOTPLAYER_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class ShotPlayer
{
public:
	ShotPlayer();
	~ShotPlayer();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap * tileMap);

	void setPosition(const glm::vec2 & pos);

	glm::vec2 getPosition();

	glm::vec2 getDirection();

	void setDirection(glm::vec2 dir);

	void setdeadShot(bool newDeadShot);

private:
	glm::ivec2 tileMapDispl, posBall;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	glm::vec2 direction;
	float speed;
	bool deadShot;
};
#endif