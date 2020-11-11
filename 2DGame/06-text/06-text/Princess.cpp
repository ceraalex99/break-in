#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <irrKlang.h>
#include "Princess.h"
#include "Game.h"


Princess::Princess()
{
}


Princess::~Princess()
{
}

void Princess::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	texProgram = shaderProgram;
	spritesheet.loadFromFile("images/princessSprites.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(48, 48), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPrincess.x), float(tileMapDispl.y + posPrincess.y)));
	animation = false;
	tiempo = 0;
}


void Princess::update(int deltaTime) {
	tiempo += deltaTime;
	if (animation) {
		sprite->update(deltaTime);
		if (tiempo > 2000) {
			posPrincess.y += 2;
			if (posPrincess.y > 410) animation = false;

			if (collisionPlayer()) {
				Game::instance().startAction(6);
			}
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPrincess.x), float(tileMapDispl.y + posPrincess.y)));
}



void Princess::render()
{
	sprite->render();
}

void Princess::setTileMap(TileMap * tileMap)
{
	map = tileMap;
}

void Princess::setPosition(const glm::vec2 & pos)
{
	posPrincess = pos;
}

void Princess::setPlayer(Player *play) {
	player = play;
}

void Princess::setAnimation(bool anim) {
	animation = anim;
}

bool Princess::collisionPlayer() {

	glm::ivec2 posPlayer = player->getPosition();
	
	int yPrincess = posPrincess.y + 47;
	int yPlayer = posPlayer.y;

	for (int i = 0; i < 20; ++i) {
		if (yPrincess + i == yPlayer) return true;

	}
	return false;
}