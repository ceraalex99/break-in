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

	sticky = true;
	
	speed = 6;
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 0);
	sprite->addKeyframe(0, glm::vec2(0, 0));
	tileMapDispl = tileMapPos;
	direction = glm::vec2(0.f, 0.f);
	animation = false;
	tiempo = 0;
	sprite->changeAnimation(0);
	powerUpSticky = false;
	stopped = false;
	animation = false;
	deadBall = false;
}

void Ball::update(int deltaTime) {
	if (!deadBall) {
		sprite->update(deltaTime);
	
		if (animation) {
			if (tiempo < 2000) {
				tiempo += deltaTime;
			}
			else {
				posBall.y -= 0.5;
				if (posBall.y < 4) {
					posBall.y += 0.5;
				}
				else {
					posBall.y -= 0.5;
					if (posBall.y < 4) {
						posBall.y += 0.5;
					}
					else {
						posBall.y -= 0.5;
						if (posBall.y < 4) {
							posBall.y += 0.5;
						}
					}
				}
				if (posBall.y < 5) {
					Game::instance().nextRoom();
				}
			}

		}
		else {
			posBall += direction * speed;
			if (sticky) {
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
				if (Game::instance().getSpecialKey(GLUT_KEY_UP) || Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
					direction = normalize(glm::vec2(0.4f, -1.f));
					sticky = false;
				}
			}
			else {

				if (posBall.y > 450) {
					if (Game::instance().getCurrentRoom() == 0 || Game::instance().getCurrentRoom() == 4) {
						if (Game::instance().getGodMode()) {
							direction.y = -direction.y;
						}
						else {
							Game::instance().loseBall();
							if (Game::instance().getNumberBalls() > 1) {
								deadBall = true;
							}
						}
					}
					else if (direction.y > 0) {
						if (Game::instance().getNumberBalls() > 1) {
							Game::instance().loseBall();
							deadBall = true;
						}
						else Game::instance().previousRoom();
					}

				}
				else if (posBall.y < 5) {
					if (direction.y < 0)
						Game::instance().nextRoom();
				}
				else {
					if ((direction.y < 0 && direction.x > 0 && map->collisionMoveUpRight(posBall, glm::ivec2(24, 24))) || (direction.y < 0 && direction.x < 0 && map->collisionMoveUpLeft(posBall, glm::ivec2(24, 24))) || (direction.y > 0 && direction.x > 0 && map->collisionMoveDownRight(posBall, glm::ivec2(24, 24))) || (direction.y > 0 && direction.x < 0 && map->collisionMoveDownLeft(posBall, glm::ivec2(24, 24)))) {
						direction.y = -direction.y;
						direction.x = -direction.x;
					}
					else if ((direction.y < 0 && map->collisionMoveUp(posBall, glm::ivec2(24, 24))) || (direction.y > 0 && map->collisionMoveDown(posBall, glm::ivec2(24, 24), &posBall.y))) {
						direction.y = -direction.y;
						posBall.y += direction.y * speed;
					}
					else if ((direction.x > 0 && map->collisionMoveRight(posBall, glm::ivec2(24, 24))) || (direction.x < 0 && map->collisionMoveLeft(posBall, glm::ivec2(24, 24)))) {
						direction.x = -direction.x;
						posBall.x += direction.x * speed;
					}
					else if (direction.y >= -0.3) {
						collisionPlayer();
					}

					if (Game::instance().getSceneState() == 4 && boss->getLife() > 0) {
						if (collisionBoss()) {
							boss->hit();
						}
					}
					if (!stopped) player->checkAnimation(posBall);
				}

			}
		}

		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
	}
}

void Ball::render() {
	if(!deadBall) sprite->render();
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

glm::vec2 Ball::getDirection() {
	return direction;
}

void Ball::setDirection(glm::vec2 dir) {
	direction = dir;
}

void Ball::stop() {
	speed = 0;
	stopped = true;
}

void Ball::reset(const glm::vec2 &pos) {
	sticky = true;
	speed = 6;
	stopped = false;
	direction = glm::vec2(0.f, 0.f);
	posBall = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}

void Ball::collisionPlayer() {

	bool doble = player->getLargePlayer();
	glm::ivec2 posPlayer = player->getPosition();
	int x0Ball = posBall.x;
	int x1Ball = posBall.x + 23;

	int yBall = posBall.y + 24;
	
	int x0Player, x1Player;
	if (doble) {
		x0Player = posPlayer.x - 23;
		x1Player = posPlayer.x + 70;
	}
	else {
		x0Player = posPlayer.x;
		x1Player = posPlayer.x + 47;
	}
	
	int y0Player = posPlayer.y;
	int y1Player = posPlayer.y + 14;

	if ((x1Ball > x0Player) && (x0Ball < x1Player)) {
		if ((y1Player > yBall) && (yBall > y0Player)) {
			if (Game::instance().getGodMode()) {
				direction = glm::vec2(0.f, -1.f);
				posBall.y += direction.y * speed;
			}
			else {
				if (powerUpSticky) {
					sticky = true;
					direction.y = -1.f;
					posBall.y += direction.y * speed;
					direction = glm::vec2(0.f, 0.f);
				}
				else {
					direction.x = ((posBall.x + 12.f) - (posPlayer.x + 24.f)) / 12.f;
					direction.y = -1.f;
					posBall.y += direction.y * speed;
					direction = normalize(direction);
				}
			}

		}
	}
}

bool Ball::collisionBoss() {
	glm::ivec2 posBoss = boss->getPosition();
	int x0Boss = posBoss.x + 20;
	int x1Boss = posBoss.x + 100;
	int y0Boss = posBoss.y + 90;
	int y1Boss = posBoss.y + 100;

	int yBall = posBall.y;
	int x0Ball = posBall.x;
	int x1Ball = posBall.x + 23;

	if ((x1Ball > x0Boss) && (x0Ball < x1Boss)) {
		if (y1Boss > yBall) {
			if (y0Boss < yBall) {
				if (direction.y < 0) {
					direction.y = -direction.y;
					posBall.y += direction.y * speed;
				}
			}
			return true;
		}
	}
	return false;
}

void Ball::setBoss(Boss *b) {
	boss = b;
}

bool Ball::getSticky() {
	return sticky;
}

void Ball::setAnimationBall(bool anim) {
	animation = anim;
}

void Ball::setPowerUpSticky(bool powerUp) {
	powerUpSticky = powerUp;
}

void Ball::setSpeed(int spd) {
	speed = spd;
}

void Ball::setDeadBall(bool dead) {
	deadBall = dead;
}