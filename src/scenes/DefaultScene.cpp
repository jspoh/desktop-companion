#include "DefaultScene.h"

DefaultScene::DefaultScene() {

}

DefaultScene::~DefaultScene() {

}

void DefaultScene::load() {
	tm.registerTexture("idle_cat_texture", "assets/css2.png");
	//music = sf::Music("assets/Cutie-Patootie.mp3");
}

void DefaultScene::init() {
	tm.createSprite("idle_cat", "idle_cat_texture", 6, 5, 0, 15, 45, 35, true, 0.1f);
	TextureManager::JS_SPRITE& idle_cat = tm.getSprite("idle_cat");
	idle_cat.sprite.setPosition({ 100.f, 100.f });

	sf::Text& text = tm.registerText("text", "hello", 50);
	text.setPosition({ 100, 0 });

	//music.play();
}

void DefaultScene::update(float dt) {

}

void DefaultScene::cleanup() {
	tm.cleanup();
}