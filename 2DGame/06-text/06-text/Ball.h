#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"
#include "Boss.h"

class Ball
{
public:
	Ball();
	~Ball();

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void collisionPlayer();

	bool collisionBoss();

	void setBoss(Boss *b);

	bool getSticky();

	void setTileMap(TileMap *tileMap);
	void setPlayer(Player *play);
	void setPosition(const glm::vec2 &pos);
	glm::vec2 getPosition();
	glm::vec2 getDirection();

	void setDirection(glm::vec2 dir);

	void stop();

	void reset(const glm::vec2 &pos);

	void setSticky(bool s);

	void setAnimationBall(bool anim);

	void setPowerUpSticky(bool powerUpSticky);

	void setSpeed(int spd);

	void setDeadBall(bool dead);

private:
	glm::ivec2 tileMapDispl, posBall;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	Player *player;
	Boss *boss;
	glm::vec2 direction;
	float speed;
	bool sticky;
	bool animation;
	bool stopped;
	int tiempo;
	bool powerUpSticky;
	bool deadBall;
};
#endif
