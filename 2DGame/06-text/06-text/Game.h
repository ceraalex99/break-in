#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE

#include <irrKlang.h>
#include "Scene.h"
#include "Menu.h"
#include "Instructions.h"
#include "Credits.h"
#pragma comment(lib, "irrKlang.lib")



// Game is a singleton (a class with a single instance) that represents our whole application

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

class Game
{

public:
	Game() {}
	
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

	void startAction(int action);
	void catchKey();
	void breakBrick();

	int getPoints();
	int getMoney();
	int getCurrentBank();
	void loseLife();

private:
	bool bPlay;                       // Continue to play game?
	Scene scene;                      // Scene to render
	Menu menu;
	Instructions instructions;
	Credits credits;
	int gameState;
	bool keys[256], specialKeys[256];
	irrklang::ISoundEngine* soundEngine;
	int points, money, currentBank;
};


#endif // _GAME_INCLUDE


