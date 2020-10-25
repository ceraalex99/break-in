#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define INIT_PLAYER_X_TILES 5
#define INIT_PLAYER_Y_TILES 20

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
	soundEngine->play2D("sounds/backgroundMusic.wav", true);

	glm::vec2 geom[2] = {glm::vec2(0.f, 0.f), glm::vec2(160.f, 16.f)};
	glm::vec2 texCoords[2] = {glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f)};
	haveKey[0] = false;
	haveKey[1] = false;
	haveKey[2] = false;
	currentRoom = 0;

	

	initShaders();
	
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f);
	arrows = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	arrowsTexture.loadFromFile("images/arrows.png", TEXTURE_PIXEL_FORMAT_RGBA);
	arrowsTexture.setMagFilter(GL_NEAREST);

	geom[1] = glm::vec2(480.f, 480.f);
	mesh = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	meshTexture.loadFromFile("images/mesh.png", TEXTURE_PIXEL_FORMAT_RGBA);
	meshTexture.setMagFilter(GL_NEAREST);

	map[0] = TileMap::createTileMap("levels/01-01.txt", glm::vec2(0, 0), texProgram);
	map[1] = TileMap::createTileMap("levels/01-02.txt", glm::vec2(0, -560), texProgram);
	map[2] = TileMap::createTileMap("levels/01-03.txt", glm::vec2(0, -1120), texProgram);
	player = new Player();
	player->init(glm::ivec2(0, 0), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map[0]->getTileSize(), INIT_PLAYER_Y_TILES * map[0]->getTileSize() / 2));
	player->setTileMap(map[0]);

	ball = new Ball();
	ball->init(glm::ivec2(0, 0), texProgram);
	ball->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map[0]->getTileSize()+8, (INIT_PLAYER_Y_TILES-1) * map[0]->getTileSize() / 2));
	ball->setTileMap(map[0]);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	
	// Select which font you want to use
	if(!text.init("fonts/ARCADEPI.ttf"))
	//if(!text.init("fonts/OpenSans-Bold.ttf"))
	//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;

}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	ball->update(deltaTime);

	if (Game::instance().getKey('p')) {
		Game::instance().keyReleased('p');
		if (currentRoom < 2) {
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
	player->render();
	ball->render();

	text.render("MONEY:", glm::vec2(528, 22), 22, glm::vec4(0.28, 0.39, 0.84, 1));
	text.render("0000000", glm::vec2(500, 44), 22, glm::vec4(0, 1, 0, 1));
	text.render("POINTS:", glm::vec2(512, 120), 22, glm::vec4(0.28, 0.39, 0.84, 1));
	text.render("0000000", glm::vec2(500, 142), 22, glm::vec4(0, 1, 0, 1));
	text.render("LIVES:", glm::vec2(526, 240), 22, glm::vec4(0.28, 0.39, 0.84, 1));
	text.render("04", glm::vec2(585, 264), 22, glm::vec4(0, 1, 0, 1));
	text.render("BANK:", glm::vec2(540, 320), 22, glm::vec4(0.28, 0.39, 0.84, 1));
	text.render("01", glm::vec2(585, 344), 22, glm::vec4(0, 1, 0, 1));
	text.render("BATMODE", glm::vec2(500, 390), 22, glm::vec4(0.28, 0.39, 0.84, 1));
	text.render("SMALL", glm::vec2(517, 414), 22, glm::vec4(0.28, 0.39, 0.84, 1));
	text.render("ROOM:", glm::vec2(540, 450), 22, glm::vec4(0.28, 0.39, 0.84, 1));
	text.render("01", glm::vec2(585, 474), 22, glm::vec4(0, 1, 0, 1));
	
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
	map[currentRoom]->openExit(texProgram);
}

void Scene::nextRoom() {
	currentRoom++;
	switch (currentRoom) {
		case 1:
			map[0]->moveTileMap(glm::vec2(0, 560), texProgram);
			map[1]->moveTileMap(glm::vec2(0, 0), texProgram);
			map[2]->moveTileMap(glm::vec2(0, -560), texProgram);
			break;
		case 2:
			map[0]->moveTileMap(glm::vec2(0, 1120), texProgram);
			map[1]->moveTileMap(glm::vec2(0, 560), texProgram);
			map[2]->moveTileMap(glm::vec2(0, 0), texProgram);
			break;
	}
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map[0]->getTileSize(), INIT_PLAYER_Y_TILES * map[0]->getTileSize() / 2));
	ball->setPosition(glm::vec2(ball->getPosition().x, 480));
	player->setTileMap(map[currentRoom]);
	ball->setTileMap(map[currentRoom]);
}

void Scene::previousRoom() {
	currentRoom--;
	switch (currentRoom) {
	case 0:
		map[0]->moveTileMap(glm::vec2(0, 0), texProgram);
		map[1]->moveTileMap(glm::vec2(0, -560), texProgram);
		map[2]->moveTileMap(glm::vec2(0, -1120), texProgram);
		break;
	case 1:
		map[0]->moveTileMap(glm::vec2(0, 560), texProgram);
		map[1]->moveTileMap(glm::vec2(0, 0), texProgram);
		map[2]->moveTileMap(glm::vec2(0, -560), texProgram);
		break;
	}
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map[0]->getTileSize(), INIT_PLAYER_Y_TILES * map[0]->getTileSize() / 2));
	ball->setPosition(glm::vec2(ball->getPosition().x, 0));
	player->setTileMap(map[currentRoom]);
	ball->setTileMap(map[currentRoom]);
}