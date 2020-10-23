#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Ball
{
public:
	Ball();
	~Ball();

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	void setSticky(bool s);

private:
	glm::ivec2 tileMapDispl, posBall;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

	glm::vec2 direction;
	float speed;
	bool sticky;

};
#endif
