#ifndef _LEVELSELECTOR_INCLUDE
#define _LEVELSELECTOR_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Text.h"
#include "TexturedQuad.h"

class LevelSelector
{
public:
	LevelSelector();
	~LevelSelector();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	int level;
	Text text;
	Texture backgroundImage;
	TexturedQuad *background;
};
#endif