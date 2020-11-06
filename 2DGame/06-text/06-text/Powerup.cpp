#include "Powerup.h"
#include "Game.h"

enum Color {
	YELLOW, RED, BLUE, GREEN, WHITE, PINK
};

Powerup::Powerup()
{
}


Powerup::~Powerup()
{
}

Color color;

void Powerup::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	tileMapDispl = tileMapPos;


	spritesheet.loadFromFile("images/powerups.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25f, 0.25f), &spritesheet, &shaderProgram);

	
	sprite->setNumberAnimations(6);

	sprite->setAnimationSpeed(YELLOW, 0);
	sprite->addKeyframe(YELLOW, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(RED, 0);
	sprite->addKeyframe(RED, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(BLUE, 0);
	sprite->addKeyframe(BLUE, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(GREEN, 0);
	sprite->addKeyframe(GREEN, glm::vec2(0.f, 0.25f));

	sprite->setAnimationSpeed(WHITE, 0);
	sprite->addKeyframe(WHITE, glm::vec2(0.25f, 0.25f));

	sprite->setAnimationSpeed(PINK, 0);
	sprite->addKeyframe(PINK, glm::vec2(0.5f, 0.25f));

	sprite->changeAnimation(WHITE);
	
	direction = glm::vec2(1.f, -1.f);
	animTimer = 0;
	color = WHITE;
}

void Powerup::update(int deltaTime)
{
	animTimer += deltaTime;

	if (animTimer > 3000) {
		animTimer = 0;
		color = Color((color + 1) % 6);
		sprite->changeAnimation(color);
	}

	sprite->update(deltaTime);
	posPowerup += direction;

	if (posPowerup.x < 18 && direction.x < 0) {
		direction.x = rand() % 2 + 1;
		posPowerup += direction;
	}
	else if (posPowerup.x > 430 && direction.x > 0) {
		direction.x = -(rand() % 2 + 1);
		posPowerup += direction;
	}
	
	if (posPowerup.y < 18 && direction.y < 0) {
		direction.y = rand() % 2 + 1;
		posPowerup += direction;
	}
	else if (posPowerup.y > 430 && direction.y > 0) {
		direction.y = -(rand() % 2 + 1);
		posPowerup += direction;
	}
	
	if (collisionPlayer()) {
		Game::instance().catchPowerup();
		switch (color) {
			case YELLOW:
				break;
			case RED:
				break;
			case BLUE:
				break;
			case GREEN:
				break;
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPowerup.x), float(tileMapDispl.y + posPowerup.y)));
}

void Powerup::render()
{
	sprite->render();
}

bool Powerup::collisionPlayer()
{
	glm::ivec2 posPlayer = player->getPosition();
	int x0Ball = posPowerup.x;
	int x1Ball = posPowerup.x + 31;

	int y0Ball = posPowerup.y;
	int y1Ball = posPowerup.y + 31;

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

void Powerup::setPlayer(Player * play)
{
	player = play;
}

void Powerup::setPosition(const glm::vec2 & pos)
{
	posPowerup = pos;
}




