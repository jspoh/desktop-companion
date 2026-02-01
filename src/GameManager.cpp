#pragma once

#include "pch.h"
#include "GameManager.h"
#include "TextureManager.h"


GameManager& GameManager::get() {
	static GameManager instance;
	return instance;
}

float GameManager::getDt() {
	return dt;
}

int GameManager::getFps() {
	return fps;
}

void GameManager::init(const sf::Color& win32_transparent_color) {
	WIN32_TRANSPARENT_COLOR = win32_transparent_color;

	sm.setScene(std::make_shared<DefaultScene>());

	Room::get();
	Cat::get();

	// no modal background
	ImGui::GetStyle().Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
}

void GameManager::update() {
	static float elapsed{};
	sf::Color clearColor{ sf::Color::Transparent };

	while (running && win.isOpen()) {

		auto start = std::chrono::high_resolution_clock::now();

		// events
		while (const std::optional event = win.pollEvent())
		{
			lastEventTime = std::chrono::system_clock::now();
			//std::cout << "event" << std::endl;

			// Close window: exit
			if (event->is<sf::Event::Closed>())
				running = false;

			ImGui::SFML::ProcessEvent(win, *event);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LControl)
			&& sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift)
			&& sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num1)) {
			running = false;
		}

		static bool prevGravePressed = false;
		if (!prevGravePressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Grave)) {
			prevGravePressed = true;
			showEditor = !showEditor;
		}
		else if (prevGravePressed && !sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Grave)) {
			prevGravePressed = false;
		}

		if (showEditor) {
			clearColor = { 0, 0, 0, 220 };
			Window::get().setClickThrough(false);
		}
		else {
			clearColor = sf::Color::Transparent;
			Window::get().setClickThrough(true);
		}

		ImGui::SFML::Update(win, sf::seconds(dt));

		//ImGui::Begin();

		//ImGui::ShowDemoWindow();

		//ImGui::Begin("Hello, world!");
		//ImGui::Button("Look at this pretty button");
		//ImGui::End();

		UI::render();

		// 
		win.clear(clearColor);
		if (!Settings::paused) {
			sm.update(dt);
			texM.render(dt);
		}

		if (showEditor) {
			ImGui::SFML::Render(win);
		}

		if (!Settings::paused) {
			// render cat again to be on top of imgui windows

			texM._render(0, texM.getSprite(Cat::getCatSpriteName()));
			win.draw(texM.getText(Cat::getTextRef()));
		}

		win.display();

		ImGui::EndFrame();

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

	win.close();
	ImGui::SFML::Shutdown();
}

void GameManager::cleanup() {
	Settings::save();

	sm.cleanup();
	texM.cleanup();
	Settings::cleanup();
}