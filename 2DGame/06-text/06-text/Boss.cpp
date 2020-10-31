#include "Boss.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <irrKlang.h>

enum Anim {
	FIRST_PHASE_IDLE, FIRST_PHASE_HIT, SECOND_PHASE_IDLE, SECOND_PHASE_HIT
};

Boss::Boss()
{
}


Boss::~Boss()
{
}

void Boss::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
	spritesheet.loadFromFile("images/boss.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(120, 120), glm::vec2(0.2, 0.125), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(6);

	sprite->setAnimationSpeed(FIRST_PHASE_IDLE, 6);
	sprite->addKeyframe(FIRST_PHASE_IDLE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(FIRST_PHASE_IDLE, glm::vec2(0.2f, 0.f));
	sprite->addKeyframe(FIRST_PHASE_IDLE, glm::vec2(0.4f, 0.f));


	sprite->setAnimationSpeed(FIRST_PHASE_HIT, 3);
	sprite->addKeyframe(FIRST_PHASE_HIT, glm::vec2(0.6f, 0.125f));
	sprite->addKeyframe(FIRST_PHASE_HIT, glm::vec2(0.8f, 0.125f)); 
	sprite->addKeyframe(FIRST_PHASE_HIT, glm::vec2(0.6f, 0.125f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));
	currentTime = 0;
	currentFrame = 0;
	playedIntro = false;
	hitting = false;
}

void Boss::update(int deltaTime) {
	currentTime += deltaTime;
	currentFrame++;
	sprite->update(deltaTime);
	if (movingState) {
		if (posBoss.y < finalPos.y) {
			posBoss.y += 5;
		}
		else {
			if (!playedIntro) {
				soundEngine->play2D("sounds/bossintro.mp3");
				currentTime = 0;
				playedIntro = true;
			}
			else if (currentTime > 3500.f) {
				movingState = false;
				currentTime = 0;
			}
		}
	}
	else {
		posBoss.x = (180 * sin(atan(1)*4*currentTime / 2000.f)) + 180;
	}

	if (sprite->animation() == FIRST_PHASE_HIT && currentFrame == startAnimFrame + 20) {
		sprite->changeAnimation(FIRST_PHASE_IDLE);
		hitting = false;
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));

}

void Boss::render() {
	sprite->render();
}

void Boss::setPosition(const glm::vec2 &pos) {
	posBoss = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x),float(tileMapDispl.y + posBoss.y)));
}

void Boss::move(const glm::vec2 &pos) {
	finalPos = pos;
	movingState = true;
}

void Boss::setSoundEngine(irrklang::ISoundEngine* eng) {
	soundEngine = eng;
}

glm::ivec2 Boss::getPosition() {
	return posBoss;
}

void Boss::hit() {
	if (!hitting) {
		hitting = true;
		soundEngine->play2D("sounds/bosshit.mp3");
		sprite->changeAnimation(FIRST_PHASE_HIT);
		startAnimFrame = currentFrame;
	}
}