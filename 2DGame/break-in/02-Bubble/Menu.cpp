#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Menu.h"
#include "Game.h"
#include <GL/glut.h>

#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25


Menu::Menu()
{
}


Menu::~Menu()
{
}

void Menu::init()
{
	count = 0;
	initShaders();
	action = 0;
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	if (!text.init("fonts/emulogic.ttf"))
		cout << "Could not load font!" << endl;
}

void Menu::update(int deltaTime)
{
	currentTime += deltaTime;
	++count;

	if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
		++action;
		action %= 3;
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
		action += 2;
		action %= 3;
	}

	//TODO: action behaviors
}
