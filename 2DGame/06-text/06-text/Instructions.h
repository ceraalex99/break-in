#ifndef _INSTRUCTIONS_INCLUDE
#define _INSTRUCTIONS_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Text.h"
#include "TexturedQuad.h"


class Instructions
{
public:
	Instructions();
	~Instructions();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	
private:
	Text text;
	glm::mat4 projection;
	float currentTime;
	Texture instructionsImage;
	ShaderProgram texProgram;
	TexturedQuad *background;
	int pag;

};
#endif

