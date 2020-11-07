#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"
#include <irrKlang.h>
#include "Game.h"


using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);

	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	finalPos = minCoords;
	currentPos = minCoords;
	prepareArrays(minCoords, program);
	alarmOn = false;
	tiempo = 0;
}

TileMap::~TileMap()
{
	if (map != NULL)
		delete map;
}

void TileMap::update(int deltaTime) {
	if (currentPos.y < finalPos.y) {
		currentPos.y += 35;
		prepareArrays(currentPos, texProgram);
	}
	else if (currentPos.y > finalPos.y) {
		currentPos.y -= 35;
		prepareArrays(currentPos, texProgram);
	}

	if (alarmOn) {
		if (tiempo > 1) {
			if (map[posicionAlarma1] == 20) {
				map[posicionAlarma1] = 28;
				map[posicionAlarma2] = 32;
				prepareArrays(currentPos, texProgram);
			}
			else if (map[posicionAlarma1] == 24) {
				map[posicionAlarma1] = 32;
				map[posicionAlarma2] = 28;
				prepareArrays(currentPos, texProgram);
			}
			else if (map[posicionAlarma1] == 28) {
				map[posicionAlarma1] = 20;
				map[posicionAlarma2] = 24;
				prepareArrays(currentPos, texProgram);
			}
			else if (map[posicionAlarma1] == 32) {
				map[posicionAlarma1] = 24;
				map[posicionAlarma2] = 20;
				prepareArrays(currentPos, texProgram);
			}
			tiempo = 0;
		}
		else {
			tiempo += deltaTime;
		}
	}
}

