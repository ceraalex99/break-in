#ifndef _VIGILANT_INCLUDE
#define _VIGILANT_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"
class Vigilant
{
public:
	Vigilant();
	~Vigilant();
	void init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram);
	void update(int deltaTime);
	void setPosition(const glm::vec2 & pos);
	void render();
	bool getActivo();
	void setPlayer(Player * play);
	void setTileMap(TileMap * tileMap);
	void alarm();
	void reset();
	bool collisionPlayer();
private:
	glm::ivec2 tileMapDispl, posVigilant;
	Texture spritesheet;
	Sprite *sprite;
	Player *player;
	TileMap *map;
	int speed;
	bool activo;
	int tiempo;
};
#endif
