#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Vigilant.h"
#include "Game.h"

enum vigilantAnims {
	SUSPICIOUS
};

Vigilant::Vigilant()
{
}


Vigilant::~Vigilant()
{
}

void Vigilant::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
	spritesheet.loadFromFile("images/VigilantSprites.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(48, 48), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(SUSPICIOUS, 2);
	sprite->addKeyframe(SUSPICIOUS, glm::vec2(0.75f, 0.f));
	sprite->addKeyframe(SUSPICIOUS, glm::vec2(0.5f, 0.f));

	sprite->changeAnimation(SUSPICIOUS);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posVigilant.x), float(tileMapDispl.y + posVigilant.y)));
	speed = 1;
	activo = false;
	tiempo = 0;
}

void Vigilant::update(int deltaTime) {
	sprite->update(deltaTime);
	if (activo) {
		if (collisionPlayer()) {
			Game::instance().loseLife();
		}

		glm::ivec2 posPlayer = player->getPosition();
		if (posPlayer.x > posVigilant.x) {
			posVigilant.x += speed;
		}
		else if(posPlayer.x < posVigilant.x) {
			posVigilant.x -= speed;
		}
		if (posPlayer.y > posVigilant.y) {
			posVigilant.y += speed;
		}
		
		else if (posPlayer.y < posVigilant.y) {
			posVigilant.y -= speed;
		}

		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posVigilant.x), float(tileMapDispl.y + posVigilant.y)));

		tiempo += deltaTime;
		if (tiempo > 10000) {
			reset();
		}
	}
	
}

void Vigilant::setPosition(const glm::vec2 &pos) {
	posVigilant = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posVigilant.x), float(tileMapDispl.y + posVigilant.y)));
}

void Vigilant::render() {
	sprite->render();
}

bool Vigilant::getActivo() {
	return activo;
}

void Vigilant::setPlayer(Player *play) {
	player = play;
}

void Vigilant::setTileMap(TileMap *tileMap) {
	map = tileMap;
}

void Vigilant::alarm() {
	activo = true;
	tiempo = 0;
}

void Vigilant::reset() {
	activo = false;
	tiempo = 0;
	posVigilant = glm::ivec2(0,0);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posVigilant.x), float(tileMapDispl.y + posVigilant.y)));
}

bool Vigilant::collisionPlayer() {
	glm::ivec2 posPlayer = player->getPosition();
	int x0Vigilant = posVigilant.x;
	int x1Vigilant = posVigilant.x + 47;

	int yVigilant = posVigilant.y + 47;

	int x0Player = posPlayer.x;
	int x1Player = posPlayer.x + 47;

	int y0Player = posPlayer.y;
	int y1Player = posPlayer.y + 47;

	if ((x1Vigilant > x0Player) && (x0Vigilant < x1Player)) {
		if ((y1Player > yVigilant) && (yVigilant > y0Player)) {
			return true;
		}
	}
	else return false;
}