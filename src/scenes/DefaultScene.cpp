#include "pch.h"
#include "DefaultScene.h"
#include "entities/Cat.h"
#include "GameManager.h"
#include "Window.h"

//#define VIEW_FPS

DefaultScene::DefaultScene() {

}

DefaultScene::~DefaultScene() {

}

void DefaultScene::load() {

	//music = sf::Music("assets/Cutie-Patootie.mp3");
}

void DefaultScene::init() {
	Room::get().init();
	Cat::get().init();

	//music.play();

#ifdef VIEW_FPS
#ifndef NDEBUG
	fps_display_text = &texM.registerText("text", std::to_string(gm.getFps()), 50);
	fps_display_text->setPosition({ 10, 10 });
#endif
#endif
}

void DefaultScene::update(float dt) {

	if (Settings::onEnforcedBreak) return;



#ifdef VIEW_FPS
#ifndef NDEBUG
	fps_display_text->setString(std::to_string(gm.getFps()));
#endif
#endif

	//#define DEBUG_ANIMATIONS
#ifdef DEBUG_ANIMATIONS
	static bool prevIsNPressed = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::N) && !prevIsNPressed) {
		prevIsNPressed = true;
		Cat::get().setEntityAnimationState((Cat::EntityAnimationStates)(((int)Cat::get().getActiveState() + 1) % (int)Cat::EntityAnimationStates::NUM_ENTITY_STATES), texM.getSprite(Cat::get().getCatSpriteName()));
		std::cout << "EntityState: " << (int)Cat::get().getActiveState() << std::endl;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::N)) {
		prevIsNPressed = false;
	}
#endif

	//#define DEBUG_SS
#ifdef DEBUG_SS
	static bool prevIsRightPressed = false;
	static bool prevIsLeftPressed = false;
	static bool prevIsUpPressed = false;
	static bool prevIsDownPressed = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right) && !prevIsRightPressed) {
		++texM.getSprite(Cat::get().getCatSpriteName()).width;
		prevIsRightPressed = true;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) {
		prevIsRightPressed = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left) && !prevIsLeftPressed) {
		--texM.getSprite(Cat::get().getCatSpriteName()).width;
		prevIsLeftPressed = true;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)) {
		prevIsLeftPressed = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up) && !prevIsUpPressed) {
		++texM.getSprite(Cat::get().getCatSpriteName()).height;
		prevIsUpPressed = true;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up)) {
		prevIsUpPressed = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down) && !prevIsDownPressed) {
		--texM.getSprite(Cat::get().getCatSpriteName()).height;
		prevIsDownPressed = true;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down)) {
		prevIsDownPressed = false;
	}


	std::cout << texM.getSprite(Cat::get().getCatSpriteName()).width << ", " << texM.getSprite(Cat::get().getCatSpriteName()).height << std::endl;
#endif

	Cat::get().update(dt);
	Room::get().update(dt);

	//#define MOVE_TO_MOUSE
#ifdef MOVE_TO_MOUSE
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		sf::Vector2i mpos = sf::Mouse::getPosition();

		Cat::get().moveTo(mpos.x, mpos.y);
}
#endif

	// handle user events and cat happiness
	auto now = std::chrono::system_clock::now();
	static bool onBreak = false;
	static bool prevOnBreak = false;

	static float inactivitySeconds{};
	static float prevInactivitySeconds = inactivitySeconds;

	if (Window::get().userIsActive()) {
		inactivitySeconds = 0;
	}
	else {
		inactivitySeconds += dt;
	}

	//std::cout << secondsSinceLastActivity << std::endl;

	if (inactivitySeconds >= Settings::MIN_BREAK_DURATION_M * 60) {
		// last event was more than break time ago
		onBreak = true;
		//std::cout << "on break" << std::endl;
	}
	else {
		onBreak = false;
		//std::cout << "no longer on break" << std::endl;
	}

	if (prevOnBreak && !onBreak) {
		// user just came back, store break data

		std::time_t endBreakTime = std::chrono::system_clock::to_time_t(now);
		std::time_t startBreakTime = std::chrono::system_clock::to_time_t(now - std::chrono::seconds((int64_t)prevInactivitySeconds));

		gm.breaks.emplace_back(BreakData((uint64_t)startBreakTime, (uint64_t)endBreakTime));

		//std::cout << "working" << std::endl;

		if (gm.breaks.size() > gm.MAX_BREAKDATA_SIZE) {
			gm.breaks.pop_front();
		}

		Settings::coins += 20;
		Settings::save();
	}

	if (onBreak) {
		Cat::get().setHappinessMax();
	}

	prevOnBreak = onBreak;
	prevInactivitySeconds = inactivitySeconds;

#ifdef PROMPT_USER_CONFIRM_BREAK
	static bool breakPopupOpen = false;
	if (secondsSinceLastActivity >= Settings::MIN_BREAK_DURATION_M * 60) {
		// last event was more than break time ago

		if (!breakPopupOpen) {
			breakPopupOpen = true;
			gm.showEditor = true;

			sf::Vector2f ppp = win.getSize() / 2.f;
			ImGui::SetNextWindowPos({ ppp.x, ppp.y }, ImGuiCond_Always, { 0, 0 });
			ImGui::OpenPopup("Breaktime");
		}
	}

	static auto handleBreaktimePopupBtn = [](bool tookBreak) {
		if (tookBreak) {
			Cat::get().setHappinessMax();
		}
		ImGui::CloseCurrentPopup();
		breakPopupOpen = false;
		};

	const int iSecsInactivity = std::floor(secondsSinceLastActivity);
	static int prevISecsInactivity = iSecsInactivity;
	if (ImGui::BeginPopupModal("Breaktime", nullptr, 0)) {

		ImGui::Text("Did you take a break?");

		ImGui::Spacing();

		ImGui::TextColored({ 0, 1, 0, 1 }, "Inactivity: %02d:%02d", prevISecsInactivity / 60, prevISecsInactivity % 60);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::Button("Yes")) {
			handleBreaktimePopupBtn(true);
		}
		if (ImGui::Button("No")) {
			handleBreaktimePopupBtn(false);
		}
		ImGui::EndPopup();
	}
	else {
		prevISecsInactivity = 0;
	}

	prevISecsInactivity = std::max(iSecsInactivity, prevISecsInactivity);
#endif

}

void DefaultScene::cleanup() {
	texM.cleanup();
}