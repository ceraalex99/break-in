#include "ShotPlayer.h"
#include "Game.h"

ShotPlayer::ShotPlayer()
{
}

ShotPlayer::~ShotPlayer()
{
}

void ShotPlayer::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/enemyShot.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(24, 24), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);

	speed = 6;
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 0);
	sprite->addKeyframe(0, glm::vec2(0, 0));
	tileMapDispl = tileMapPos;
	sprite->changeAnimation(0);
	deadShot = false;
}

void ShotPlayer::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (!deadShot) {

		posBall += direction * speed;
	
		if (map->collisionMoveUp(posBall, glm::ivec2(24, 24))) {
			deadShot = true;
			Game::instance().setShotCreated(false);
		}

		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
	}
}

void ShotPlayer::render()
{
	sprite->render();
}

void ShotPlayer::setTileMap(TileMap * tileMap)
{
	map = tileMap;
}

void ShotPlayer::setPosition(const glm::vec2 & pos)
{
	posBall = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));

}

glm::vec2 ShotPlayer::getPosition()
{
	return posBall;
}

glm::vec2 ShotPlayer::getDirection()
{
	return direction;
}

void ShotPlayer::setDirection(glm::vec2 dir)
{
	direction = dir;
}

void ShotPlayer::setdeadShot(bool newDeadShot) {
	deadShot = newDeadShot;
}