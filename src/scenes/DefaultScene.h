#pragma once

#include "Scene.h"
#include "TextureManager.h"
#include "defines.h"


class DefaultScene : public Scene {
private:
	sf::Music music;

public:
	DefaultScene() {}

	~DefaultScene() {

	}

	void load() override {
		tm.registerTexture("idle_cat_texture", "assets/css2.png");
		//music = sf::Music("assets/Cutie-Patootie.mp3");
	}

	void init() override {
		tm.createSprite("idle_cat", "idle_cat_texture", 6, 0, 0, 50, 50);
		TextureManager::JS_SPRITE& idle_cat = tm.getSprite("idle_cat");
		idle_cat.sprite.setPosition({ 100.f, 100.f });

		sf::Text& text = tm.registerText("text", "hello", 50);
		text.setPosition({ 100, 0 });

		//music.play();
	}

	void update(float dt) override {

	}

	void cleanup() override {
		tm.cleanup();
	}
};
