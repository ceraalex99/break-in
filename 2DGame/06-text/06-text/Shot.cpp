#include "Shot.h"
#include "Game.h"


Shot::Shot()
{
}


Shot::~Shot()
{
}

void Shot::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/enemyShot.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(24, 24), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);

	speed = 6;
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 0);
	sprite->addKeyframe(0, glm::vec2(0, 0));
	tileMapDispl = tileMapPos;
	sprite->changeAnimation(0);
}

void Shot::update(int deltaTime)
{
	sprite->update(deltaTime);

	posBall += direction * speed;
	if ((direction.x < 0 && posBall.x < 5) || (direction.x > 0 && posBall.x > 450)) {
		direction.x = -direction.x;
		posBall.x += direction.x * speed;
	}
	
	if (collisionPlayer()) {
		Game::instance().loseLife();
		Game::instance().stopBossShooting();
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}

void Shot::render()
{
	sprite->render();
}



bool Shot::collisionPlayer() {
	glm::ivec2 posPlayer = player->getPosition();
	int x0Ball = posBall.x;
	int x1Ball = posBall.x + 23;

	int y0Ball = posBall.y;
	int y1Ball = posBall.y + 24;

	int x0Player = posPlayer.x;
	int x1Player = posPlayer.x + 47;

	int y0Player = posPlayer.y;
	int y1Player = posPlayer.y + 14;

	if ((x1Ball > x0Player) && (x0Ball < x1Player)) {
		if ((y1Player > y0Ball) && (y1Ball > y0Player)) {
			return true;
		}
	}
	return false;
}

void Shot::setTileMap(TileMap * tileMap)
{
	map = tileMap;
}

void Shot::setPlayer(Player * play)
{
	player = play;
}

void Shot::setPosition(const glm::vec2 & pos)
{
	posBall = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));

}

glm::vec2 Shot::getPosition()
{
	return posBall;
}

glm::vec2 Shot::getDirection()
{
	return direction;
}

void Shot::setDirection(glm::vec2 dir)
{
	direction = dir;
}
