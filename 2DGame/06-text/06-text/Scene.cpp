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
	PLAYING, GAME_OVER, GAME_WIN, LOSE_LIFE, BOSS_FIGHT, BOSS_INTRO, FINAL_ANIMATION
};

State state;

enum PlayerState {
	SMALL, LARGE, STICKY, BATGUN
};

PlayerState playerState;

Scene::Scene()
{
	player = NULL;
	map[0] = NULL;
	map[1] = NULL;
	map[2] = NULL;
}

Scene::~Scene()
{
	if (arrows != NULL)
		delete arrows;
	if (player != NULL)
		delete player;
}


void Scene::init()
{
	playerState = SMALL;
	soundEngine->stopAllSounds();
	soundEngine->play2D("sounds/backgroundMusicLow.wav", true);

	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(120.f, 20.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	largeTex.loadFromFile("images/large.png", TEXTURE_PIXEL_FORMAT_RGBA);
	largeTex.setMagFilter(GL_NEAREST);
	smallTex.loadFromFile("images/small.png", TEXTURE_PIXEL_FORMAT_RGBA);
	smallTex.setMagFilter(GL_NEAREST);
	batgunTex.loadFromFile("images/gunbat.png", TEXTURE_PIXEL_FORMAT_RGBA);
	batgunTex.setMagFilter(GL_NEAREST);
	stickyTex.loadFromFile("images/sticky.png", TEXTURE_PIXEL_FORMAT_RGBA);
	stickyTex.setMagFilter(GL_NEAREST);



	haveKey[0] = false;
	haveKey[1] = false;
	haveKey[2] = false;
	haveKey[3] = false;
	haveKey[4] = false;
	haveKey[5] = false;

	currentRoom = 0;
	lives = 4;

	dead = false;

	initShaders();

	powerupQuad = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	geom[1] = glm::vec2(160.f, 16.f);

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

	geom[1] = glm::vec2(400.f, 300.f);
	pointsTexture.loadFromFile("images/backgroundPoints.png", TEXTURE_PIXEL_FORMAT_RGBA);
	points = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	pointsTexture.setMagFilter(GL_NEAREST);



	currentBank = Game::instance().getCurrentBank();

	if (currentBank == 1) {
		map[0] = TileMap::createTileMap("levels/01-01.txt", glm::vec2(0, 0), texProgram);
		map[1] = TileMap::createTileMap("levels/01-02.txt", glm::vec2(0, -560), texProgram);
		map[2] = TileMap::createTileMap("levels/01-03.txt", glm::vec2(0, -1120), texProgram);
		map[3] = TileMap::createTileMap("levels/01-bonus.txt", glm::vec2(0, -1680), texProgram);
		map[4] = TileMap::createTileMap("levels/emptyLevel1.txt", glm::vec2(0, -2240), texProgram);
		map[5] = TileMap::createTileMap("levels/emptyLevel1.txt", glm::vec2(0, -2800), texProgram);
		currentMoney = 18;
	}

	else if (currentBank == 2) {
		map[0] = TileMap::createTileMap("levels/02-01.txt", glm::vec2(0, 0), texProgram);
		map[1] = TileMap::createTileMap("levels/02-02.txt", glm::vec2(0, -560), texProgram);
		map[2] = TileMap::createTileMap("levels/02-03.txt", glm::vec2(0, -1120), texProgram);
		map[3] = TileMap::createTileMap("levels/02-bonus.txt", glm::vec2(0, -1680), texProgram);
		map[4] = TileMap::createTileMap("levels/emptyLevel2.txt", glm::vec2(0, -2240), texProgram);
		map[5] = TileMap::createTileMap("levels/emptyLevel2.txt", glm::vec2(0, -2800), texProgram);
		currentMoney = 8;
	}

	else {
		map[0] = TileMap::createTileMap("levels/03-01.txt", glm::vec2(0, 0), texProgram);
		map[1] = TileMap::createTileMap("levels/03-02.txt", glm::vec2(0, -560), texProgram);
		map[2] = TileMap::createTileMap("levels/03-03.txt", glm::vec2(0, -1120), texProgram);
		map[3] = TileMap::createTileMap("levels/03-bonus.txt", glm::vec2(0, -1680), texProgram);
		map[4] = TileMap::createTileMap("levels/bossLevel.txt", glm::vec2(0, -2240), texProgram);
		map[5] = TileMap::createTileMap("levels/emptyLevel3.txt", glm::vec2(0, -2800), texProgram);
		currentMoney = 10;
	}

	map[0]->setShaderProgram(texProgram);
	map[1]->setShaderProgram(texProgram);
	map[2]->setShaderProgram(texProgram);
	map[3]->setShaderProgram(texProgram);
	map[4]->setShaderProgram(texProgram);
	map[5]->setShaderProgram(texProgram);


	map[0]->setSoundEngine(soundEngine);
	map[1]->setSoundEngine(soundEngine);
	map[2]->setSoundEngine(soundEngine);
	map[3]->setSoundEngine(soundEngine);
	map[4]->setSoundEngine(soundEngine);
	map[5]->setShaderProgram(texProgram);


	player = new Player();
	player->init(glm::ivec2(0, 0), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map[0]->getTileSize(), INIT_PLAYER_Y_TILES * map[0]->getTileSize() / 2));
	player->setTileMap(map[0]);
	player->setSoundEngine(soundEngine);

	ball = new Ball();
	ball->init(glm::ivec2(0, 0), texProgram);
	ball->setPosition(glm::ivec2(INIT_PLAYER_X_TILES * map[0]->getTileSize() + 9, (INIT_PLAYER_Y_TILES - 1.3) * map[0]->getTileSize() / 2));
	ball->setTileMap(map[0]);
	ball->setPlayer(player);

	vigilant = new Vigilant();
	vigilant->init(glm::ivec2(0, 0), texProgram);
	vigilant->setPosition(glm::ivec2(0, 0));
	vigilant->setPlayer(player);
	vigilant->setTileMap(map[0]);



	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	// Select which font you want to use
	if (!text.init("fonts/ARCADEPI.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;

	reloadMoney();
	reloadPoints();
	reloadLives();
	reloadBank();
	reloadRoom();

	powerupTimer = 0;
	powerupIsActive = false;

	powerActive = false;
	bossDefeated = false;

	balls = 1;
	startTripleBall = false;
}

void Scene::update(int deltaTime)
{
	powerupTimer += deltaTime;

	if (powerupTimer > 4000 && !powerupIsActive) {
		powerup = new Powerup();
		powerup->init(glm::vec2(0, 0), texProgram);
		powerup->setPosition(glm::ivec2(-30, 540));
		powerup->setPlayer(player);
		powerup->setBall(ball);
		powerupIsActive = true;
	}
	if (telefono) {
		ball->stop();
		if (balls > 1) {
			ball2->stop();
			if (balls > 2) ball3->stop();
		}
		player->stop();
		countPoints();
	}

	if (startTripleBall) {
		ball2 = new Ball();
		ball2->init(glm::ivec2(0, 0), texProgram);
		ball2->setSticky(false);
		ball2->setPosition(glm::vec2(ball->getPosition().x, ball->getPosition().y));
		ball2->setTileMap(map[currentRoom]);
		ball2->setPlayer(player);
		ball2->setDirection(glm::vec2(ball->getDirection().x + 0.2f, ball->getDirection().y));
		ball2->setSpeed(6);

		ball3 = new Ball();
		ball3->init(glm::ivec2(0, 0), texProgram);
		ball3->setSticky(false);
		ball3->setPosition(glm::vec2(ball->getPosition().x, ball->getPosition().y));
		ball3->setTileMap(map[currentRoom]);
		ball3->setPlayer(player);
		ball3->setDirection(glm::vec2(ball->getDirection().x - 0.2f, ball->getDirection().y));
		ball3->setSpeed(6);


		balls = 3;

		startTripleBall = false;
	}



	currentTime += deltaTime;
	player->update(deltaTime);
	if (state == FINAL_ANIMATION) {
		princess->update(deltaTime);
	}
	else {
		ball->update(deltaTime);
		if (balls > 1) {
			ball2->update(deltaTime);
			if (balls > 2) ball3->update(deltaTime);
		}
	}
	vigilant->update(deltaTime);
	if (powerupIsActive)
		powerup->update(deltaTime);

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
	map[5]->update(deltaTime);


	if (state == LOSE_LIFE) {
		if (loseTime + 1500 < currentTime) {
			if (dead) {
				Game::instance().startAction(5);
			}
			else if (currentRoom == 4) {
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
			if (!(currentRoom == 4 && currentBank == 3)) {
				nextRoom();
				ball->setDirection(glm::vec2(ball->getDirection().x, -abs(ball->getDirection().y)));
				balls = 1;
				delete ball2;
				delete ball3;
			}
			else nextRoom();
		}
	}
	else if (Game::instance().getKey('o')) {
		Game::instance().keyReleased('o');
		if (currentRoom > 0) {
			previousRoom();
			ball->setDirection(glm::vec2(ball->getDirection().x, abs(ball->getDirection().y)));
			balls = 1;
			delete ball2;
			delete ball3;
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
				if (firstPhase)
					simpleProgram.setUniform4f("color", 1.f, 0.5f, 0.f, 1.f);
				else
					simpleProgram.setUniform4f("color", 1.f, 0.f, 0.f, 1.f);
			else if (bossLife == i - 1 && boss->getHitting()) {
				simpleProgram.setUniform4f("color", 1.f, 1.f, 1.f, 1.f);
			}
			else {
				simpleProgram.setUniform4f("color", 0.f, 0.f, 0.f, 1.f);
			}
			modelview = glm::translate(glm::mat4(1.f), glm::vec3(44.f*i - 22.f, 15.f, 0.f));
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

	if (currentRoom < 4 || currentBank == 3) mesh->render(meshTexture);
	else endLevel->render(endLevelTexture);
	map[0]->render();
	map[1]->render();
	map[2]->render();
	map[3]->render();
	map[4]->render();
	map[5]->render();


	player->render();
	if (state == FINAL_ANIMATION) {
		princess->render();
	}
	else {
		ball->render();
		if (balls > 1 && !startTripleBall) {
			ball2->render();
			if (balls > 2 && !startTripleBall) ball3->render();
		}
	}

	if (powerupIsActive)
		powerup->render();
	if (vigilant->getActivo()) {
		vigilant->render();
	}

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

	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(500.f, 400.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	switch (playerState) {
	case SMALL:
		powerupQuad->render(smallTex);
		break;
	case LARGE:
		powerupQuad->render(largeTex);
		break;
	case BATGUN:
		powerupQuad->render(batgunTex);
		break;
	case STICKY:
		powerupQuad->render(stickyTex);
		break;
	}


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

	if (telefono) {
		if (Game::instance().getPoints() > 0) {
			texProgram.use();
			texProgram.setUniformMatrix4f("projection", projection);
			texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

			modelview = glm::translate(glm::mat4(1.0f), glm::vec3(map[0]->getTileSize(), 4 * map[0]->getTileSize(), 0.f));
			texProgram.setUniformMatrix4f("modelview", modelview);
			texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
			points->render(pointsTexture);

			text.render(moneyStr.str(), glm::vec2(map[0]->getTileSize() + 120, 7 * map[0]->getTileSize() + 10), 32, glm::vec4(0, 1, 0, 1));
			text.render(pointsStr.str(), glm::vec2(map[0]->getTileSize() + 120, 9 * map[0]->getTileSize()), 32, glm::vec4(0, 1, 0, 1));

		}
		else {
			telefono = false;
			ball->setSpeed(6);
			player->setSpeed(2);
			if (balls > 1) {
				ball2->setSpeed(6);
				if (balls > 2) ball3->setSpeed(6);
			}
		}
	}

}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/simple.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/simple.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleProgram.init();
	simpleProgram.addShader(vShader);
	simpleProgram.addShader(fShader);
	simpleProgram.link();
	if (!simpleProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleProgram.log() << endl << endl;
	}
	simpleProgram.bindFragmentOutput("outColor");

	vShader.free();
	fShader.free();
	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
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
	playerState = SMALL;
	currentRoom++;
	stopPowerUps();
	switch (currentRoom) {
	case 1:
		map[0]->moveTileMap(glm::vec2(0, 560));
		map[1]->moveTileMap(glm::vec2(0, 0));
		map[2]->moveTileMap(glm::vec2(0, -560));
		map[3]->moveTileMap(glm::vec2(0, -1120));
		map[4]->moveTileMap(glm::vec2(0, -1680));
		map[5]->moveTileMap(glm::vec2(0, -2240));
		break;
	case 2:
		map[0]->moveTileMap(glm::vec2(0, 1120));
		map[1]->moveTileMap(glm::vec2(0, 560));
		map[2]->moveTileMap(glm::vec2(0, 0));
		map[3]->moveTileMap(glm::vec2(0, -560));
		map[4]->moveTileMap(glm::vec2(0, -1120));
		map[5]->moveTileMap(glm::vec2(0, -1680));
		break;
	case 3:
		map[0]->moveTileMap(glm::vec2(0, 1680));
		map[1]->moveTileMap(glm::vec2(0, 1120));
		map[2]->moveTileMap(glm::vec2(0, 560));
		map[3]->moveTileMap(glm::vec2(0, 0));
		map[4]->moveTileMap(glm::vec2(0, -560));
		map[5]->moveTileMap(glm::vec2(0, -1120));
		break;
	case 4:
		map[0]->moveTileMap(glm::vec2(0, 2240));
		map[1]->moveTileMap(glm::vec2(0, 1680));
		map[2]->moveTileMap(glm::vec2(0, 1120));
		map[3]->moveTileMap(glm::vec2(0, 560));
		map[4]->moveTileMap(glm::vec2(0, 0));
		map[5]->moveTileMap(glm::vec2(0, -560));
		if (currentBank < 3) startAnimFinalBank();
		else {
			if (!bossDefeated) startBossFight();
			glm::vec2 geom[2] = { glm::vec2(0.f, 40.f), glm::vec2(480.f, 480.f) };
			glm::vec2 texCoords[2] = { glm::vec2(0.f, 40.f / 480.f), glm::vec2(1.f, 1.f) };
			mesh = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
		}
		break;
	case 5:
		if (currentBank < 3)	Game::instance().nextLevel();
		else {
			map[0]->moveTileMap(glm::vec2(0, 2800));
			map[1]->moveTileMap(glm::vec2(0, 2240));
			map[2]->moveTileMap(glm::vec2(0, 1680));
			map[3]->moveTileMap(glm::vec2(0, 1120));
			map[4]->moveTileMap(glm::vec2(0, 560));
			map[5]->moveTileMap(glm::vec2(0, 0));
			startAnimFinalGame();

			glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
			glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(160.f, 16.f) };
			geom[1] = glm::vec2(480.f, 480.f);

			mesh = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
		}
		break;
	}
	player->setTileMap(map[currentRoom]);

	player->setPosition(glm::ivec2(INIT_PLAYER_X_TILES * map[0]->getTileSize(), INIT_PLAYER_Y_TILES * map[0]->getTileSize() / 2));
	if (ball->getSticky() || currentRoom == 4) {
		ball->reset(glm::ivec2(INIT_PLAYER_X_TILES*map[0]->getTileSize() + 9, (INIT_PLAYER_Y_TILES - 1.3) * map[0]->getTileSize() / 2));
	}
	else {
		if (balls == 2) {
			balls = 1;
			if (ball->getPosition().y > ball2->getPosition().y) {
				ball = ball2;

			}
			ball->setDeadBall(false);
			ball2 = NULL;
			delete ball2;
		}
		if (balls == 3) {
			balls = 1;
			if (ball->getPosition().y > ball2->getPosition().y) {
				ball = ball2;
				if (ball->getPosition().y > ball3->getPosition().y) {
					ball = ball3;
				}
				ball->setDeadBall(false);
				ball2 = NULL;
				delete ball2;
				ball3 = NULL;
				delete ball3;
			}

		}
		ball->setPosition(glm::ivec2(ball->getPosition().x, 480));
	}
	ball->setTileMap(map[currentRoom]);

	reloadRoom();
}

void Scene::previousRoom() {
	playerState = SMALL;
	currentRoom--;
	stopPowerUps();
	switch (currentRoom) {
	case 0:
		map[0]->moveTileMap(glm::vec2(0, 0));
		map[1]->moveTileMap(glm::vec2(0, -560));
		map[2]->moveTileMap(glm::vec2(0, -1120));
		map[3]->moveTileMap(glm::vec2(0, -1680));
		map[4]->moveTileMap(glm::vec2(0, -2240));
		map[5]->moveTileMap(glm::vec2(0, -2800));
		break;
	case 1:
		map[0]->moveTileMap(glm::vec2(0, 560));
		map[1]->moveTileMap(glm::vec2(0, 0));
		map[2]->moveTileMap(glm::vec2(0, -560));
		map[3]->moveTileMap(glm::vec2(0, -1120));
		map[4]->moveTileMap(glm::vec2(0, -1680));
		map[5]->moveTileMap(glm::vec2(0, -2240));
		break;
	case 2:
		map[0]->moveTileMap(glm::vec2(0, 1120));
		map[1]->moveTileMap(glm::vec2(0, 560));
		map[2]->moveTileMap(glm::vec2(0, 0));
		map[3]->moveTileMap(glm::vec2(0, -560));
		map[4]->moveTileMap(glm::vec2(0, -1120));
		map[5]->moveTileMap(glm::vec2(0, -1680));
		break;
	case 3:
		map[0]->moveTileMap(glm::vec2(0, 1680));
		map[1]->moveTileMap(glm::vec2(0, 1120));
		map[2]->moveTileMap(glm::vec2(0, 560));
		map[3]->moveTileMap(glm::vec2(0, 0));
		map[4]->moveTileMap(glm::vec2(0, -560));
		map[5]->moveTileMap(glm::vec2(0, -1120));
		if (currentBank == 3) {
			glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
			glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(160.f, 16.f) };
			geom[1] = glm::vec2(480.f, 480.f);

			mesh = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

			endBossFight();
		}
		else {
			resetPlayer();
			ball->setAnimationBall(false);
		}
		break;
	case 4:
		map[0]->moveTileMap(glm::vec2(0, 2240));
		map[1]->moveTileMap(glm::vec2(0, 1680));
		map[2]->moveTileMap(glm::vec2(0, 1120));
		map[3]->moveTileMap(glm::vec2(0, 560));
		map[4]->moveTileMap(glm::vec2(0, 0));
		map[5]->moveTileMap(glm::vec2(0, -560));
		if (currentBank == 3) {
			if (!bossDefeated) startBossFight();
			glm::vec2 geom[2] = { glm::vec2(0.f, 40.f), glm::vec2(480.f, 480.f) };
			glm::vec2 texCoords[2] = { glm::vec2(0.f, 40.f / 480.f), glm::vec2(1.f, 1.f) };
			mesh = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
		}

		break;
	case 5:
		map[0]->moveTileMap(glm::vec2(0, 2800));
		map[1]->moveTileMap(glm::vec2(0, 2240));
		map[2]->moveTileMap(glm::vec2(0, 1680));
		map[3]->moveTileMap(glm::vec2(0, 1120));
		map[4]->moveTileMap(glm::vec2(0, 560));
		map[5]->moveTileMap(glm::vec2(0, 0));

	}
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map[0]->getTileSize(), INIT_PLAYER_Y_TILES * map[0]->getTileSize() / 2));
	player->setTileMap(map[currentRoom]);

	if (ball->getSticky()) {
		ball->reset(glm::ivec2(INIT_PLAYER_X_TILES*map[0]->getTileSize() + 9, (INIT_PLAYER_Y_TILES - 1.3) * map[0]->getTileSize() / 2));
	}
	else {
		ball->setPosition(glm::vec2(ball->getPosition().x, 0));
		if (balls == 2) {
			balls = 1;
			if (ball->getPosition().y > ball2->getPosition().y) {
				ball = ball2;

			}
			ball->setDeadBall(false);
			ball2 = NULL;
			delete ball2;
		}
		else if (balls == 3) {
			balls = 1;
			if (ball->getPosition().y > ball2->getPosition().y) {
				ball = ball2;
				if (ball->getPosition().y > ball3->getPosition().y) {
					ball = ball3;
				}
				ball->setDeadBall(false);
				ball2 = NULL;
				delete ball2;
				ball3 = NULL;
				delete ball3;
			}

		}

	}
	
	ball->setTileMap(map[currentRoom]);

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
	if (state != LOSE_LIFE && !Game::instance().getGodMode()) {
		if (state == BOSS_FIGHT)
			stopBossShooting();
		state = LOSE_LIFE;
		if (lives > 0)lives--;
		else dead = true;
		reloadLives();
		vigilant->reset();
		ball->stop();
		ball->setDeadBall(false);
		player->die();
		stopPowerUps();
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
	resetPlayer();
	player->stop();
	startBossTime = currentTime;
	boss = new Boss();
	boss->setSoundEngine(soundEngine);
	boss->init(glm::ivec2(0, 0), texProgram);
	boss->setPosition(glm::vec2(180, -230));
	boss->move(glm::vec2(180, 40));

	ball->setBoss(boss);

}

void Scene::endBossFight() {
	soundEngine->stopAllSounds();
	soundEngine->play2D("sounds/backgroundMusicLow.wav", true);
	resetPlayer();
	state = PLAYING;
}

int Scene::getState() {
	return state;
}

void Scene::startAnimFinalBank() {

	//ball->stop();
	ball->reset(glm::vec2(INIT_PLAYER_X_TILES * map[0]->getTileSize() + 8, (INIT_PLAYER_Y_TILES - 1.3) * map[0]->getTileSize() / 2));
	player->reset(glm::vec2(INIT_PLAYER_X_TILES * map[0]->getTileSize(), INIT_PLAYER_Y_TILES * map[0]->getTileSize() / 2));
	player->setAnimationPlayer();
	ball->setAnimationBall(true);
}

void Scene::startAnimFinalGame() {
	ball->stop();
	player->reset(glm::vec2(INIT_PLAYER_X_TILES * map[0]->getTileSize(), INIT_PLAYER_Y_TILES * map[0]->getTileSize() / 2));
	player->setAnimationPlayer();
	princess = new Princess();
	princess->init(glm::ivec2(0, 0), texProgram);
	princess->setTileMap(map[currentRoom]);
	princess->setPlayer(player);
	princess->setAnimation(true);
	princess->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map[0]->getTileSize(), ((24 - INIT_PLAYER_Y_TILES) * map[0]->getTileSize() / 2)));
	state = FINAL_ANIMATION;
}