void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);

	borderTilesheet.loadFromFile("images/borders.png", TEXTURE_PIXEL_FORMAT_RGBA);
	borderTilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	borderTilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	borderTilesheet.setMinFilter(GL_NEAREST);
	borderTilesheet.setMagFilter(GL_NEAREST);

	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	map = new int[mapSize.x * mapSize.y];
	moneyTiles = 0;
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			fin.get(tile);
			if (tile == ' ')
				map[j*mapSize.x + i] = 0;
			else {
				map[j*mapSize.x + i] = tile - int('A') + 1;
				if (tile == 'K' || tile == 'L') moneyTiles++;
			}
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();

	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if (tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize / 2);
				texCoordTile[0] = glm::vec2(float((tile - 1) % 4) / tilesheetSize.x, float((tile - 1) / 4) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize / 2);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize / 2);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize / 2);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) {
	int x, y0, y1;

	x = pos.x / tileSize;
	y0 = (pos.y + 4) / (tileSize / 2);
	y1 = (pos.y - 4 + size.y - 1) / (tileSize / 2);
	for (int y = y0; y <= y1; y++)
	{
		if ((map[y*mapSize.x + x] != 0 && map[y*mapSize.x + x] != 13) || (pos.x < 22)) {
			if (map[y*mapSize.x + x] != 13 && map[y*mapSize.x + x] != 9) {
				checkTile(x, y);
			}
			else if (pos.x < 22)
				soundEngine->play2D("sounds/wall.wav");
			return true;
		}

	}

	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) {
	int x, y0, y1;

	x = (pos.x + size.x - 1) / tileSize;
	y0 = (pos.y + 4) / (tileSize / 2);
	y1 = (pos.y - 4 + size.y - 1) / (tileSize / 2);
	for (int y = y0; y <= y1; y++)
	{
		if ((map[y*mapSize.x + x] != 0 && map[y*mapSize.x + x] != 10) || (pos.x > 430)) {
			if (map[y*mapSize.x + x] != 10 && map[y*mapSize.x + x] != 9) {
				checkTile(x, y);
			}
			else if (pos.x > 430)
				soundEngine->play2D("sounds/wall.wav");
			return true;
		}

	}

	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) {
	int x0, x1, y;

	x0 = (pos.x + 4) / tileSize;
	x1 = (pos.x - 4 + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / (tileSize / 2);
	for (int x = x0; x <= x1; x++)
	{
		if ((map[y*mapSize.x + x] != 0 && map[y*mapSize.x + x] != 13 && map[y*mapSize.x + x] != 10) || (pos.y > 460))
		{
			if (map[y*mapSize.x + x] != 9) {
				checkTile(x, y);
			}
			return true;
		}
	}

	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size) {
	int x0, x1, y;

	x0 = (pos.x + 4) / tileSize;
	x1 = (pos.x - 4 + size.x - 1) / tileSize;
	y = pos.y / (tileSize / 2);

	for (int x = x0; x <= x1; x++) {
		int currentTile = map[y*mapSize.x + x];
		if ((currentTile != 0 && currentTile != 13 && currentTile != 10)) {
			if (map[(y + 1)*mapSize.x + x] == 0) {
				if (currentTile != 9) {
					checkTile(x, y);
				}
				return true;
			}
		}
	}
	return false;
}

bool TileMap::collisionMoveUpRight(const glm::ivec2 &pos, const glm::ivec2 &size) {
	int x0, x1, y;

	x0 = (pos.x + 4) / tileSize;
	x1 = (pos.x - 4 + size.x - 1) / tileSize;
	y = pos.y / (tileSize / 2);

	int res1 = collision(pos.x, pos.x + 19, pos.y);
	int res2 = collision(pos.x + 19, pos.x + 23, pos.y);
	if (res1 == -1 && res2 != -1) {
		if (map[(y + 1)*mapSize.x + res2] == 0) {
			if (map[y*mapSize.x + res2] != 9) {
				checkTile(res2, y);
				return true;
			}
		}
	}
	return false;
}



bool TileMap::collisionMoveUpLeft(const glm::ivec2 &pos, const glm::ivec2 &size) {
	int y;
	y = pos.y / (tileSize / 2);

	int res1 = collision(pos.x, pos.x + 5, pos.y);
	int res2 = collision(pos.x + 5, pos.x + 23, pos.y);
	if (res1 != -1 && res2 == -1) {
		if (map[(y + 1)*mapSize.x + res1] == 0) {
			if (map[y*mapSize.x + res1] != 9) {
				checkTile(res1, y);
				return true;
			}
		}
	}
	return false;
}

bool TileMap::collisionMoveDownLeft(const glm::ivec2 &pos, const glm::ivec2 &size) {
	int y;
	y = (pos.y + size.y - 1) / (tileSize / 2);

	int res1 = collision(pos.x, pos.x + 5, pos.y + size.y - 1);
	int res2 = collision(pos.x + 5, pos.x + 23, pos.y + size.y - 1);
	if (res1 != -1 && res2 == -1) {
		if (map[y*mapSize.x + res1] != 9) {
			checkTile(res1, y);
			return true;
		}

	}
	return false;
}

bool TileMap::collisionMoveDownRight(const glm::ivec2 &pos, const glm::ivec2 &size) {
	int y;
	y = (pos.y + size.y - 1) / (tileSize / 2);

	int res1 = collision(pos.x, pos.x + 19, pos.y + size.y - 1);
	int res2 = collision(pos.x + 19, pos.x + 23, pos.y + size.y - 1);
	if (res1 == -1 && res2 != -1) {
		if (map[y*mapSize.x + res2] != 9) {
			checkTile(res2, y);
			return true;
		}

	}
	return false;
}

int TileMap::collision(const int x0, const int x1, const int y) {
	int x0aux = x0 / tileSize;
	int x1aux = x1 / tileSize;
	int yaux = y / (tileSize / 2);

	for (int x = x0aux; x <= x1aux; x++) {
		if ((map[yaux*mapSize.x + x] != 0 && map[yaux*mapSize.x + x] != 13 && map[yaux*mapSize.x + x] != 10)) {
			return x;
		}
	}
	return -1;
}


void TileMap::openBossExit() {
	for (int x = 4; x < 8; x++) {
		map[2*mapSize.x+x] = 0;
	}
	prepareArrays(glm::vec2(0, 0), texProgram);
}

void TileMap::openExit() {
	for (int x = 4; x < 8; x++) {
		map[x] = 0;
	}
	prepareArrays(glm::vec2(0, 0), texProgram);
}


void TileMap::setShaderProgram(ShaderProgram program) {
	texProgram = program;
}

void TileMap::moveTileMap(const glm::vec2 &minCoords) {
	currentPos = finalPos;
	finalPos = minCoords;
	prepareArrays(minCoords, texProgram);
}

void TileMap::setSoundEngine(irrklang::ISoundEngine* eng) {
	soundEngine = eng;
}

void TileMap::checkTile(int x, int y) {
	int currentTile = map[y*mapSize.x + x];

	if (currentTile == 18 || currentTile == 22) {
		Game::instance().catchKey();
	}
	else if (currentTile == 17 || currentTile == 21) {
		Game::instance().setTelefono();
	}
	else if (currentTile == 11 || currentTile == 15) {
		Game::instance().gainMoney(100);
	}
	else if (currentTile == 12 || currentTile == 16) {
		Game::instance().gainMoney(400);
	}
	else {
		Game::instance().breakBrick();
	}
	if (currentTile == 11 || currentTile == 12 || currentTile == 17 || currentTile == 18) {
		map[(y + 1)*mapSize.x + x] = 0;
	}
	else if (currentTile == 15 || currentTile == 16 || currentTile == 21 || currentTile == 22) {
		map[(y - 1)*mapSize.x + x] = 0;
	}

	if (currentTile == 19) map[y*mapSize.x + x] = 23;
	else if (currentTile == 25) map[y*mapSize.x + x] = 29;
	else if (currentTile == 29) map[y*mapSize.x + x] = 26;
	else if (currentTile == 20 || currentTile == 24 || currentTile == 28 || currentTile == 32) {
		if (!alarmOn) {
			Game::instance().alarmOn();
			alarmOn = true;
			posicionAlarma1 = y * mapSize.x + x;
			if (currentTile == 20) posicionAlarma2 = (y + 1) * mapSize.x + x;
			else posicionAlarma2 = (y - 1) * mapSize.x + x;
		}
	}
	else map[y*mapSize.x + x] = 0;
	prepareArrays(glm::vec2(0, 0), texProgram);
}

void TileMap::setAlarm(bool alarm) {
	alarmOn = alarm;
}