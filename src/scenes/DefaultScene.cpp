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
#ifdef DEBUG_ANIMATIONS
	static bool prevIsRightPressed = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right) && !prevIsRightPressed) {
		prevIsRightPressed = true;
		Cat::get().setEntityState((Cat::EntityStates)((Cat::get().getActiveState() + 1) % Cat::EntityStates::NUM_ENTITY_STATES), tm.getSprite(Cat::get().getCatSpriteName()));
		std::cout << "EntityState: " << Cat::get().getActiveState() << std::endl;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) {
		prevIsRightPressed = false;
	}
#endif

	Cat::get().update(dt);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		sf::Vector2i mpos = sf::Mouse::getPosition();

		Cat::get().moveTo(mpos.x, mpos.y);
	}
}

void DefaultScene::cleanup() {
	tm.cleanup();
}