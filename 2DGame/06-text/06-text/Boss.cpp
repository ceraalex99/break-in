#include "Boss.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>



Boss::Boss()
{
}


Boss::~Boss()
{
}

void Boss::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
	spritesheet.loadFromFile("images/player.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(120, 120), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));
}

void Boss::update(int deltaTime) {
	currentTime += deltaTime;
	sprite->update(deltaTime);
	posBoss.x = (180*sin(currentTime/2000.f))+180;

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));

}

void Boss::render() {
	sprite->render();
}

void Boss::setPosition(const glm::vec2 &pos) {
	posBoss = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x),float(tileMapDispl.y + posBoss.y)));
}