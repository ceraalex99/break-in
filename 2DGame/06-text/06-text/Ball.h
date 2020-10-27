#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"

class Ball
{
public:
	Ball();
	~Ball();

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	bool collisionPlayer();

	void setTileMap(TileMap *tileMap);
	void setPlayer(Player *play);
	void setPosition(const glm::vec2 &pos);
	glm::vec2 getPosition();
	glm::vec2 getDirection();

	void setDirection(glm::vec2 dir);

	void stop();

	void reset(const glm::vec2 &pos);

	void setSticky(bool s);

private:
	glm::ivec2 tileMapDispl, posBall;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	Player *player;

	glm::vec2 direction;
	float speed;
	bool sticky;

};
#endif
