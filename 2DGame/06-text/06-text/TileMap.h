#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include <irrKlang.h>



// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);
	~TileMap();

	void render() const;
	void free();
	
	int getTileSize() const { return tileSize; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size);
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size);
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY);
	bool collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size);

	bool collisionMoveUpRight(const glm::ivec2 & pos, const glm::ivec2 & size);
	bool collisionMoveUpLeft(const glm::ivec2 & pos, const glm::ivec2 & size);
	bool collisionMoveDownRight(const glm::ivec2 & pos, const glm::ivec2 & size);
	bool collisionMoveDownLeft(const glm::ivec2 & pos, const glm::ivec2 & size);

	void openExit(ShaderProgram texProgram);
	void setShaderProgram(ShaderProgram program);
	void moveTileMap(const glm::vec2 &minCoords, ShaderProgram texProgram);
	void setSoundEngine(irrklang::ISoundEngine* eng);
	
private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet, borderTilesheet;
	glm::vec2 tileTexSize;
	int *map;
	ShaderProgram texProgram;
	irrklang::ISoundEngine* soundEngine;


};


#endif // _TILE_MAP_INCLUDE


