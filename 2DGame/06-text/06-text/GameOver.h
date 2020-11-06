#ifndef _GAMEOVER_INCLUDE
#define _GAMEOVER_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Text.h"
#include "TexturedQuad.h"


class GameOver
{
public:
	GameOver();
	~GameOver();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	int action;
	Text text;
	Texture backgroundImage;
	TexturedQuad *background;
};
#endif
