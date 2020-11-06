#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"

enum playerAnims {
	UP, UPRIGHT, RIGHT, DOWNRIGHT, DOWN, DOWNLEFT, LEFT, UPLEFT, IDLE, DEAD, SUSPICIOUS
};

Player::Player()
{
}


Player::~Player()
{
}

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
	spritesheet.loadFromFile("images/playerSprites.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(48, 48), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(11);
	
	sprite->setAnimationSpeed(IDLE, 8);
	sprite->addKeyframe(IDLE, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(UP, 8);
	sprite->addKeyframe(UP, glm::vec2(0.f, 0.25f));

	sprite->setAnimationSpeed(UPRIGHT, 8);
	sprite->addKeyframe(UPRIGHT, glm::vec2(0.25f, 0.25f));

	sprite->setAnimationSpeed(RIGHT, 8);
	sprite->addKeyframe(RIGHT, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(DOWNRIGHT, 8);
	sprite->addKeyframe(DOWNRIGHT, glm::vec2(0.5f, 0.25f));

	sprite->setAnimationSpeed(DOWN, 8);
	sprite->addKeyframe(DOWN, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(DOWNLEFT, 8);
	sprite->addKeyframe(DOWNLEFT, glm::vec2(0.75f, 0.25f));

	sprite->setAnimationSpeed(LEFT, 8);
	sprite->addKeyframe(LEFT, glm::vec2(0.75f, 0.f));

	sprite->setAnimationSpeed(UPLEFT, 8);
	sprite->addKeyframe(UPLEFT, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(DEAD, 2);
	sprite->addKeyframe(DEAD, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(DEAD, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(DEAD, glm::vec2(0.75f, 0.5f));
	sprite->addKeyframe(DEAD, glm::vec2(0.75f, 0.f));

	sprite->setAnimationSpeed(SUSPICIOUS,2);
	sprite->addKeyframe(SUSPICIOUS, glm::vec2(0.75f, 0.f));
	sprite->addKeyframe(SUSPICIOUS, glm::vec2(0.5f,0.f));

	sprite->changeAnimation(IDLE);



	spriteDouble = Sprite::createSprite(glm::ivec2(96, 48), glm::vec2(0.5, 0.25), &spritesheet, &shaderProgram);

	spriteDouble->setNumberAnimations(1);

	spriteDouble->setAnimationSpeed(0, 8);
	spriteDouble->addKeyframe(0, glm::vec2(0.5f,0.75f));


	spriteDouble->changeAnimation(0);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	spriteDouble->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x - 24), float(tileMapDispl.y + posPlayer.y)));
	speed = 2;
	animation = false;
	tiempo = 0;
	largePlayer = false;
}

void Player::update(int deltaTime) {
	sprite->update(deltaTime);
	if (animation) {
		if (tiempo < 2000) {
			tiempo += deltaTime;
		}
		else {
			posPlayer.y -= 0.5;
			if (posPlayer.y < 9) {
				posPlayer.y += 0.5;
			}
			else {
				posPlayer.y -= 0.5;
				if (posPlayer.y < 9) {
					posPlayer.y += 0.5;
				}
				else {
					posPlayer.y -= 0.5;
					if (posPlayer.y < 9) {
						posPlayer.y += 0.5;
					}
				}
			}
			if (posPlayer.y < 10) {
				animation = false;
				sprite->changeAnimation(IDLE);
			}
		}
		
	}
	else {
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {

			posPlayer.x -= speed;
			if (posPlayer.x < 22) {
				posPlayer.x += speed;
			}
			else {
				posPlayer.x -= speed;
				if (posPlayer.x < 22) {
					posPlayer.x += speed;
				}
				else {
					posPlayer.x -= speed;
					if (posPlayer.x < 22) {
						posPlayer.x += speed;
					}
				}
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
			posPlayer.x += speed;
			if (posPlayer.x > 412) {
				posPlayer.x -= speed;
			}
			else {
				posPlayer.x += speed;
				if (posPlayer.x > 412) {
					posPlayer.x -= speed;
				}
				else {
					posPlayer.x += speed;
					if (posPlayer.x > 412) {
						posPlayer.x -= speed;
					}
				}
			}
		}

		if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
			posPlayer.y -= speed / 2;
			if (posPlayer.y < 40) {
				posPlayer.y += speed / 2;
			}
			else {
				posPlayer.y -= speed / 2;
				if (posPlayer.y < 40) {
					posPlayer.y += speed / 2;
				}
				else {
					posPlayer.y -= speed / 2;
					if (posPlayer.y < 40) {
						posPlayer.y += speed / 2;
					}
				}
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
			posPlayer.y += speed / 2;
			if (posPlayer.y > 432) {
				posPlayer.y -= speed / 2;
			}
			else {
				posPlayer.y += speed / 2;
				if (posPlayer.y > 432) {
					posPlayer.y -= speed / 2;
				}
				else {
					posPlayer.y += speed / 2;
					if (posPlayer.y > 432) {
						posPlayer.y -= speed / 2;
					}
				}
			}

		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	if (largePlayer) spriteDouble->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x - 24), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render() {
	sprite->render();
	if (largePlayer) spriteDouble->render();
}

void Player::setTileMap(TileMap *tileMap) {
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos) {
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	if(largePlayer) spriteDouble->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x - 24), float(tileMapDispl.y + posPlayer.y)));
}



void Player::stop() {
	sprite->changeAnimation(DEAD);
	speed = 0;
}

void Player::reset(const glm::vec2 &pos) {
	animation = false;
	tiempo = 0;
	speed = 2;
	posPlayer = pos;
	largePlayer = false;
	sprite->changeAnimation(IDLE);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2 Player::getPosition() {
	return posPlayer;
}

void Player::checkAnimation(const glm::vec2 &posBall) {

	if (posBall.x < posPlayer.x - 20) {
		if (posBall.y < posPlayer.y - 30) sprite->changeAnimation(UPLEFT);
		else if (posBall.y > posPlayer.y + 30) sprite->changeAnimation(DOWNLEFT);
		else sprite->changeAnimation(LEFT);
	}

	else if (posBall.x > posPlayer.x + 20) {
		if (posBall.y < posPlayer.y - 30) sprite->changeAnimation(UPRIGHT);
		else if (posBall.y > posPlayer.y + 30) sprite->changeAnimation(DOWNRIGHT);
		else sprite->changeAnimation(RIGHT);
	}

	else {
		if (posBall.y < posPlayer.y - 60) sprite->changeAnimation(UP);
		else if (posBall.y > posPlayer.y + 30) sprite->changeAnimation(DOWN);
		else sprite->changeAnimation(IDLE);
	}
}

void Player::setAnimationPlayer() {
	animation = true;
	sprite->changeAnimation(SUSPICIOUS);
}

bool Player::getLargePlayer() {
	return largePlayer;
}