#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Text.h"
#include "TexturedQuad.h"



class Menu
{
public:
	Menu();
	~Menu();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	bool play();

private:
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	int action;
	Text text;
	Texture imgFondo;
	TexturedQuad *fondo;
	int count;

};
#endif

