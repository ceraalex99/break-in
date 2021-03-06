#include "Instructions.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glut.h>
#include "Game.h"




Instructions::Instructions()
{
}


Instructions::~Instructions()
{
}

void Instructions::init() {
	initShaders();
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(640.f, 480.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	if (!text.init("fonts/8-bit-pusab.ttf"))
		cout << "Could not load font" << endl;
	instructionsImage.loadFromFile("images/instructions1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	currentTime = 0.f;
	pag = 1;
}

void Instructions::update(int deltaTime) {
	currentTime += deltaTime;
	if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
		Game::instance().specialKeyReleased(GLUT_KEY_RIGHT);
		++pag;
	switch (pag) {
		case 0:
			Game::instance().startAction(3);
			break;
		case 1:
			instructionsImage.loadFromFile("images/instructions1.png", TEXTURE_PIXEL_FORMAT_RGBA);
			break;
		case 2:
			instructionsImage.loadFromFile("images/instructions2.png", TEXTURE_PIXEL_FORMAT_RGBA);
			break;
		case 3:
			instructionsImage.loadFromFile("images/instructions3.png", TEXTURE_PIXEL_FORMAT_RGBA);
			break;
		case 4:
			instructionsImage.loadFromFile("images/instructions4.png", TEXTURE_PIXEL_FORMAT_RGBA);
			break;
		case 5:
			instructionsImage.loadFromFile("images/instructions5.png", TEXTURE_PIXEL_FORMAT_RGBA);
			break;
		case 6:
			instructionsImage.loadFromFile("images/instructions6.png", TEXTURE_PIXEL_FORMAT_RGBA);
			break;
		case 7:
			instructionsImage.loadFromFile("images/instructions7.png", TEXTURE_PIXEL_FORMAT_RGBA);
			break;
		case 8:
			instructionsImage.loadFromFile("images/instructions8.png", TEXTURE_PIXEL_FORMAT_RGBA);
			break;
		case 9:
			instructionsImage.loadFromFile("images/instructions9.png", TEXTURE_PIXEL_FORMAT_RGBA);
			break;
		case 10:
			Game::instance().startAction(3);
			break;
		}
	}

	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
		Game::instance().specialKeyReleased(GLUT_KEY_LEFT);
		--pag;
		switch (pag) {
		case 0:
			Game::instance().startAction(3);
			break;
		case 1:
			instructionsImage.loadFromFile("images/instructions1.png", TEXTURE_PIXEL_FORMAT_RGBA);
			break;
		case 2:
			instructionsImage.loadFromFile("images/instructions2.png", TEXTURE_PIXEL_FORMAT_RGBA);
			break;
		case 3:
			instructionsImage.loadFromFile("images/instructions3.png", TEXTURE_PIXEL_FORMAT_RGBA);
			break;
		case 4:
			instructionsImage.loadFromFile("images/instructions4.png", TEXTURE_PIXEL_FORMAT_RGBA);
			break;
		case 5:
			instructionsImage.loadFromFile("images/instructions5.png", TEXTURE_PIXEL_FORMAT_RGBA);
			break;
		case 6:
			instructionsImage.loadFromFile("images/instructions6.png", TEXTURE_PIXEL_FORMAT_RGBA);
			break;
		case 7:
			instructionsImage.loadFromFile("images/instructions7.png", TEXTURE_PIXEL_FORMAT_RGBA);
			break;
		case 8:
			instructionsImage.loadFromFile("images/instructions8.png", TEXTURE_PIXEL_FORMAT_RGBA);
			break;
		case 9:
			instructionsImage.loadFromFile("images/instructions9.png", TEXTURE_PIXEL_FORMAT_RGBA);
			break;
		case 10:
			Game::instance().startAction(3);
			break;
		}
	}
	

}

void Instructions::render() {
	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	background->render(instructionsImage);

	//text.render("PRESS ESC TO EXIT TO MENU SCREEN", glm::vec2(240, 450), 10, glm::vec4(1.f,1.f,1.f,1.f));

}

void Instructions::initShaders() {
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