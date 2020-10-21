#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Menu.h"
#include "Game.h"
#include <GL/glut.h>




Menu::Menu()
{
}


Menu::~Menu()
{
}

void Menu::init()
{
	initShaders();
	action = 0;
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(640.f, 480.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	backgroundImage.loadFromFile("images/breakin.png", TEXTURE_PIXEL_FORMAT_RGBA);
	if (!text.init("fonts/emulogic.ttf"))
		cout << "Could not load font" << endl;
	currentTime = 0.0f;
}

void Menu::update(int deltaTime)
{
	currentTime += deltaTime;

	if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
		Game::instance().specialKeyReleased(GLUT_KEY_DOWN);
		++action;
		action %= 3;
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
		Game::instance().specialKeyReleased(GLUT_KEY_UP);
		action += 2;
		action %= 3;
	}

	if (Game::instance().getKey(13) || Game::instance().getKey(32)) {
		Game::instance().startAction(action);
	}

	//TODO: action behaviors
}

void Menu::render(){
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	background->render(backgroundImage);

	if (action == 0) {
		text.render("Play", glm::vec2(260, 380), 20, glm::vec4(1, 1, 1, 1));
		text.render("Instructions", glm::vec2(220, 420), 14, glm::vec4(1, 1, 1, 1));
		text.render("Credits", glm::vec2(250, 460), 14, glm::vec4(1, 1, 1, 1));
	}
	else if (action == 1) {
		text.render("Play", glm::vec2(270, 380), 14, glm::vec4(1, 1, 1, 1));
		text.render("Instructions", glm::vec2(180, 420), 20, glm::vec4(1, 1, 1, 1));
		text.render("Credits", glm::vec2(250, 460), 14, glm::vec4(1, 1, 1, 1));
	}
	else{
		text.render("Play", glm::vec2(270, 380), 14, glm::vec4(1, 1, 1, 1));
		text.render("Instructions", glm::vec2(220, 420), 14, glm::vec4(1, 1, 1, 1));
		text.render("Credits", glm::vec2(220, 460), 20, glm::vec4(1, 1, 1, 1));
	}

}

void Menu::initShaders(){
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