void Scene::win() {
	bossDefeated = true;
	map[currentRoom]->openBossExit();
	state = GAME_WIN;
}

Player* Scene::getPlayer() {
	return player;
}

TileMap* Scene::getTileMap() {
	return map[currentRoom];
}

void Scene::stopBossShooting() {
	boss->stopShooting();
}
void Scene::alarmOn() {
	vigilant->alarm();
}

void Scene::alarmOff() {
	vigilant->reset();
}

void Scene::catchPowerup(int color) {
	switch (color) {
	case 1:
		playerState = BATGUN;
		break;
	case 2:
		playerState = STICKY;
		break;
	case 3:
		playerState = LARGE;
		break;
	case 4:
	case 5:
		playerState = SMALL;
		break;
	}
	powerupIsActive = false;
	powerupTimer = 0;
	powerActive = true;
}

void Scene::stopPowerUps() {
	powerActive = false;
	player->setLargePlayer(false);
	player->setStickyPlayer(false);
	ball->setPowerUpSticky(false);
	player->setFirePlayer(false);
	player->setShotCreated(false);
}

void Scene::tripleBall() {
	balls = 3;
	startTripleBall = true;
}

void Scene::loseBall() {
	--balls;
	if (balls < 1) {
		loseLife();
		balls = 1;
	}
	else if (balls == 1) {
		ball2 = NULL;
		delete ball2;
		ball3 = NULL;
		delete ball3;
	}
	else if (balls == 2) {
		if (ball->getPosition().y > 450) {
			ball = ball2;
			ball2 = ball3;
		}
		else if (ball2->getPosition().y > 450) {
			ball2 = ball3;

		}
		ball->setDeadBall(false);
		ball2->setDeadBall(false);
		ball3 = NULL;
		delete ball3;
	}
}

int Scene::getNumberBalls() {
	return balls;
}

void Scene::setTelefono() {
	telefono = true;
}

void Scene::countPoints() {

	if (Game::instance().getPoints() > 0) {
		Game::instance().setMoney(Game::instance().getMoney() + 100);
		Game::instance().setPoints(Game::instance().getPoints() - 100);
	}
	reloadMoney();
	reloadPoints();
}

void Scene::setShotCreated(bool created) {
	player->setShotCreated(created);
}

void Scene::gainMoney() {
	currentMoney--;
	if (currentMoney == 0) nextRoom();
}