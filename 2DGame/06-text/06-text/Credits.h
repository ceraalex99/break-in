#ifndef _CREDITS_INCLUDE
#define _CREDITS_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Text.h"
#include "TexturedQuad.h"

class Credits
{
public:
	Credits();
	~Credits();

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
	Texture creditsImage;
	TexturedQuad *background;
};
#endif
