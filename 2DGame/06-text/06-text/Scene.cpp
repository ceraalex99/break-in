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
	PLAYING, GAME_OVER, GAME_WIN, LOSE_LIFE, BOSS_FIGHT, BOSS_INTRO
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

	currentRoom = 0;
	lives = 4;
	

	initShaders();
	
	blackBackground = Quad::createQuad(0.f, 0.f, 640.f, 480.f, simpleProgram);
	bossLifeBar = Quad::createQuad(0.f, 0.f, 44.f, 10.f, simpleProgram);
	
	arrows = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	arrowsTexture.loadFromFile("images/arrows.png", TEXTURE_PIXEL_FORMAT_RGBA);
	arrowsTexture.setMagFilter(GL_NEAREST);

	geom[1] = glm::vec2(480.f, 480.f);
	mesh = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	meshTexture.loadFromFile("images/mesh.png", TEXTURE_PIXEL_FORMAT_RGBA);
	meshTexture.setMagFilter(GL_NEAREST);

	geom[1] = glm::vec2(480.f, 480.f);
	endLevel = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	endLevelTexture.loadFromFile("images/endLevel.png", TEXTURE_PIXEL_FORMAT_RGBA);
	endLevelTexture.setMagFilter(GL_NEAREST);

	geom[1] = glm::vec2(160.f, 480.f);
	lettersTexture.loadFromFile("images/letters.png", TEXTURE_PIXEL_FORMAT_RGBA);
	letters = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	lettersTexture.setMagFilter(GL_NEAREST);



	currentBank = Game::instance().getCurrentBank();

	if (currentBank == 1) {
		map[0] = TileMap::createTileMap("levels/01-01.txt", glm::vec2(0, 0), texProgram);
		map[1] = TileMap::createTileMap("levels/01-02.txt", glm::vec2(0, -560), texProgram);
		map[2] = TileMap::createTileMap("levels/01-03.txt", glm::vec2(0, -1120), texProgram);
		map[3] = TileMap::createTileMap("levels/01-bonus.txt", glm::vec2(0, -1680), texProgram);
		map[4] = TileMap::createTileMap("levels/emptyLevel1.txt", glm::vec2(0, -2240), texProgram);
	}

	else if (currentBank == 2) {
		map[0] = TileMap::createTileMap("levels/02-01.txt", glm::vec2(0, 0), texProgram);
		map[1] = TileMap::createTileMap("levels/02-02.txt", glm::vec2(0, -560), texProgram);
		map[2] = TileMap::createTileMap("levels/02-03.txt", glm::vec2(0, -1120), texProgram);
		map[3] = TileMap::createTileMap("levels/02-bonus.txt", glm::vec2(0, -1680), texProgram);
		map[4] = TileMap::createTileMap("levels/emptyLevel2.txt", glm::vec2(0, -2240), texProgram);
	}
	
	else {
		map[0] = TileMap::createTileMap("levels/03-01.txt", glm::vec2(0, 0), texProgram);
		map[1] = TileMap::createTileMap("levels/03-02.txt", glm::vec2(0, -560), texProgram);
		map[2] = TileMap::createTileMap("levels/03-03.txt", glm::vec2(0, -1120), texProgram);
		map[3] = TileMap::createTileMap("levels/03-bonus.txt", glm::vec2(0, -1680), texProgram);
		map[4] = TileMap::createTileMap("levels/bossLevel.txt", glm::vec2(0, -2240), texProgram);
	}

	map[0]->setShaderProgram(texProgram);
	map[1]->setShaderProgram(texProgram);
	map[2]->setShaderProgram(texProgram);
	map[3]->setShaderProgram(texProgram);
	map[4]->setShaderProgram(texProgram);
	
	map[0]->setSoundEngine(soundEngine);
	map[1]->setSoundEngine(soundEngine);
	map[2]->setSoundEngine(soundEngine);
	map[3]->setSoundEngine(soundEngine);
	map[4]->setSoundEngine(soundEngine);
	
	player = new Player();
	player->init(glm::ivec2(0, 0), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map[0]->getTileSize(), INIT_PLAYER_Y_TILES * map[0]->getTileSize() / 2));
	player->setTileMap(map[0]);
	
	ball = new Ball();
	ball->init(glm::ivec2(0, 0), texProgram);
	ball->setPosition(glm::ivec2(INIT_PLAYER_X_TILES * map[0]->getTileSize()+9, (INIT_PLAYER_Y_TILES-1.3) * map[0]->getTileSize() / 2));
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
	else if (state == BOSS_INTRO) {
		ball->stop();
		if (startBossTime + 4000 < currentTime) {
			state = BOSS_FIGHT;
			resetPlayer();
		}
		boss->update(deltaTime);
	}
	map[0]->update(deltaTime);
	map[1]->update(deltaTime);
	map[2]->update(deltaTime);
	map[3]->update(deltaTime);
	map[4]->update(deltaTime);

	if (state == LOSE_LIFE) {
		if (loseTime + 2000 < currentTime) {
			if (currentRoom == 4) {
				state = BOSS_FIGHT;
			}
			else
				state = PLAYING;
			resetPlayer();
		}
	}

	if (Game::instance().getKey('p')) {
		Game::instance().keyReleased('p');
		if (currentRoom < 5) {
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

	simpleProgram.use();
	simpleProgram.setUniformMatrix4f("projection", projection);
	simpleProgram.setUniform4f("color", 0.0f, 0.0f, 0.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	simpleProgram.setUniformMatrix4f("modelview", modelview);

	blackBackground->render();
	if (currentRoom == 4 && currentBank == 3 && boss->getLife() != 0) {
		int bossLife = boss->getLife();
		bool firstPhase;
		if (bossLife > 10) {
			bossLife -= 10;
			firstPhase = true;
		}
		else
			firstPhase = false;
		for (int i = 1; i <= 10; ++i) {
			if (bossLife >= i)
				if(firstPhase)
					simpleProgram.setUniform4f("color", 1.f, 0.5f, 0.f, 1.f);
				else
					simpleProgram.setUniform4f("color", 1.f, 0.f, 0.f, 1.f);
			else if(bossLife == i-1  && boss->getHitting()) {
				simpleProgram.setUniform4f("color", 1.f, 1.f, 1.f, 1.f);
			}
			else {
				simpleProgram.setUniform4f("color", 0.f, 0.f, 0.f, 1.f);
			}
			modelview = glm::translate(glm::mat4(1.f), glm::vec3(44.f*i-22.f, 15.f, 0.f));
			simpleProgram.setUniformMatrix4f("modelview", modelview);
			bossLifeBar->render();
		}
	}
	

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	
	if(currentRoom < 4 || currentBank == 3) mesh->render(meshTexture);
	else endLevel->render(endLevelTexture);
	map[0]->render();
	map[1]->render();
	map[2]->render();
	map[3]->render();
	map[4]->render();
	
	player->render();
	ball->render();
	if (currentRoom == 4 && (state == BOSS_FIGHT || state == BOSS_INTRO || state == LOSE_LIFE)) {
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
			break;
		case 2:
			map[0]->moveTileMap(glm::vec2(0, 1120));
			map[1]->moveTileMap(glm::vec2(0, 560));
			map[2]->moveTileMap(glm::vec2(0, 0));
			map[3]->moveTileMap(glm::vec2(0, -560));
			map[4]->moveTileMap(glm::vec2(0, -1120));
			break;
		case 3:
			map[0]->moveTileMap(glm::vec2(0, 1680));
			map[1]->moveTileMap(glm::vec2(0, 1120));
			map[2]->moveTileMap(glm::vec2(0, 560));
			map[3]->moveTileMap(glm::vec2(0, 0));
			map[4]->moveTileMap(glm::vec2(0, -560));
			break;
		case 4:
			map[0]->moveTileMap(glm::vec2(0, 2240));
			map[1]->moveTileMap(glm::vec2(0, 1680));
			map[2]->moveTileMap(glm::vec2(0, 1120));
			map[3]->moveTileMap(glm::vec2(0, 560));
			map[4]->moveTileMap(glm::vec2(0, 0));
			if(currentBank < 3) startAnim();
			else {
				startBossFight();
				glm::vec2 geom[2] = { glm::vec2(0.f, 40.f), glm::vec2(480.f, 480.f) };
				glm::vec2 texCoords[2] = { glm::vec2(0.f, 40.f / 480.f), glm::vec2(1.f, 1.f) };
				mesh = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
			}
			break;
		case 5:
			Game::instance().nextLevel();
			break;
	}
	player->setTileMap(map[currentRoom]);
	ball->setTileMap(map[currentRoom]);
	player->setPosition(glm::ivec2(INIT_PLAYER_X_TILES * map[0]->getTileSize(), INIT_PLAYER_Y_TILES * map[0]->getTileSize() / 2));
	if (ball->getSticky() || currentRoom == 4) {
		ball->reset(glm::ivec2(INIT_PLAYER_X_TILES*map[0]->getTileSize()+9, (INIT_PLAYER_Y_TILES-1.3) * map[0]->getTileSize() / 2));
	}
	else {
		ball->setPosition(glm::ivec2(ball->getPosition().x, 480));
	}
	
	reloadRoom();
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
			break;
		case 1:
			map[0]->moveTileMap(glm::vec2(0, 560));
			map[1]->moveTileMap(glm::vec2(0, 0));
			map[2]->moveTileMap(glm::vec2(0, -560));
			map[3]->moveTileMap(glm::vec2(0, -1120));
			map[4]->moveTileMap(glm::vec2(0, -1680));
			break;
		case 2:
			map[0]->moveTileMap(glm::vec2(0, 1120));
			map[1]->moveTileMap(glm::vec2(0, 560));
			map[2]->moveTileMap(glm::vec2(0, 0));
			map[3]->moveTileMap(glm::vec2(0, -560));
			map[4]->moveTileMap(glm::vec2(0, -1120));
			break;
		case 3:
			map[0]->moveTileMap(glm::vec2(0, 1680));
			map[1]->moveTileMap(glm::vec2(0, 1120));
			map[2]->moveTileMap(glm::vec2(0, 560));
			map[3]->moveTileMap(glm::vec2(0, 0));
			map[4]->moveTileMap(glm::vec2(0, -560));
			if (currentBank < 3) startAnim();
			else {
				startBossFight();
				glm::vec2 geom[2] = { glm::vec2(0.f, 40.f), glm::vec2(480.f, 480.f) };
				glm::vec2 texCoords[2] = { glm::vec2(0.f, 40.f / 480.f), glm::vec2(1.f, 1.f) };
				mesh = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
			}
			break;
			break;
		case 4:
			map[0]->moveTileMap(glm::vec2(0, 2240));
			map[1]->moveTileMap(glm::vec2(0, 1680));
			map[2]->moveTileMap(glm::vec2(0, 1120));
			map[3]->moveTileMap(glm::vec2(0, 560));
			map[4]->moveTileMap(glm::vec2(0, 0));
			break;
	}
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map[0]->getTileSize(), INIT_PLAYER_Y_TILES * map[0]->getTileSize() / 2));
	player->setTileMap(map[currentRoom]);
	ball->setTileMap(map[currentRoom]);
	if (ball->getSticky()) {
		ball->reset(glm::ivec2(INIT_PLAYER_X_TILES*map[0]->getTileSize() + 9, (INIT_PLAYER_Y_TILES - 1.3) * map[0]->getTileSize() / 2));
	}
	else {
		ball->setPosition(glm::vec2(ball->getPosition().x, 0));
	}

	reloadRoom();
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
	if (currentRoom == 4) {
		roomStr << "??" << endl;
	}
	else {
		roomStr << setfill('0') << setw(2) << currentRoom + 1 << endl;
	}
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
	state = BOSS_INTRO;
	player->stop();
	startBossTime = currentTime;
	boss = new Boss();
	boss->setSoundEngine(soundEngine);
	boss->init(glm::ivec2(0, 0), texProgram);
	boss->setPosition(glm::vec2(180, -230));
	boss->move(glm::vec2(180,40));

	ball->setBoss(boss);

}

int Scene::getState() {
	return state;
}

void Scene::startAnim() {
	//ball->stop();
	ball->reset(glm::vec2(INIT_PLAYER_X_TILES * map[0]->getTileSize() + 8, (INIT_PLAYER_Y_TILES - 1.3) * map[0]->getTileSize() / 2));
	player->reset(glm::vec2(INIT_PLAYER_X_TILES * map[0]->getTileSize(), INIT_PLAYER_Y_TILES * map[0]->getTileSize() / 2));
	player->setAnimationPlayer();
	ball->setAnimationBall();
}
void Scene::win(){
	state = GAME_WIN;
}
