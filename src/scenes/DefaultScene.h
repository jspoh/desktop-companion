#pragma once

#include "Scene.h"
#include "TextureManager.h"
#include "defines.h"


class DefaultScene : public Scene {
private:
	sf::Music music;

public:
	DefaultScene();

	~DefaultScene();

	void load() override;

	void init() override;

	void update(float dt) override;

	void cleanup() override;
};
