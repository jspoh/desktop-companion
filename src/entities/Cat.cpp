#include "Cat.h"


void Cat::init() {
	tm.registerTexture("cat_texture", "assets/css2.png");

	tm.createSprite("cat", "cat_texture", STATE_FRAMES_MAP.at(activeAnimationState), xoffset, leftOffset, topOffset, width, height, true, animationAdvanceTime);
	TextureManager::JS_SPRITE& catSprite = tm.getSprite("cat");
	catSprite.sprite.setPosition(pos);

	sf::Text& text = tm.registerText("text", "hello", 50);
	text.setPosition({ 100, 0 });
}


void Cat::setEntityState(EntityAnimationStates s, std::optional<std::reference_wrapper<TextureManager::JS_SPRITE>> opt_sprite) {
	activeAnimationState = s;

	if (opt_sprite == std::nullopt) return;

	TextureManager::JS_SPRITE& sprite = opt_sprite.value();

	//sprite.top = STATE_TOP_MAP.at(s);
	sprite.top = (int)s * yoffset;
	sprite.left = leftOffset;
	sprite.numFrames = STATE_FRAMES_MAP.at(s);
}


void Cat::moveTo(float x, float y) {
	setEntityState(EntityAnimationStates::RUNNING, tm.getSprite(catSpriteName));

	target = { x, y };

	moveToComplete = false;
}

void Cat::update(float dt) {
	static bool handledStopMoving = false;

	TextureManager::JS_SPRITE& catSprite = tm.getSprite(catSpriteName);

	if (!moveToComplete && (target - pos).lengthSquared() > std::pow(MOVEMENT_EPSILON, 2.f)) {
		handledStopMoving = false;

		d = target - pos;
		if (d.lengthSquared() != 0) d = d.normalized();

		// accelerate cat

		move_vector += d * (dt * ACCELERATION);
		if (move_vector.length() > MAX_MOVEMENT_SPEED) {
			move_vector = move_vector.normalized() * MAX_MOVEMENT_SPEED;
		}

		// P = A + td where d = B-A
		pos += dt * move_vector;

		// ensure no overshot using dot product
		// same dir = +ve, orthogonal = 0, opposite dir = -ve
		if ((target - pos).dot(d) < 0) {
			// has overshot.
			pos = target;
		}

		// facing left/right
		if (d.dot(RIGHT_VECTOR) < 0) {
			// is moving left
			catSprite.sprite.setScale({ -1, 1 });
		}
		else {
			catSprite.sprite.setScale({ 1, 1 });
		}
	}
	else if (!handledStopMoving) {
		// handle what happens when stop moving
		handledStopMoving = true;

		setEntityState(EntityAnimationStates::IDLE, catSprite);
		pos = target;
		moveToComplete = true;
		move_vector = { 0, 0 };
		d = { 0, 0 };
	}

	catSprite.sprite.setPosition(pos);
}
