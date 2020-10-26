#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Quad.h"
#include "TexturedQuad.h"
#include "Text.h"
#include "Player.h"
#include "Ball.h"
#include <irrKlang.h>


#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	void setSoundEngine(irrklang::ISoundEngine* eng);
	void catchKey();

	void nextRoom();
	void previousRoom();

private:
	void initShaders();

private:
	Texture arrowsTexture;
	TexturedQuad *arrows;
	ShaderProgram simpleProgram, texProgram;
	float currentTime;
	glm::mat4 projection;
	Text text;
	Player *player;
	TileMap *map[3];
	bool haveKey[3];
	Ball *ball;
	Texture meshTexture, lettersTexture;
	TexturedQuad *mesh, *letters;
	int currentRoom;
	
	irrklang::ISoundEngine* soundEngine;
};


#endif // _SCENE_INCLUDE

