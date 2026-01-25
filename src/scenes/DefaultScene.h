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
		tm.registerTexture("idle_cat_texture", "assets/Idle.png");
		music = sf::Music("assets/Cutie-Patootie.mp3");
	}

	void init() override {
		tm.createSprite("idle_cat", "idle_cat_texture");
		sf::Sprite& idle_cat = tm.getSprite("idle_cat");
		idle_cat.setPosition({ 100.f, 100.f });

		sf::Text& text = tm.registerText("text", "hello", 50);
		text.setPosition({ 100, 0 });

		music.play();
	}

	void update() override {

	}

	void cleanup() override {
		tm.cleanup();
	}
};
