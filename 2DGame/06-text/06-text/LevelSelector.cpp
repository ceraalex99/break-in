#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "LevelSelector.h"
#include "Game.h"
#include <GL/glut.h>

LevelSelector::LevelSelector()
{
}

LevelSelector::~LevelSelector()
{
}

void LevelSelector::init() {
	initShaders();
	level = 0;
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(640.f, 480.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	backgroundImage.loadFromFile("images/breakin.png", TEXTURE_PIXEL_FORMAT_RGBA);
	if (!text.init("fonts/8-bit-pusab.ttf"))
		cout << "Could not load font" << endl;
	currentTime = 0.0f;
}

void LevelSelector::update(int deltaTime) {
	currentTime += deltaTime;

	if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
		Game::instance().specialKeyReleased(GLUT_KEY_DOWN);
		++level;
		level %= 3;
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
		Game::instance().specialKeyReleased(GLUT_KEY_UP);
		level += 2;
		level %= 3;
	}

	if (Game::instance().getKey(13) || Game::instance().getKey(32)) {
		Game::instance().keyReleased(13);
		Game::instance().keyReleased(32);
		Game::instance().selectLevel(level);
	}

	//TODO: action behaviors
}

void LevelSelector::render() {
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	background->render(backgroundImage);

	if (level == 0) {
		text.render("LEVEL 1", glm::vec2(240, 360), 36, glm::vec4(0.5, 0.4, 1, 1));
		text.render("LEVEL 2", glm::vec2(260, 410), 24, glm::vec4(1, 1, 1, 1));
		text.render("LEVEL 3", glm::vec2(260, 460), 24, glm::vec4(1, 1, 1, 1));
	}
	else if (level == 1) {
		text.render("LEVEL 1", glm::vec2(260, 360), 24, glm::vec4(1, 1, 1, 1));
		text.render("LEVEL 2", glm::vec2(240, 410), 36, glm::vec4(0.5, 0.4, 1, 1));
		text.render("LEVEL 3", glm::vec2(260, 460), 24, glm::vec4(1, 1, 1, 1));
	}
	else {
		text.render("LEVEL 1", glm::vec2(260, 360), 24, glm::vec4(1, 1, 1, 1));
		text.render("LEVEL 2", glm::vec2(260, 410), 24, glm::vec4(1, 1, 1, 1));
		text.render("LEVEL 3", glm::vec2(240, 460), 36, glm::vec4(0.5, 0.4, 1, 1));
	}

}

void LevelSelector::initShaders() {
	Shader vShader, fShader;

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
	vShader.free();
	fShader.free();
}