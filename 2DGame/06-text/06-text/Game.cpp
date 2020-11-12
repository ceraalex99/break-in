#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include <irrKlang.h>
#include <iostream>


void Game::init()
{
	bPlay = true;
	godMode = false;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	soundEngine = irrklang::createIrrKlangDevice();
	if (!soundEngine) {
		cout << "Could not start engine" << endl;
	}
	soundEngine->setSoundVolume(0.5f);
	soundEngine->play2D("sounds/menu.ogg", true);
	menu.init();
	gameState = 3;
}

bool Game::update(int deltaTime)
{
	switch (gameState) {
		case 0:
			levelSelector.update(deltaTime);
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
		case 4:
			scene.update(deltaTime);
			break;
		case 5:
			gameOver.update(deltaTime);
			break;
		case 6:
			youWon.update(deltaTime);
			break;
	}
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (gameState) {
	case 0:
		levelSelector.render();
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
	case 4:
		scene.render();
		break;
	case 5:
		gameOver.render();
		break;
	case 6:
		youWon.render();
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
	else if (key == 'g') {
		godMode = !godMode;
	}
	
	keys[key] = true;

	if (key == 13) {
		if (gameState == 5 || gameState == 6) {
			keyReleased(13);
			startAction(3);
		}
	}
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
			points = 0;
			money = 0;
			levelSelector.init();
			break;
		case 1:
			instructions.init();
			break;
		case 2:
			credits.init();
			break;
		case 3:
			soundEngine->stopAllSounds();
			soundEngine->setSoundVolume(0.5f);
			soundEngine->play2D("sounds/menu.ogg", true);
			menu.init();
			break;
		case 5:
			soundEngine->stopAllSounds();
			soundEngine->setSoundVolume(0.2f);
			soundEngine->play2D("sounds/gameOver.mp3", true);
			gameOver.init();
			break;
		case 6:
			soundEngine->stopAllSounds();
			soundEngine->play2D("sounds/youWon.mp3", true);
			soundEngine->setSoundVolume(0.2f);
			youWon.init();
			break;
	}
	gameState = action;
}

void Game::selectLevel(int level) {
	currentBank = level+1;
	scene.setSoundEngine(soundEngine);
	gameState = 4;
	scene.init();
}

void Game::nextLevel() {
	currentBank += 1;
	scene.setSoundEngine(soundEngine);
	gameState = 4;
	scene.init();
}


void Game::catchKey(){
	scene.catchKey();
}

void Game::breakBrick() {
	gainPoints(100);
	soundEngine->play2D("sounds/brick.wav");
}

void Game::gainPoints(int p) {
	points += p;
	scene.reloadPoints();
}

int Game::getPoints() {
	return points;
}

void Game::setPoints(int newPoints) {
	points = newPoints;
}

int Game::getMoney() {
	return money;
}

void Game::setMoney(int newMoney) {
	money = newMoney;
}

void Game::loseLife() {
	scene.loseLife();
}

int Game::getCurrentBank() {
	return currentBank;
}

bool Game::getGodMode() {
	return godMode;
}

void Game::nextRoom() {
	scene.nextRoom();
}

int Game::getCurrentRoom() {
	return scene.getCurrentRoom();
}

void Game::previousRoom() {
	scene.previousRoom();
}

void Game::gainMoney(int mon) {
	money += mon;
	scene.gainMoney();
	scene.reloadMoney();
	soundEngine->play2D("sounds/money.wav");
}

int Game::getSceneState() {
	return scene.getState();
}

void Game::win() {
	scene.win();
}

void Game::alarmOn() {

	scene.alarmOn();
}

void Game::alarmOff() {

	scene.alarmOff();
}

Player* Game::getPlayer() {
	return scene.getPlayer();
}

TileMap* Game::getTileMap() {
	return scene.getTileMap();
}

void Game::stopBossShooting() {
	scene.stopBossShooting();
}

void Game::catchPowerup(int color) {
	gainPoints(200);
	scene.catchPowerup(color);
}

void Game::tripleBall() {
	scene.tripleBall();
}

void Game::loseBall() {
	scene.loseBall();
}

int Game::getNumberBalls() {
	return scene.getNumberBalls();
}

void Game::setTelefono() {
	scene.setTelefono();
}

void Game::setShotCreated(bool created) {
	scene.setShotCreated(created);
}