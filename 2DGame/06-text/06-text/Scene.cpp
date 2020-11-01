#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include <iomanip>
#include <sstream>


#define INIT_PLAYER_X_TILES 5
#define INIT_PLAYER_Y_TILES 20

enum State {
	PLAYING, GAME_OVER, GAME_WIN, LOSE_LIFE, BOSS_FIGHT
};

State state;

Scene::Scene()
{
	player = NULL;
	map[0] = NULL;
	map[1] = NULL;
	map[2] = NULL;
}

Scene::~Scene()
{
	if(arrows != NULL)
		delete arrows;
	if (player != NULL)
		delete player;
}


void Scene::init()
{
	soundEngine->stopAllSounds();
	soundEngine->play2D("sounds/backgroundMusicLow.wav", true);

	glm::vec2 geom[2] = {glm::vec2(0.f, 0.f), glm::vec2(160.f, 16.f)};
	glm::vec2 texCoords[2] = {glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f)};
	haveKey[0] = false;
	haveKey[1] = false;
	haveKey[2] = false;
	haveKey[3] = false;
	haveKey[4] = false;
	haveKey[5] = false;
	haveKey[6] = false;
	haveKey[7] = false;
	haveKey[8] = false;
	haveKey[9] = false;
	haveKey[10] = false;
	haveKey[11] = false;
	haveKey[12] = false;

	currentRoom = 0;
	lives = 4;
	

	initShaders();
	
	
	arrows = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	arrowsTexture.loadFromFile("images/arrows.png", TEXTURE_PIXEL_FORMAT_RGBA);
	arrowsTexture.setMagFilter(GL_NEAREST);

	geom[1] = glm::vec2(480.f, 480.f);
	mesh = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	meshTexture.loadFromFile("images/mesh.png", TEXTURE_PIXEL_FORMAT_RGBA);
	meshTexture.setMagFilter(GL_NEAREST);

	geom[1] = glm::vec2(160.f, 480.f);
	lettersTexture.loadFromFile("images/letters.png", TEXTURE_PIXEL_FORMAT_RGBA);
	letters = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	lettersTexture.setMagFilter(GL_NEAREST);

	map[0] = TileMap::createTileMap("levels/01-01.txt", glm::vec2(0, 0), texProgram);
	map[1] = TileMap::createTileMap("levels/01-02.txt", glm::vec2(0, -560), texProgram);
	map[2] = TileMap::createTileMap("levels/01-03.txt", glm::vec2(0, -1120), texProgram);
	map[3] = TileMap::createTileMap("levels/01-bonus.txt", glm::vec2(0, -1680), texProgram);
	map[4] = TileMap::createTileMap("levels/emptyLevel.txt", glm::vec2(0, -2240), texProgram);
	map[5] = TileMap::createTileMap("levels/02-01.txt", glm::vec2(0, -2800), texProgram);
	map[6] = TileMap::createTileMap("levels/02-02.txt", glm::vec2(0, -3360), texProgram);
	map[7] = TileMap::createTileMap("levels/02-03.txt", glm::vec2(0, -3920), texProgram);
	map[8] = TileMap::createTileMap("levels/02-bonus.txt", glm::vec2(0, -4480), texProgram);
	map[9] = TileMap::createTileMap("levels/03-01.txt", glm::vec2(0, -5040), texProgram);
	map[10] = TileMap::createTileMap("levels/03-02.txt", glm::vec2(0, -5600), texProgram);
	map[11] = TileMap::createTileMap("levels/03-03.txt", glm::vec2(0, -6160), texProgram);
	map[12] = TileMap::createTileMap("levels/03-bonus.txt", glm::vec2(0, -6720), texProgram);

	map[0]->setShaderProgram(texProgram);
	map[1]->setShaderProgram(texProgram);
	map[2]->setShaderProgram(texProgram);
	map[3]->setShaderProgram(texProgram);
	map[4]->setShaderProgram(texProgram);
	map[5]->setShaderProgram(texProgram);
	map[6]->setShaderProgram(texProgram);
	map[7]->setShaderProgram(texProgram);
	map[8]->setShaderProgram(texProgram);
	map[9]->setShaderProgram(texProgram);
	map[10]->setShaderProgram(texProgram);
	map[11]->setShaderProgram(texProgram);
	map[12]->setShaderProgram(texProgram);
	map[0]->setSoundEngine(soundEngine);
	map[1]->setSoundEngine(soundEngine);
	map[2]->setSoundEngine(soundEngine);
	map[3]->setSoundEngine(soundEngine);
	map[4]->setSoundEngine(soundEngine);
	map[5]->setSoundEngine(soundEngine);
	map[6]->setSoundEngine(soundEngine);
	map[7]->setSoundEngine(soundEngine);
	map[8]->setSoundEngine(soundEngine);
	map[9]->setShaderProgram(texProgram);
	map[10]->setShaderProgram(texProgram);
	map[11]->setShaderProgram(texProgram);
	map[12]->setShaderProgram(texProgram);
	player = new Player();
	player->init(glm::ivec2(0, 0), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map[0]->getTileSize(), INIT_PLAYER_Y_TILES * map[0]->getTileSize() / 2));
	player->setTileMap(map[0]);
	
	ball = new Ball();
	ball->init(glm::ivec2(0, 0), texProgram);
	ball->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map[0]->getTileSize()+9, (INIT_PLAYER_Y_TILES-1.3) * map[0]->getTileSize() / 2));
	ball->setTileMap(map[0]);
	ball->setPlayer(player);


	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	
	// Select which font you want to use
	if(!text.init("fonts/ARCADEPI.ttf"))
	//if(!text.init("fonts/OpenSans-Bold.ttf"))
	//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;

	reloadMoney();
	reloadPoints();
	reloadLives();
	reloadBank();
	reloadRoom();

}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	ball->update(deltaTime);
	if (state == BOSS_FIGHT) {
		boss->update(deltaTime);
	}
	map[0]->update(deltaTime);
	map[1]->update(deltaTime);
	map[2]->update(deltaTime);
	map[3]->update(deltaTime);
	map[4]->update(deltaTime);
	map[5]->update(deltaTime);
	map[6]->update(deltaTime);
	map[7]->update(deltaTime);
	map[8]->update(deltaTime);
	map[9]->update(deltaTime);
	map[10]->update(deltaTime);
	map[11]->update(deltaTime);
	map[12]->update(deltaTime);

	if (state == LOSE_LIFE) {
		if (loseTime + 1500 < currentTime) {
			state = PLAYING;
			resetPlayer();
		}
	}

	if (Game::instance().getKey('p')) {
		Game::instance().keyReleased('p');
		if (currentRoom < 13) {
			nextRoom();
			ball->setDirection(glm::vec2(ball->getDirection().x, -abs(ball->getDirection().y)));
		}
	}
	else if (Game::instance().getKey('o')) {
		Game::instance().keyReleased('o');
		if (currentRoom > 0) {
			previousRoom();
			ball->setDirection(glm::vec2(ball->getDirection().x, abs(ball->getDirection().y)));
		}
	}

}

