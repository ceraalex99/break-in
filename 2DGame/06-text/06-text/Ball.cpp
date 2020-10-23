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

	speed = 2;
	sticky = true;
	direction = glm::vec2(0.f, 0.f);
}

void Ball::update(int deltaTime) {
	sprite->update(deltaTime);
	posBall += direction * speed;

	if (sticky) {
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
			posBall.x -= speed;
			if (posBall.x < 8) {
				posBall.x += 2;
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
			posBall.x += speed;
			if (posBall.x > 416) {
				posBall.x -= 2;
			}
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
			direction = glm::vec2(0.f, -1.f);
			sticky = false;
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
			posBall.y += speed;
			if (posBall.y > 380) {
				posBall.y -= 2;
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

void Ball::setPosition(const glm::vec2 &pos) {
	posBall = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}

void Ball::setSticky(bool s) {
	sticky = s;
}
