#ifndef _POWERUP_INCLUDE
#define _POWERUP_INCLUDE

#include "Sprite.h"
#include "Player.h"
#include "Ball.h"
#include <irrKlang.h>



class Powerup
{
public:
	Powerup();
	~Powerup();

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	bool collisionPlayer();

	void setPlayer(Player *play);
	void setPosition(const glm::vec2 &pos);

	void setBall(Ball *pelota);

	void setSoundEngine(irrklang::ISoundEngine * eng);

private:
	glm::ivec2 tileMapDispl, posPowerup;
	Sprite *sprite;
	Texture spritesheet;
	Player *player;
	Ball *ball;
	glm::vec2 direction;
	irrklang::ISoundEngine* soundEngine;
	int animTimer;
	bool powerUpActive;
	int tiempo;
	
};
#endif