void Scene::render()
{
	glm::mat4 modelview;

	
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	
	mesh->render(meshTexture);
	map[0]->render();
	map[1]->render();
	map[2]->render();
	map[3]->render();
	map[4]->render();
	map[5]->render();
	map[6]->render();
	map[7]->render();
	map[8]->render();
	map[9]->render();
	map[10]->render();
	map[11]->render();
	map[12]->render();
	player->render();
	ball->render();
	if (state == BOSS_FIGHT) {
		boss->render();
	}

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(480.f, 0.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	letters->render(lettersTexture);

	
	
	text.render(moneyStr.str(), glm::vec2(500, 44), 22, glm::vec4(0, 1, 0, 1));
	
	text.render(pointsStr.str(), glm::vec2(500, 142), 22, glm::vec4(0, 1, 0, 1));
	
	text.render(livesStr.str(), glm::vec2(585, 264), 22, glm::vec4(0, 1, 0, 1));
	
	
	text.render(bankStr.str(), glm::vec2(585, 344), 22, glm::vec4(0, 1, 0, 1));
	
	text.render(roomStr.str(), glm::vec2(585, 474), 22, glm::vec4(0, 1, 0, 1));
	
	if (haveKey[currentRoom]) {
		texProgram.use();
		texProgram.setUniformMatrix4f("projection", projection);
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

		modelview = glm::translate(glm::mat4(1.0f), glm::vec3(160.f, 0.f, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		arrows->render(arrowsTexture);
	}
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/simple.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/simple.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleProgram.init();
	simpleProgram.addShader(vShader);
	simpleProgram.addShader(fShader);
	simpleProgram.link();
	if(!simpleProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleProgram.log() << endl << endl;
	}
	simpleProgram.bindFragmentOutput("outColor");

	vShader.free();
	fShader.free();
	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
}

void Scene::setSoundEngine(irrklang::ISoundEngine* eng) {
	soundEngine = eng;
}

void Scene::catchKey() {
	haveKey[currentRoom] = true;
	map[currentRoom]->openExit();
}

void Scene::nextRoom() {
	currentRoom++;
	switch (currentRoom) {
		case 1:
			map[0]->moveTileMap(glm::vec2(0, 560));
			map[1]->moveTileMap(glm::vec2(0, 0));
			map[2]->moveTileMap(glm::vec2(0, -560));
			map[3]->moveTileMap(glm::vec2(0, -1120));
			map[4]->moveTileMap(glm::vec2(0, -1680));
			map[5]->moveTileMap(glm::vec2(0, -2240));
			map[6]->moveTileMap(glm::vec2(0, -2800));
			map[7]->moveTileMap(glm::vec2(0, -3360));
			map[8]->moveTileMap(glm::vec2(0, -3920));
			map[9]->moveTileMap(glm::vec2(0, -4480));
			map[10]->moveTileMap(glm::vec2(0, -5040));
			map[11]->moveTileMap(glm::vec2(0, -5600));
			map[12]->moveTileMap(glm::vec2(0, -6160));
			break;
		case 2:
			map[0]->moveTileMap(glm::vec2(0, 1120));
			map[1]->moveTileMap(glm::vec2(0, 560));
			map[2]->moveTileMap(glm::vec2(0, 0));
			map[3]->moveTileMap(glm::vec2(0, -560));
			map[4]->moveTileMap(glm::vec2(0, -1120));
			map[5]->moveTileMap(glm::vec2(0, -1680));
			map[6]->moveTileMap(glm::vec2(0, -2240));
			map[7]->moveTileMap(glm::vec2(0, -2800));
			map[8]->moveTileMap(glm::vec2(0, -3360));
			map[9]->moveTileMap(glm::vec2(0, -3920));
			map[10]->moveTileMap(glm::vec2(0, -4480));
			map[11]->moveTileMap(glm::vec2(0, -5040));
			map[12]->moveTileMap(glm::vec2(0, -5600));
			break;
		case 3:
			map[0]->moveTileMap(glm::vec2(0, 1680));
			map[1]->moveTileMap(glm::vec2(0, 1120));
			map[2]->moveTileMap(glm::vec2(0, 560));
			map[3]->moveTileMap(glm::vec2(0, 0));
			map[4]->moveTileMap(glm::vec2(0, -560));
			map[5]->moveTileMap(glm::vec2(0, -1120));
			map[6]->moveTileMap(glm::vec2(0, -1680));
			map[7]->moveTileMap(glm::vec2(0, -2240));
			map[8]->moveTileMap(glm::vec2(0, -2800));
			map[9]->moveTileMap(glm::vec2(0, -3360));
			map[10]->moveTileMap(glm::vec2(0, -3920));
			map[11]->moveTileMap(glm::vec2(0, -4480));
			map[12]->moveTileMap(glm::vec2(0, -5040));
			break;
		case 4:
			map[0]->moveTileMap(glm::vec2(0, 2240));
			map[1]->moveTileMap(glm::vec2(0, 1680));
			map[2]->moveTileMap(glm::vec2(0, 1120));
			map[3]->moveTileMap(glm::vec2(0, 560));
			map[4]->moveTileMap(glm::vec2(0, 0));
			map[5]->moveTileMap(glm::vec2(0,-560));
			map[6]->moveTileMap(glm::vec2(0, -1120));
			map[7]->moveTileMap(glm::vec2(0, -1680));
			map[8]->moveTileMap(glm::vec2(0, -2240));
			map[9]->moveTileMap(glm::vec2(0, -2800));
			map[10]->moveTileMap(glm::vec2(0, -3360));
			map[11]->moveTileMap(glm::vec2(0, -3920));
			map[12]->moveTileMap(glm::vec2(0, -4480));
			
			break;
		case 5:
			map[0]->moveTileMap(glm::vec2(0, 2800));
			map[1]->moveTileMap(glm::vec2(0, 2240));
			map[2]->moveTileMap(glm::vec2(0, 1680));
			map[3]->moveTileMap(glm::vec2(0, 1120));
			map[4]->moveTileMap(glm::vec2(0, 560));
			map[5]->moveTileMap(glm::vec2(0, 0));
			map[6]->moveTileMap(glm::vec2(0, -560));
			map[7]->moveTileMap(glm::vec2(0, -1120));
			map[8]->moveTileMap(glm::vec2(0, -1680));
			map[9]->moveTileMap(glm::vec2(0, -2240));
			map[10]->moveTileMap(glm::vec2(0, -2800));
			map[11]->moveTileMap(glm::vec2(0, -3360));
			map[12]->moveTileMap(glm::vec2(0, -3920));
			break;
		case 6:
			map[0]->moveTileMap(glm::vec2(0, 3360));
			map[1]->moveTileMap(glm::vec2(0, 2800));
			map[2]->moveTileMap(glm::vec2(0, 2240));
			map[3]->moveTileMap(glm::vec2(0, 1680));
			map[4]->moveTileMap(glm::vec2(0, 1120));
			map[5]->moveTileMap(glm::vec2(0, 560));
			map[6]->moveTileMap(glm::vec2(0, 0));
			map[7]->moveTileMap(glm::vec2(0, -560));
			map[8]->moveTileMap(glm::vec2(0, -1120));
			map[9]->moveTileMap(glm::vec2(0, -1680));
			map[10]->moveTileMap(glm::vec2(0, -2240));
			map[11]->moveTileMap(glm::vec2(0, -2800));
			map[12]->moveTileMap(glm::vec2(0, -3360));
			break;
		case 7:
			map[0]->moveTileMap(glm::vec2(0, 3920));
			map[1]->moveTileMap(glm::vec2(0, 3360));
			map[2]->moveTileMap(glm::vec2(0, 2800));
			map[3]->moveTileMap(glm::vec2(0, 2240));
			map[4]->moveTileMap(glm::vec2(0, 1680));
			map[5]->moveTileMap(glm::vec2(0, 1120));
			map[6]->moveTileMap(glm::vec2(0, 560));
			map[7]->moveTileMap(glm::vec2(0, 0));
			map[8]->moveTileMap(glm::vec2(0, -560));
			map[9]->moveTileMap(glm::vec2(0, -1120));
			map[10]->moveTileMap(glm::vec2(0, -1680));
			map[11]->moveTileMap(glm::vec2(0, -2240));
			map[12]->moveTileMap(glm::vec2(0, -2800));
			break;
		case 8:
			map[0]->moveTileMap(glm::vec2(0, 4480));
			map[1]->moveTileMap(glm::vec2(0, 3920));
			map[2]->moveTileMap(glm::vec2(0, 3360));
			map[3]->moveTileMap(glm::vec2(0, 2800));
			map[4]->moveTileMap(glm::vec2(0, 2240));
			map[5]->moveTileMap(glm::vec2(0, 1680));
			map[6]->moveTileMap(glm::vec2(0, 1120));
			map[7]->moveTileMap(glm::vec2(0, 560));
			map[8]->moveTileMap(glm::vec2(0, 0));
			map[9]->moveTileMap(glm::vec2(0, -560));
			map[10]->moveTileMap(glm::vec2(0, -1120));
			map[11]->moveTileMap(glm::vec2(0, -1680));
			map[12]->moveTileMap(glm::vec2(0, -2240));
			break;
		case 9:
			map[0]->moveTileMap(glm::vec2(0, 5040));
			map[1]->moveTileMap(glm::vec2(0, 4480));
			map[2]->moveTileMap(glm::vec2(0, 3920));
			map[3]->moveTileMap(glm::vec2(0, 3360));
			map[4]->moveTileMap(glm::vec2(0, 2800));
			map[5]->moveTileMap(glm::vec2(0, 2240));
			map[6]->moveTileMap(glm::vec2(0, 1680));
			map[7]->moveTileMap(glm::vec2(0, 1120));
			map[8]->moveTileMap(glm::vec2(0, 560));
			map[9]->moveTileMap(glm::vec2(0, 0));
			map[10]->moveTileMap(glm::vec2(0, -560));
			map[11]->moveTileMap(glm::vec2(0, -1120));
			map[12]->moveTileMap(glm::vec2(0, -1680));
			break;
		case 10:
			map[0]->moveTileMap(glm::vec2(0, 5600));
			map[1]->moveTileMap(glm::vec2(0, 5040));
			map[2]->moveTileMap(glm::vec2(0, 4480));
			map[3]->moveTileMap(glm::vec2(0, 3920));
			map[4]->moveTileMap(glm::vec2(0, 3360));
			map[5]->moveTileMap(glm::vec2(0, 2800));
			map[6]->moveTileMap(glm::vec2(0, 2240));
			map[7]->moveTileMap(glm::vec2(0, 1680));
			map[8]->moveTileMap(glm::vec2(0, 1120));
			map[9]->moveTileMap(glm::vec2(0, 560));
			map[10]->moveTileMap(glm::vec2(0, 0));
			map[11]->moveTileMap(glm::vec2(0, -560));
			map[12]->moveTileMap(glm::vec2(0, -1120));
			break;
		case 11:
			map[0]->moveTileMap(glm::vec2(0, 6160));
			map[1]->moveTileMap(glm::vec2(0, 5600));
			map[2]->moveTileMap(glm::vec2(0, 5040));
			map[3]->moveTileMap(glm::vec2(0, 4480));
			map[4]->moveTileMap(glm::vec2(0, 3920));
			map[5]->moveTileMap(glm::vec2(0, 3360));
			map[6]->moveTileMap(glm::vec2(0, 2800));
			map[7]->moveTileMap(glm::vec2(0, 2240));
			map[8]->moveTileMap(glm::vec2(0, 1680));
			map[9]->moveTileMap(glm::vec2(0, 1120));
			map[10]->moveTileMap(glm::vec2(0, 560));
			map[11]->moveTileMap(glm::vec2(0, 0));
			map[12]->moveTileMap(glm::vec2(0, -560));
			break;
		case 12:
			map[0]->moveTileMap(glm::vec2(0, 6720));
			map[1]->moveTileMap(glm::vec2(0, 6160));
			map[2]->moveTileMap(glm::vec2(0, 5600));
			map[3]->moveTileMap(glm::vec2(0, 5060));
			map[4]->moveTileMap(glm::vec2(0, 4480));
			map[5]->moveTileMap(glm::vec2(0, 3920));
			map[6]->moveTileMap(glm::vec2(0, 3360));
			map[7]->moveTileMap(glm::vec2(0, 2800));
			map[8]->moveTileMap(glm::vec2(0, 2240));
			map[9]->moveTileMap(glm::vec2(0, 1680));
			map[10]->moveTileMap(glm::vec2(0, 1120));
			map[11]->moveTileMap(glm::vec2(0, 560));
			map[12]->moveTileMap(glm::vec2(0, 0));
			break;
	}
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map[0]->getTileSize(), INIT_PLAYER_Y_TILES * map[0]->getTileSize() / 2));
	if (ball->getSticky()) {
		ball->setPosition(glm::vec2(INIT_PLAYER_X_TILES*map[0]->getTileSize()+9, (INIT_PLAYER_Y_TILES-1.3) * map[0]->getTileSize() / 2));
	}
	else {
		ball->setPosition(glm::vec2(ball->getPosition().x, 480));
	}
	player->setTileMap(map[currentRoom]);
	ball->setTileMap(map[currentRoom]);
}

void Scene::previousRoom() {
	currentRoom--;
	switch (currentRoom) {
		case 0:
			map[0]->moveTileMap(glm::vec2(0, 0));
			map[1]->moveTileMap(glm::vec2(0, -560));
			map[2]->moveTileMap(glm::vec2(0, -1120));
			map[3]->moveTileMap(glm::vec2(0, -1680));
			map[4]->moveTileMap(glm::vec2(0, -2240));
			map[5]->moveTileMap(glm::vec2(0, -2800));
			map[6]->moveTileMap(glm::vec2(0, -3360));
			map[7]->moveTileMap(glm::vec2(0, -3920));
			map[8]->moveTileMap(glm::vec2(0, -4480));
			map[9]->moveTileMap(glm::vec2(0, -5040));
			map[10]->moveTileMap(glm::vec2(0, -5600));
			map[11]->moveTileMap(glm::vec2(0, -6160));
			map[12]->moveTileMap(glm::vec2(0, -6720));
			break;
		case 1:
			map[0]->moveTileMap(glm::vec2(0, 560));
			map[1]->moveTileMap(glm::vec2(0, 0));
			map[2]->moveTileMap(glm::vec2(0, -560));
			map[3]->moveTileMap(glm::vec2(0, -1120));
			map[4]->moveTileMap(glm::vec2(0, -1680));
			map[5]->moveTileMap(glm::vec2(0, -2240));
			map[6]->moveTileMap(glm::vec2(0, -2800));
			map[7]->moveTileMap(glm::vec2(0, -3360));
			map[8]->moveTileMap(glm::vec2(0, -3920));
			map[9]->moveTileMap(glm::vec2(0, -4480));
			map[10]->moveTileMap(glm::vec2(0, -5040));
			map[11]->moveTileMap(glm::vec2(0, -5600));
			map[12]->moveTileMap(glm::vec2(0, -6160));
			break;
		case 2:
			map[0]->moveTileMap(glm::vec2(0, 1120));
			map[1]->moveTileMap(glm::vec2(0, 560));
			map[2]->moveTileMap(glm::vec2(0, 0));
			map[3]->moveTileMap(glm::vec2(0, -560));
			map[4]->moveTileMap(glm::vec2(0, -1120));
			map[5]->moveTileMap(glm::vec2(0, -1680));
			map[6]->moveTileMap(glm::vec2(0, -2240));
			map[7]->moveTileMap(glm::vec2(0, -2800));
			map[8]->moveTileMap(glm::vec2(0, -3360));
			map[9]->moveTileMap(glm::vec2(0, -3920));
			map[10]->moveTileMap(glm::vec2(0, -4480));
			map[11]->moveTileMap(glm::vec2(0, -5040));
			map[12]->moveTileMap(glm::vec2(0, -5600));
			break;
		case 3:
			map[0]->moveTileMap(glm::vec2(0, 1680));
			map[1]->moveTileMap(glm::vec2(0, 1120));
			map[2]->moveTileMap(glm::vec2(0, 560));
			map[3]->moveTileMap(glm::vec2(0, 0));
			map[4]->moveTileMap(glm::vec2(0, -560));
			map[5]->moveTileMap(glm::vec2(0, -1120));
			map[6]->moveTileMap(glm::vec2(0, -1680));
			map[7]->moveTileMap(glm::vec2(0, -2240));
			map[8]->moveTileMap(glm::vec2(0, -2800));
			map[9]->moveTileMap(glm::vec2(0, -3360));
			map[10]->moveTileMap(glm::vec2(0, -3920));
			map[11]->moveTileMap(glm::vec2(0, -4480));
			map[12]->moveTileMap(glm::vec2(0, -5040));
			break;
		case 4:
			map[0]->moveTileMap(glm::vec2(0, 2240));
			map[1]->moveTileMap(glm::vec2(0, 1680));
			map[2]->moveTileMap(glm::vec2(0, 1120));
			map[3]->moveTileMap(glm::vec2(0, 560));
			map[4]->moveTileMap(glm::vec2(0, 0));
			map[5]->moveTileMap(glm::vec2(0, -560));
			map[6]->moveTileMap(glm::vec2(0, -1120));
			map[7]->moveTileMap(glm::vec2(0, -1680));
			map[8]->moveTileMap(glm::vec2(0, -2240));
			map[9]->moveTileMap(glm::vec2(0, -2800));
			map[10]->moveTileMap(glm::vec2(0, -3360));
			map[11]->moveTileMap(glm::vec2(0, -3920));
			map[12]->moveTileMap(glm::vec2(0, -4480));
			break;
		case 5:
			map[0]->moveTileMap(glm::vec2(0, 2800));
			map[1]->moveTileMap(glm::vec2(0, 2240));
			map[2]->moveTileMap(glm::vec2(0, 1680));
			map[3]->moveTileMap(glm::vec2(0, 1120));
			map[4]->moveTileMap(glm::vec2(0, 560));
			map[5]->moveTileMap(glm::vec2(0, 0));
			map[6]->moveTileMap(glm::vec2(0, -560));
			map[7]->moveTileMap(glm::vec2(0, -1120));
			map[8]->moveTileMap(glm::vec2(0, -1680));
			map[8]->moveTileMap(glm::vec2(0, -2240));
			map[8]->moveTileMap(glm::vec2(0, -2800));
			map[8]->moveTileMap(glm::vec2(0, -3360));
			map[8]->moveTileMap(glm::vec2(0, -3920));
			break;
		case 6:
			map[0]->moveTileMap(glm::vec2(0, 3360));
			map[1]->moveTileMap(glm::vec2(0, 2800));
			map[2]->moveTileMap(glm::vec2(0, 2240));
			map[3]->moveTileMap(glm::vec2(0, 1680));
			map[4]->moveTileMap(glm::vec2(0, 1120));
			map[5]->moveTileMap(glm::vec2(0, 560));
			map[6]->moveTileMap(glm::vec2(0, 0));
			map[7]->moveTileMap(glm::vec2(0, -560));
			map[8]->moveTileMap(glm::vec2(0, -1120));
			map[9]->moveTileMap(glm::vec2(0, -1680));
			map[10]->moveTileMap(glm::vec2(0, -2240));
			map[11]->moveTileMap(glm::vec2(0, -2800));
			map[12]->moveTileMap(glm::vec2(0, -3360));
			break;
		case 7:
			map[0]->moveTileMap(glm::vec2(0, 3920));
			map[1]->moveTileMap(glm::vec2(0, 3360));
			map[2]->moveTileMap(glm::vec2(0, 2800));
			map[3]->moveTileMap(glm::vec2(0, 2240));
			map[4]->moveTileMap(glm::vec2(0, 1680));
			map[5]->moveTileMap(glm::vec2(0, 1120));
			map[6]->moveTileMap(glm::vec2(0, 560));
			map[7]->moveTileMap(glm::vec2(0, 0));
			map[8]->moveTileMap(glm::vec2(0, -560));
			map[9]->moveTileMap(glm::vec2(0, -1120));
			map[10]->moveTileMap(glm::vec2(0, -1680));
			map[11]->moveTileMap(glm::vec2(0, -2240));
			map[12]->moveTileMap(glm::vec2(0, -2800));
			break;
		case 8:
			map[0]->moveTileMap(glm::vec2(0, 4480));
			map[1]->moveTileMap(glm::vec2(0, 3920));
			map[2]->moveTileMap(glm::vec2(0, 3360));
			map[3]->moveTileMap(glm::vec2(0, 2800));
			map[4]->moveTileMap(glm::vec2(0, 2240));
			map[5]->moveTileMap(glm::vec2(0, 1680));
			map[6]->moveTileMap(glm::vec2(0, 1120));
			map[7]->moveTileMap(glm::vec2(0, 560));
			map[8]->moveTileMap(glm::vec2(0, 0));
			map[9]->moveTileMap(glm::vec2(0, -560));
			map[10]->moveTileMap(glm::vec2(0, -1120));
			map[11]->moveTileMap(glm::vec2(0, -1680));
			map[12]->moveTileMap(glm::vec2(0, -2240));
			break;
		case 9:
			map[0]->moveTileMap(glm::vec2(0, 5040));
			map[1]->moveTileMap(glm::vec2(0, 4480));
			map[2]->moveTileMap(glm::vec2(0, 3920));
			map[3]->moveTileMap(glm::vec2(0, 3360));
			map[4]->moveTileMap(glm::vec2(0, 2800));
			map[5]->moveTileMap(glm::vec2(0, 2240));
			map[6]->moveTileMap(glm::vec2(0, 1680));
			map[7]->moveTileMap(glm::vec2(0, 1120));
			map[8]->moveTileMap(glm::vec2(0, 560));
			map[9]->moveTileMap(glm::vec2(0, 0));
			map[10]->moveTileMap(glm::vec2(0, -560));
			map[11]->moveTileMap(glm::vec2(0, -1120));
			map[12]->moveTileMap(glm::vec2(0, -1680));
			break;
		case 10:
			map[0]->moveTileMap(glm::vec2(0, 5600));
			map[1]->moveTileMap(glm::vec2(0, 5040));
			map[2]->moveTileMap(glm::vec2(0, 4480));
			map[3]->moveTileMap(glm::vec2(0, 3920));
			map[4]->moveTileMap(glm::vec2(0, 3360));
			map[5]->moveTileMap(glm::vec2(0, 2800));
			map[6]->moveTileMap(glm::vec2(0, 2240));
			map[7]->moveTileMap(glm::vec2(0, 1680));
			map[8]->moveTileMap(glm::vec2(0, 1120));
			map[9]->moveTileMap(glm::vec2(0, 560));
			map[10]->moveTileMap(glm::vec2(0, 0));
			map[11]->moveTileMap(glm::vec2(0, -560));
			map[12]->moveTileMap(glm::vec2(0, -1120));
			break;
		case 11:
			map[0]->moveTileMap(glm::vec2(0, 6160));
			map[1]->moveTileMap(glm::vec2(0, 5600));
			map[2]->moveTileMap(glm::vec2(0, 5040));
			map[3]->moveTileMap(glm::vec2(0, 4480));
			map[4]->moveTileMap(glm::vec2(0, 3920));
			map[5]->moveTileMap(glm::vec2(0, 3360));
			map[6]->moveTileMap(glm::vec2(0, 2800));
			map[7]->moveTileMap(glm::vec2(0, 2240));
			map[8]->moveTileMap(glm::vec2(0, 1680));
			map[9]->moveTileMap(glm::vec2(0, 1120));
			map[10]->moveTileMap(glm::vec2(0, 560));
			map[11]->moveTileMap(glm::vec2(0, 0));
			map[12]->moveTileMap(glm::vec2(0, -560));
			break;
		case 12:
			map[0]->moveTileMap(glm::vec2(0, 6720));
			map[1]->moveTileMap(glm::vec2(0, 6160));
			map[2]->moveTileMap(glm::vec2(0, 5600));
			map[3]->moveTileMap(glm::vec2(0, 5040));
			map[4]->moveTileMap(glm::vec2(0, 4480));
			map[5]->moveTileMap(glm::vec2(0, 3920));
			map[6]->moveTileMap(glm::vec2(0, 3360));
			map[7]->moveTileMap(glm::vec2(0, 2800));
			map[8]->moveTileMap(glm::vec2(0, 2240));
			map[9]->moveTileMap(glm::vec2(0, 1680));
			map[10]->moveTileMap(glm::vec2(0, 1120));
			map[11]->moveTileMap(glm::vec2(0, 560));
			map[12]->moveTileMap(glm::vec2(0, 0));
			break;
	}
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map[0]->getTileSize(), INIT_PLAYER_Y_TILES * map[0]->getTileSize() / 2));
	ball->setPosition(glm::vec2(ball->getPosition().x, 0));
	player->setTileMap(map[currentRoom]);
	ball->setTileMap(map[currentRoom]);
}

