#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"



Player::Player()
{
}


Player::~Player()
{
}

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
	spritesheet.loadFromFile("images/player.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(48, 48), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	
	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	speed = 6;
}

void Player::update(int deltaTime) {
	sprite->update(deltaTime);
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {

		posPlayer.x -= 2;
		if (posPlayer.x < 22) {
			posPlayer.x += 2;
		}
		else {
			posPlayer.x -= 2;
			if (posPlayer.x < 22) {
				posPlayer.x += 2;
			}
			else {
				posPlayer.x -= 2;
				if (posPlayer.x < 22) {
					posPlayer.x += 2;
				}
			}
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
		posPlayer.x += 2;
		if (posPlayer.x > 412) {
			posPlayer.x -= 2;
		}
		else {
			posPlayer.x += 2;
			if (posPlayer.x > 412) {
				posPlayer.x -= 2;
			}
			else {
				posPlayer.x += 2;
				if (posPlayer.x > 412) {
					posPlayer.x -= 2;
				}
			}
		}
	}

	if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
		posPlayer.y -= 2;
		if (posPlayer.y < 40) {
			posPlayer.y += 2;
		}
		else {
			posPlayer.y -= 2;
			if (posPlayer.y < 40) {
				posPlayer.y += 2;
			}
			else {
				posPlayer.y -= 2;
				if (posPlayer.y < 40) {
					posPlayer.y += 2;
				}
			}
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
		posPlayer.y += 2;
		if (posPlayer.y > 432) {
			posPlayer.y -= 2;
		}
		else {
			posPlayer.y += 2;
			if (posPlayer.y > 432) {
				posPlayer.y -= 2;
			}
			else {
				posPlayer.y += 2;
				if (posPlayer.y > 432) {
					posPlayer.y -= 2;
				}
			}
		}

	}


	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Player::render() {
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap) {
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos) {
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::stop() {
	speed = 0;
}

void Player::reset(const glm::vec2 &pos) {
	speed = 6;
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2 Player::getPosition() {
	return posPlayer;
}