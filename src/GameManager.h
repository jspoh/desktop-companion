#pragma once

#include "TextureManager.h"

class GameManager {
private:
	sf::Color WIN32_TRANSPARENT_COLOR;

	GameManager() {};
	~GameManager() {};

public:
	static GameManager& get() {
		static GameManager gm;
		return gm;
	}

	void init(const sf::Color& win32_transparent_color) {
		WIN32_TRANSPARENT_COLOR = win32_transparent_color;
	}


};