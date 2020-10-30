#ifndef _BOSS_INCLUDE
#define _BOSS_INCLUDE

#include "Sprite.h"

class Boss
{
public:
	Boss();
	~Boss();

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void setPosition(const glm::vec2 & pos);
	void render();

private:
	glm::ivec2 tileMapDispl, posBoss;
	float currentTime;
	Texture spritesheet;
	Sprite *sprite;
};
#endif