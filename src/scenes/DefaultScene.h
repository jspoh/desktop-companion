#pragma once

#include "pch.h"
#include "Scene.h"
#include "TextureManager.h"


class DefaultScene : public Scene {
private:
	sf::Music music;
	sf::Text* fps_display_text;
public:
	DefaultScene();

	~DefaultScene();

	void load() override;

	void init() override;

	void update(float dt) override;

	void cleanup() override;
};