void Scene::reloadMoney() {
	moneyStr.str(string());
	moneyStr << setfill('0') << setw(7) << Game::instance().getMoney() << endl;
}

void Scene::reloadPoints() {
	pointsStr.str(string());
	pointsStr << setfill('0') << setw(7) << Game::instance().getPoints() << endl;
}

void Scene::reloadLives() {
	livesStr.str(string());
	livesStr << setfill('0') << setw(2) << lives << endl;
}

void Scene::reloadRoom() {
	roomStr.str(string());
	roomStr << setfill('0') << setw(2) << currentRoom + 1 << endl;
}

void Scene::reloadBank() {
	bankStr.str(string());
	bankStr << setfill('0') << setw(2) << Game::instance().getCurrentBank() << endl;
}

void Scene::loseLife() {
	if (state != LOSE_LIFE) {
		state = LOSE_LIFE;
		lives--;
		reloadLives();
		ball->stop();
		player->stop();
		soundEngine->play2D("sounds/gameover.wav");
		loseTime = currentTime;
	}
}

void Scene::resetPlayer() {
	ball->reset(glm::vec2(INIT_PLAYER_X_TILES * map[0]->getTileSize() + 8, (INIT_PLAYER_Y_TILES - 1.3) * map[0]->getTileSize() / 2));
	player->reset(glm::vec2(INIT_PLAYER_X_TILES * map[0]->getTileSize(), INIT_PLAYER_Y_TILES * map[0]->getTileSize() / 2));
}

int Scene::getCurrentRoom() {
	return currentRoom;
}

void Scene::startBossFight() {
	soundEngine->stopAllSounds();
	state = BOSS_FIGHT;
	boss = new Boss();
	boss->setSoundEngine(soundEngine);
	boss->init(glm::ivec2(0, 0), texProgram);
	boss->setPosition(glm::vec2(180, -230));
	boss->move(glm::vec2(180,0));

	ball->setBoss(boss);
}

int Scene::getState() {
	return state;
}