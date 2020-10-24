#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include <irrKlang.h>
#include <iostream>


void Game::init()
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	soundEngine = irrklang::createIrrKlangDevice();
	if (!soundEngine) {
		cout << "Could not start engine" << endl;
	}
	soundEngine->setSoundVolume(0.5f);
	menu.init();
	gameState = 3;
}

bool Game::update(int deltaTime)
{
	switch (gameState) {
		case 0:
			scene.update(deltaTime);
			break;
		case 1:
			instructions.update(deltaTime);
			break;
		case 2:
			credits.update(deltaTime);
			break;
		case 3:
			menu.update(deltaTime);
			break;
	}
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (gameState) {
	case 0:
		scene.render();
		break;
	case 1:
		instructions.render();
		break;
	case 2:
		credits.render();
		break;
	case 3:
		menu.render();
		break;
	}
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		if(gameState != 3){
			startAction(3);
		}
		else {
			bPlay = false;
		}
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

void Game::startAction(int action) {
	
	switch (action) {
		case 0:
			scene.setSoundEngine(soundEngine);
			scene.init();
			break;
		case 1:
			instructions.init();
		case 2:
			credits.init();
		case 3:
			soundEngine->stopAllSounds();
			menu.init();
			break;
	}
	gameState = action;
}





