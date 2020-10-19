#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"



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

private:
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

};
#endif

