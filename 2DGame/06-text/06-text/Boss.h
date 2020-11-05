#ifndef _BOSS_INCLUDE
#define _BOSS_INCLUDE

#include "Sprite.h"
#include <irrKlang.h>
#include "Shot.h"
#include "ShaderProgram.h"

class Boss
{
public:
	Boss();
	~Boss();

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void setPosition(const glm::vec2 & pos);
	void move(const glm::vec2 & pos);
	void setSoundEngine(irrklang::ISoundEngine * eng);
	glm::ivec2 getPosition();
	void hit();
	int getLife();
	bool getHitting();
	void die();
	void shoot();
	void stopShooting();
	void render();

private:
	glm::ivec2 tileMapDispl, posBoss;
	int currentTime;
	Texture spritesheet;
	Sprite *sprite;
	bool movingState;
	glm::ivec2 finalPos;
	irrklang::ISoundEngine* soundEngine;
	bool playedIntro;
	int currentFrame, startAnimFrame, startAnimTime;
	bool hitting;
	int lives;
	int shotTimer;
	ShaderProgram texProgram;
	bool shooting;
	bool secPhase;

	Shot *shot1, *shot2, *shot3;
};
#endif