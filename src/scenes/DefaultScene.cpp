#include "DefaultScene.h"
#include "entities/Cat.h"

DefaultScene::DefaultScene() {

}

DefaultScene::~DefaultScene() {

}

void DefaultScene::load() {
	
	//music = sf::Music("assets/Cutie-Patootie.mp3");
}

void DefaultScene::init() {
	Cat c;
	c.init();

	//music.play();
}

void DefaultScene::update(float dt) {

}

void DefaultScene::cleanup() {
	tm.cleanup();
}