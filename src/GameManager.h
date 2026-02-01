#pragma once

#include "pch.h"
#include "TextureManager.h"
#include "SceneManager.h"
#include "scenes/DefaultScene.h"
#include "Window.h"
#include "ui/ui.h"
#include "BreakData.h"

class GameManager {
public:
	std::chrono::system_clock::time_point lastEventTime = std::chrono::system_clock::now();

private:
	sf::Color WIN32_TRANSPARENT_COLOR;

	GameManager() {
		Settings::init();
	};
	~GameManager() {
		Settings::save();
	};

	bool running = true;
	float dt = 0.f;
	int fps = 0;

public:
	static constexpr int MAX_BREAKDATA_SIZE = 5;
	std::deque<BreakData> breaks;

	static GameManager& get();

	float getDt();

	int getFps();

	void init(const sf::Color& win32_transparent_color);

	bool showEditor{};
	void update();

	void cleanup();
};