#ifndef _YOUWON_INCLUDE
#define _YOUWON_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Text.h"
#include "TexturedQuad.h"

class YouWon
{
public:
	YouWon();
	~YouWon();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	Text text;
	Texture backgroundImage;
	TexturedQuad *background;
};
#endif