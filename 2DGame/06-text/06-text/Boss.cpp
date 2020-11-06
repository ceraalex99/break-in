#include "Boss.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <irrKlang.h>
#include "Game.h"

enum Anim {
	FIRST_PHASE_IDLE, FIRST_PHASE_HIT, FIRST_TO_SECOND, SECOND_PHASE_IDLE, SECOND_PHASE_HIT, DYING, FIRST_PHASE_SHOT, SECOND_PHASE_SHOT
};

Boss::Boss()
{
}


Boss::~Boss()
{
}

void Boss::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
	texProgram = shaderProgram;
	spritesheet.loadFromFile("images/boss.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(120, 120), glm::vec2(0.2, 0.125), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(8);

	sprite->setAnimationSpeed(FIRST_PHASE_IDLE, 6);
	sprite->addKeyframe(FIRST_PHASE_IDLE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(FIRST_PHASE_IDLE, glm::vec2(0.2f, 0.f));
	sprite->addKeyframe(FIRST_PHASE_IDLE, glm::vec2(0.4f, 0.f));

	sprite->setAnimationSpeed(FIRST_PHASE_HIT, 6);
	sprite->addKeyframe(FIRST_PHASE_HIT, glm::vec2(0.6f, 0.f));
	sprite->addKeyframe(FIRST_PHASE_HIT, glm::vec2(0.8f, 0.f));

	sprite->setAnimationSpeed(FIRST_TO_SECOND, 9);
	sprite->addKeyframe(FIRST_TO_SECOND, glm::vec2(0.0f, 0.375f));
	sprite->addKeyframe(FIRST_TO_SECOND, glm::vec2(0.2f, 0.375f));
	sprite->addKeyframe(FIRST_TO_SECOND, glm::vec2(0.4f, 0.375f));
	sprite->addKeyframe(FIRST_TO_SECOND, glm::vec2(0.6f, 0.375f));
	sprite->addKeyframe(FIRST_TO_SECOND, glm::vec2(0.8f, 0.375f));
	sprite->addKeyframe(FIRST_TO_SECOND, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(FIRST_TO_SECOND, glm::vec2(0.2f, 0.5f));
	sprite->addKeyframe(FIRST_TO_SECOND, glm::vec2(0.4f, 0.5f));
	sprite->addKeyframe(FIRST_TO_SECOND, glm::vec2(0.8f, 0.375f));
	sprite->addKeyframe(FIRST_TO_SECOND, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(FIRST_TO_SECOND, glm::vec2(0.2f, 0.5f));
	sprite->addKeyframe(FIRST_TO_SECOND, glm::vec2(0.4f, 0.5f));
	sprite->addKeyframe(FIRST_TO_SECOND, glm::vec2(0.8f, 0.375f));
	sprite->addKeyframe(FIRST_TO_SECOND, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(FIRST_TO_SECOND, glm::vec2(0.2f, 0.5f));
	sprite->addKeyframe(FIRST_TO_SECOND, glm::vec2(0.4f, 0.5f));
	sprite->addKeyframe(FIRST_TO_SECOND, glm::vec2(0.6f, 0.5f));
	sprite->addKeyframe(FIRST_TO_SECOND, glm::vec2(0.f, 0.125f));


	sprite->setAnimationSpeed(SECOND_PHASE_IDLE, 6);
	sprite->addKeyframe(SECOND_PHASE_IDLE, glm::vec2(0.f, 0.125f));
	sprite->addKeyframe(SECOND_PHASE_IDLE, glm::vec2(0.2f, 0.125f));

	sprite->setAnimationSpeed(SECOND_PHASE_HIT, 6);
	sprite->addKeyframe(SECOND_PHASE_HIT, glm::vec2(0.6f, 0.125f));
	sprite->addKeyframe(SECOND_PHASE_HIT, glm::vec2(0.8f, 0.125f));

	sprite->setAnimationSpeed(DYING, 6);
	sprite->addKeyframe(DYING, glm::vec2(0.f, 0.625f));
	sprite->addKeyframe(DYING, glm::vec2(0.2f, 0.625f));

	sprite->setAnimationSpeed(FIRST_PHASE_SHOT, 6);
	sprite->addKeyframe(FIRST_PHASE_SHOT, glm::vec2(0.f, 0.375f));
	

	sprite->setAnimationSpeed(SECOND_PHASE_SHOT, 6);
	sprite->addKeyframe(SECOND_PHASE_SHOT, glm::vec2(0.6f, 0.125f));
	sprite->addKeyframe(SECOND_PHASE_SHOT, glm::vec2(0.8f, 0.125f));
	sprite->addKeyframe(SECOND_PHASE_SHOT, glm::vec2(0.f, 0.25f));
	sprite->addKeyframe(SECOND_PHASE_SHOT, glm::vec2(0.2f, 0.25f));
	sprite->addKeyframe(SECOND_PHASE_SHOT, glm::vec2(0.4f, 0.25f));
	sprite->addKeyframe(SECOND_PHASE_SHOT, glm::vec2(0.6f, 0.25f));



	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));
	currentTime = 0;
	currentFrame = 0;
	startAnimTime = 0;
	playedIntro = false;
	hitting = false;
	lives = 20;
	shotTimer = 0;
	shooting = false;
	secPhase = false;
	time = 0;

	text.init("fonts/ARCADEPI.ttf");
}

void Boss::update(int deltaTime) {
	time += deltaTime;
	currentTime += deltaTime;
	shotTimer += deltaTime;
	currentFrame++;
	sprite->update(deltaTime);

	if (!secPhase && shooting) {
		shot1->update(deltaTime);
	}
	else if (secPhase && shooting) {
		shot1->update(deltaTime);
		shot2->update(deltaTime);
		shot3->update(deltaTime);
	}

	if (movingState) {
		if (sprite->animation() == FIRST_TO_SECOND) {
			
			if (posBoss.x > 180) {
				posBoss.x -= 2;
			}
			else if(posBoss.x < 180){
				posBoss.x += 2;
			}
			else {
				movingState = false;
			}
		}
		else {
			if (posBoss.y < finalPos.y) {
				posBoss.y += 5;
			}
			else {
				if (!playedIntro) {
					soundEngine->play2D("sounds/bossSong.mp3", true);
					currentTime = 0;
					playedIntro = true;
				}
				else if (currentTime > 3500.f) {
					movingState = false;
					currentTime = 0;
				}
			}
		}
	}
	else {
		if (sprite->animation() != FIRST_TO_SECOND && sprite->animation() != DYING) {
			posBoss.x = (180 * sin(atan(1) * 4 * currentTime / 2000.f)) + 180;
			if (shotTimer >= 3000){
				shotTimer = 0;
				if ((sprite->animation() == FIRST_PHASE_IDLE || sprite->animation() == SECOND_PHASE_IDLE)) {
					shoot();
				}
			}
		}
	}

	if (sprite->animation() == FIRST_PHASE_HIT && currentFrame == startAnimFrame + 35) {
		if (lives == 10) {
			soundEngine->play2D("sounds/phaseChange.mp3");
			sprite->changeAnimation(FIRST_TO_SECOND);
			startAnimTime = currentTime;
			movingState = true;
			shooting = false;
			secPhase = true;
			if (posBoss.x % 2 == 1)
				posBoss.x--;
		}
		else {
			sprite->changeAnimation(FIRST_PHASE_IDLE);
			hitting = false;
		}
	}
	else if (sprite->animation() == FIRST_TO_SECOND && currentTime > startAnimTime + 1800) {
		sprite->changeAnimation(SECOND_PHASE_IDLE);
		hitting = false;
		currentTime = 0;
	}

	else if (sprite->animation() == SECOND_PHASE_HIT && currentFrame == startAnimFrame + 30) {
		sprite->changeAnimation(SECOND_PHASE_IDLE);
		hitting = false;
	}
	else if (sprite->animation() == DYING && currentFrame == startAnimFrame + 100) {
		die();
	}

	if (shotTimer > 200) {
		if (sprite->animation() == FIRST_PHASE_SHOT) {
			sprite->changeAnimation(FIRST_PHASE_IDLE);
		}
		else if (sprite->animation() == SECOND_PHASE_SHOT) {
			sprite->changeAnimation(SECOND_PHASE_IDLE);
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));
}

void Boss::render() {
	sprite->render();
	if (shooting && !secPhase) {
		shot1->render();
	}
	else if(shooting && secPhase){
		shot1->render();
		shot2->render();
		shot3->render();
	}

	if (time <= 2800) {
		text.render("THE", glm::vec2(173, 215), 72, glm::vec4(1, 1, 1, 1));
		text.render("KIDNAPPER", glm::vec2(17, 290), 64, glm::vec4(1, 1, 1, 1));
	}
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
		lives--;
		if (lives == 0) {
			sprite->changeAnimation(DYING);
			startAnimFrame = currentFrame;
			soundEngine->play2D("sounds/phaseChange.mp3");
		}
		else {
			soundEngine->play2D("sounds/bosshit.mp3");
			if (!secPhase)
				sprite->changeAnimation(FIRST_PHASE_HIT);
			else
				sprite->changeAnimation(SECOND_PHASE_HIT);
			startAnimFrame = currentFrame;
		}
	}
}

int Boss::getLife() {
	return lives;
}

bool Boss::getHitting() {
	return hitting;
}

void Boss::die() {
	soundEngine->stopAllSounds();
	soundEngine->play2D("sounds/dying2.mp3");
	Game::instance().win();
}

void Boss::shoot() {
	shooting = true;
	shot1 = new Shot();
	shot1->init(glm::ivec2(0, 0), texProgram);
	shot1->setDirection(glm::vec2(0.f, 1.f));
	shot1->setPosition(glm::ivec2(posBoss.x + 60, posBoss.y + 110));
	shot1->setPlayer(Game::instance().getPlayer());
	shot1->setTileMap(Game::instance().getTileMap());

	soundEngine->play2D("sounds/bossshot.mp3");
	if (!secPhase) {
		sprite->changeAnimation(FIRST_PHASE_SHOT);
	}
	else {
		sprite->changeAnimation(SECOND_PHASE_SHOT);
		shot2 = new Shot();
		shot2->init(glm::ivec2(0, 0), texProgram);
		shot2->setDirection(normalize(glm::vec2(-1.f, 1.f)));
		shot2->setPosition(glm::ivec2(posBoss.x + 60, posBoss.y + 110));
		shot2->setPlayer(Game::instance().getPlayer());
		shot2->setTileMap(Game::instance().getTileMap());

		shot3 = new Shot();
		shot3->init(glm::ivec2(0, 0), texProgram);
		shot3->setDirection(normalize(glm::vec2(1.f, 1.f)));
		shot3->setPosition(glm::ivec2(posBoss.x + 60, posBoss.y + 110));
		shot3->setPlayer(Game::instance().getPlayer());
		shot3->setTileMap(Game::instance().getTileMap());
	}
}

void Boss::stopShooting() {
	shooting = false;
}