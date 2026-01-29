#pragma once

#include "TextureManager.h"
#include "SceneManager.h"
#include "pch.h"
#include "scenes/DefaultScene.h"
#include "Window.h"

class GameManager {
private:
	sf::Color WIN32_TRANSPARENT_COLOR;

	GameManager() {};
	~GameManager() {};

	bool running = true;
	float dt = 0.f;
	int fps = 0;
public:
	static GameManager& get() {
		static GameManager instance;
		return instance;
	}

	float getDt() {
		return dt;
	}

	int getFps() {
		return fps;
	}

	void init(const sf::Color& win32_transparent_color) {
		WIN32_TRANSPARENT_COLOR = win32_transparent_color;

		sm.setScene(std::make_shared<DefaultScene>());
	}

	void update() {
		static float elapsed{};
		while (running && win.isOpen()) {

			auto start = std::chrono::high_resolution_clock::now();

			// events
			while (const std::optional event = win.pollEvent())
			{
				// Close window: exit
				if (event->is<sf::Event::Closed>())
					win.close();
			}

			// 
			win.clear(WIN32_TRANSPARENT_COLOR);
			sm.update(dt);
			tm.render(dt);
			win.display();

			auto end = std::chrono::high_resolution_clock::now();

			const float dt_ms = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
			dt = dt_ms / 1000.f;

			//std::cout << dt << std::endl;
			elapsed += dt;

			if (elapsed > 1) {
				elapsed = 0;
				fps = std::floor(1 / dt);
			}
		}
	}

	void cleanup() {
		sm.cleanup();
		tm.cleanup();
	}
};