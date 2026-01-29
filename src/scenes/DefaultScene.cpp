#include "pch.h"
#include "DefaultScene.h"
#include "entities/Cat.h"

DefaultScene::DefaultScene() {

}

DefaultScene::~DefaultScene() {

}

void DefaultScene::load() {
	
	//music = sf::Music("assets/Cutie-Patootie.mp3");
}

void DefaultScene::init() {
	Cat::get().init();

	//music.play();
}

void DefaultScene::update(float dt) {
//#define DEBUG_ANIMATIONS
#ifdef DEBUG_ANIMATIONS
	static bool prevIsNPressed = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::N) && !prevIsNPressed) {
		prevIsNPressed = true;
		Cat::get().setEntityAnimationState((Cat::EntityAnimationStates)(((int)Cat::get().getActiveState() + 1) % (int)Cat::EntityAnimationStates::NUM_ENTITY_STATES), tm.getSprite(Cat::get().getCatSpriteName()));
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
		++tm.getSprite(Cat::get().getCatSpriteName()).width;
		prevIsRightPressed = true;
	} 
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) {
		prevIsRightPressed = false;
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left) && !prevIsLeftPressed) {
		--tm.getSprite(Cat::get().getCatSpriteName()).width;
		prevIsLeftPressed = true;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)) {
		prevIsLeftPressed = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up) && !prevIsUpPressed) {
		++tm.getSprite(Cat::get().getCatSpriteName()).height;
		prevIsUpPressed = true;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up)) {
		prevIsUpPressed = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down) && !prevIsDownPressed) {
		--tm.getSprite(Cat::get().getCatSpriteName()).height;
		prevIsDownPressed = true;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down)) {
		prevIsDownPressed = false;
	}


	std::cout << tm.getSprite(Cat::get().getCatSpriteName()).width << ", " << tm.getSprite(Cat::get().getCatSpriteName()).height << std::endl;
#endif

	Cat::get().update(dt);

//#define MOVE_TO_MOUSE
#ifdef MOVE_TO_MOUSE
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		sf::Vector2i mpos = sf::Mouse::getPosition();

		Cat::get().moveTo(mpos.x, mpos.y);
	}
#endif


}

void DefaultScene::cleanup() {
	tm.cleanup();
}