#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Ball.h"
#include "Game.h"



Ball::Ball()
{
}


Ball::~Ball()
{
}

void Ball::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
	spritesheet.loadFromFile("images/ball.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(24, 24), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));

	speed = 6;
	sticky = true;
	direction = glm::vec2(0.f, 0.f);
}

void Ball::update(int deltaTime) {
	sprite->update(deltaTime);
	posBall += direction * speed;

	if (sticky) {
		speed = 6;
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
			posBall.x -= speed;
			if (posBall.x < 34) {
				posBall.x += 6;
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
			posBall.x += speed;
			if (posBall.x > 428) {
				posBall.x -= 6;
			}
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
			direction = glm::vec2(1.f, -1.f);
			sticky = false;
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
			posBall.y += 2;
			if (posBall.y > 406) {
				posBall.y -= 2;
			}
			else {
				posBall.y += 2;
				if (posBall.y > 406) {
					posBall.y -= 2;
				}
				else {
					posBall.y += 2;
					if (posBall.y > 406) {
						posBall.y -= 2;
					}
				}
			}
		}
	}
	else {
		speed = 4;
		
		if (posBall.y > 450) {
			Game::instance().loseLife();
			speed = 0;
		}
		else {
			if ((direction.y < 0 && map->collisionMoveUp(posBall, glm::ivec2(24, 24))) || (direction.y > 0 && map->collisionMoveDown(posBall, glm::ivec2(24, 24), &posBall.y))) {
				direction.y = -direction.y;
				posBall.y += direction.y * speed;
			}
			else if ((direction.x > 0 && map->collisionMoveRight(posBall, glm::ivec2(24, 24))) || (direction.x < 0 && map->collisionMoveLeft(posBall, glm::ivec2(24, 24)))) {
				direction.x = -direction.x;
				posBall.x += direction.x * speed;
			}
			else if (direction.y > 0 && collisionPlayer()) {
				posBall.y -= direction.y * speed;
				direction.y = -direction.y;
			}
		}
		
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));

}

void Ball::render() {
	sprite->render();
}

void Ball::setTileMap(TileMap *tileMap) {
	map = tileMap;
}

void Ball::setPlayer(Player *play) {
	player = play;
}

void Ball::setPosition(const glm::vec2 &pos) {
	posBall = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}

void Ball::setSticky(bool s) {
	sticky = s;
}

glm::vec2 Ball::getPosition() {
	return posBall;
}

glm::vec2 Ball::getDirection(){
	return direction;
}

void Ball::setDirection(glm::vec2 dir) {
	direction = dir;
}

void Ball::stop() {
	speed = 0;
}

void Ball::reset(const glm::vec2 &pos) {
	sticky = true;
	speed = 4;
	direction = glm::vec2(0.f, 0.f);
	posBall = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}

bool Ball::collisionPlayer() {
	glm::ivec2 posPlayer = player->getPosition();
	int x0Ball = posBall.x;
	int x1Ball = posBall.x + 23;

	int yBall = posBall.y + 24;

	int x0Player = posPlayer.x;
	int x1Player = posPlayer.x + 47;

	int y0Player = posPlayer.y;
	int y1Player = posPlayer.y + 14;

	if ((x1Ball > x0Player) && (x0Ball < x1Player)) {
		if ((y1Player > yBall) && (yBall > y0Player)) {
			direction.x = ((posBall.x + 12.f) - (posPlayer.x + 24.f)) / 24.f;
			return true;
		}
	}
	return false;
}