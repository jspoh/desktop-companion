#include "Cat.h"


void Cat::init() {
	tm.registerTexture("idle_cat_texture", "assets/css2.png");

	tm.createSprite("idle_cat", "idle_cat_texture", STATE_FRAMES_MAP.at(activeState), xoffset, leftOffset, topOffset, width, height, true, animationAdvanceTime);
	TextureManager::JS_SPRITE& idle_cat = tm.getSprite("idle_cat");
	idle_cat.sprite.setPosition({ 100.f, 100.f });

	sf::Text& text = tm.registerText("text", "hello", 50);
	text.setPosition({ 100, 0 });
}


void Cat::setEntityState(EntityStates s, std::optional<std::reference_wrapper<TextureManager::JS_SPRITE>> opt_sprite) {
	activeState = s;

	if (opt_sprite == std::nullopt) return;

	TextureManager::JS_SPRITE& sprite = opt_sprite.value();

	sprite.top = (int)s * yoffset;
	sprite.left = leftOffset;
	sprite.numFrames = STATE_FRAMES_MAP.at(s);
}
