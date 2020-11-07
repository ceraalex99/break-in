#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <irrKlang.h>
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
	texProgram = shaderProgram;
	spritesheet.loadFromFile("images/playerSprites1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(48, 48), glm::vec2(0.20, 0.20), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(11);
	
	sprite->setAnimationSpeed(IDLE, 8);
	sprite->addKeyframe(IDLE, glm::vec2(0.20f, 0.f));

	sprite->setAnimationSpeed(UP, 8);
	sprite->addKeyframe(UP, glm::vec2(0.f, 0.2f));

	sprite->setAnimationSpeed(UPRIGHT, 8);
	sprite->addKeyframe(UPRIGHT, glm::vec2(0.2f, 0.2f));

	sprite->setAnimationSpeed(RIGHT, 8);
	sprite->addKeyframe(RIGHT, glm::vec2(0.4f, 0.f));

	sprite->setAnimationSpeed(DOWNRIGHT, 8);
	sprite->addKeyframe(DOWNRIGHT, glm::vec2(0.4f, 0.2f));

	sprite->setAnimationSpeed(DOWN, 8);
	sprite->addKeyframe(DOWN, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(DOWNLEFT, 8);
	sprite->addKeyframe(DOWNLEFT, glm::vec2(0.6f, 0.2f));

	sprite->setAnimationSpeed(LEFT, 8);
	sprite->addKeyframe(LEFT, glm::vec2(0.6f, 0.f));

	sprite->setAnimationSpeed(UPLEFT, 8);
	sprite->addKeyframe(UPLEFT, glm::vec2(0.f, 0.4f));

	sprite->setAnimationSpeed(DEAD, 2);
	sprite->addKeyframe(DEAD, glm::vec2(0.2f, 0.4f));
	sprite->addKeyframe(DEAD, glm::vec2(0.4f, 0.4f));
	sprite->addKeyframe(DEAD, glm::vec2(0.6f, 0.4f));
	sprite->addKeyframe(DEAD, glm::vec2(0.6f, 0.f));

	sprite->setAnimationSpeed(SUSPICIOUS,2);
	sprite->addKeyframe(SUSPICIOUS, glm::vec2(0.6f, 0.f));
	sprite->addKeyframe(SUSPICIOUS, glm::vec2(0.4f,0.f));

	sprite->changeAnimation(IDLE);



	spriteDouble = Sprite::createSprite(glm::ivec2(96, 48), glm::vec2(0.4, 0.2), &spritesheet, &shaderProgram);

	spriteDouble->setNumberAnimations(1);

	spriteDouble->setAnimationSpeed(0, 8);
	spriteDouble->addKeyframe(0, glm::vec2(0.4f,0.6f));


	spriteDouble->changeAnimation(0);


	spriteSticky = Sprite::createSprite(glm::ivec2(48, 48), glm::vec2(0.2, 0.2), &spritesheet, &shaderProgram);

	spriteSticky->setNumberAnimations(1);

	spriteSticky->setAnimationSpeed(0, 8);
	spriteSticky->addKeyframe(0, glm::vec2(0.f, 0.8f));



	spriteFire = Sprite::createSprite(glm::ivec2(48, 48), glm::vec2(0.2, 0.2), &spritesheet, &shaderProgram);

	spriteFire->setNumberAnimations(1);

	spriteFire->setAnimationSpeed(0, 8);
	spriteFire->addKeyframe(0, glm::vec2(0.2f, 0.8f));


	spriteDouble->changeAnimation(0);
	spriteSticky->changeAnimation(0);
	spriteFire->changeAnimation(0);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	spriteDouble->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x - 24), float(tileMapDispl.y + posPlayer.y)));
	speed = 2;
	animation = false;
	tiempo = 0;
	
	shotCreated = false;
	
	largePlayer = false;
	stickyPlayer = false;
	firePlayer = false;

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
		if (firePlayer) {
			if (Game::instance().getKey(32) && !shotCreated) {
				Game::instance().keyReleased(32);
				shot1 = new ShotPlayer();
				shot1->init(glm::ivec2(0, 0), texProgram);
				shot1->setDirection(glm::vec2(0.f, -1.f));
				shot1->setPosition(glm::ivec2(posPlayer.x + 24, posPlayer.y + 10));
				shot1->setTileMap(Game::instance().getTileMap());
				shotCreated = true;

				soundEngine->play2D("sounds/bossshot.mp3");
			}
			if (shotCreated) {
				shot1->update(deltaTime);
			}
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	if (largePlayer) spriteDouble->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x - 24), float(tileMapDispl.y + posPlayer.y)));
	if (stickyPlayer) spriteSticky->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	if (firePlayer) spriteFire->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render() {
	sprite->render();
	if (largePlayer) spriteDouble->render();
	if (stickyPlayer) spriteSticky->render();
	if (firePlayer) {
		spriteFire->render();
		if(shotCreated) shot1->render();
	}
}

void Player::setTileMap(TileMap *tileMap) {
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos) {
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	if(largePlayer) spriteDouble->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x - 24), float(tileMapDispl.y + posPlayer.y)));
	if (stickyPlayer) spriteSticky->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	if (firePlayer) spriteFire->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}



void Player::stop() {
	sprite->changeAnimation(IDLE);
	speed = 0;
}

void Player::die() {
	sprite->changeAnimation(DEAD);
	speed = 0;
}

void Player::reset(const glm::vec2 &pos) {
	animation = false;
	tiempo = 0;
	speed = 2;
	posPlayer = pos;
	largePlayer = false;
	stickyPlayer = false;
	firePlayer = false;
	sprite->changeAnimation(IDLE);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2 Player::getPosition() {
	return posPlayer;
}

void Player::setSpeed(int spd) {
	speed = spd;
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

void Player::setLargePlayer(bool large) {
	largePlayer = large;
}

void Player::setStickyPlayer(bool sticky) {
	stickyPlayer = sticky;
}

void Player::setFirePlayer(bool fire) {
	firePlayer = fire;
}

void Player::setSoundEngine(irrklang::ISoundEngine* eng) {
	soundEngine = eng;
}

void Player::setShotCreated(bool created) {
	if(shotCreated)	shot1->setdeadShot(true);
	shotCreated = created;
}