#include "pch.h"
#include "Cat.h"


void Cat::init() {
	tm.registerTexture("cat_texture", "assets/Cats/AllCatsGreyWhite.png");

	tm.createSprite("cat", "cat_texture", STATE_FRAMES_MAP.at(activeAnimationState), xoffset, leftOffset, topOffset, width, height, true, animationAdvanceTime);
	TextureManager::JS_SPRITE& catSprite = tm.getSprite("cat");
	pos = Window::get().getWindow().getSize() / 2.f;
	catSprite.sprite.setPosition(pos);
	catSprite.sprite.setScale({ SPRITE_SCALE, SPRITE_SCALE });

	sf::Text& text = tm.registerText("text", "hello", 50);
	text.setPosition({ 100, 0 });
}


void Cat::setEntityState(EntityStates s, std::optional<std::reference_wrapper<TextureManager::JS_SPRITE>> opt_sprite) {
	if (entityState == s) return;

	entityState = s;
	entityStateElapsedS = 0.f;

	if (opt_sprite == std::nullopt) return;
}


void Cat::setEntityAnimationState(EntityAnimationStates s, std::optional<std::reference_wrapper<TextureManager::JS_SPRITE>> opt_sprite, int animationLoopCount=-1) {
	if (s == activeAnimationState) return;
	
	// setup queue system for after this animation state
	activeAnimationState = s;

	if (opt_sprite == std::nullopt) return;

	TextureManager::JS_SPRITE& sprite = opt_sprite.value();

	//sprite.top = STATE_TOP_MAP.at(s);
	sprite.top = topOffset + (int)s * (yoffset + height);
	sprite.left = leftOffset;
	sprite.numFrames = STATE_FRAMES_MAP.at(s);

	sprite.animationLoopCount = animationLoopCount;
	sprite.animationElapsedLoops = 0;
	if (sprite.animationLoopCount != 0) sprite.playingAnimation = true;
}


void Cat::moveTo(float x, float y) {
	setEntityAnimationState(EntityAnimationStates::RUNNING, tm.getSprite(catSpriteName), -1);

	static constexpr int w_offset = width / 2;
	static constexpr int h_offset = (height / 2) + yoffset;

	target = { x, y };

	if (facing == FACING_DIRECTIONS::LEFT) {
		target.x += w_offset;
	}
	else {
		target.x -= w_offset;
	}
	target.y -= h_offset;

	moveToComplete = false;
}

void Cat::update(float dt) {
	const auto [winX, winY] = win.getSize();
	const auto [mX, mY] = sf::Mouse::getPosition();

	static bool handledStopMoving = true;

	TextureManager::JS_SPRITE& catSprite = tm.getSprite(catSpriteName);

	// movement
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
			catSprite.sprite.setScale({ -SPRITE_SCALE, SPRITE_SCALE });
			facing = FACING_DIRECTIONS::LEFT;
		}
		else {
			catSprite.sprite.setScale({ SPRITE_SCALE, SPRITE_SCALE });
			facing = FACING_DIRECTIONS::RIGHT;
		}
	}
	else if (!handledStopMoving) {
		// handle what happens when stop moving
		handledStopMoving = true;

		// set up queue system
		setEntityAnimationState(EntityAnimationStates::IDLE, catSprite, -1);
		pos = target;
		moveToComplete = true;
		move_vector = { 0, 0 };
		d = { 0, 0 };
	}

	catSprite.sprite.setPosition(pos);

	// happiness decrement
	happiness -= dt * happiness_drain_rate_s;

	// set state according to happiness level
	for (const auto [est, s] : entityStateThresholds) {
		if (happiness <= est) {
			setEntityState(s, catSprite);
			break;
		}
	}

	entityStateElapsedS += dt;

	bool isMovingAnimationPlaying = std::find(MOVEMENT_ANIMATION_STATES.begin(), MOVEMENT_ANIMATION_STATES.end(), activeAnimationState) != MOVEMENT_ANIMATION_STATES.end();
	static bool prevIsMovingAnimationPlaying = true;

	// just stopped moving animation, play an idle animation dependent on happiness level
	if (!isMovingAnimationPlaying && prevIsMovingAnimationPlaying) {
		const auto& availableAnimations = STATE_ANIMATION_MAP.at(entityState);
		setEntityAnimationState(availableAnimations.at(rand() % (int)availableAnimations.size()), catSprite);

		// set idle time
		idleTimeLeft = (float)(rand() % (MAX_IDLE_TIME - MIN_IDLE_TIME) + MIN_IDLE_TIME);
		//std::cout << "Idling for " << idleTimeLeft << std::endl;
	}

	prevIsMovingAnimationPlaying = isMovingAnimationPlaying;
	idleTimeLeft -= dt;

	if (idleTimeLeft <= 0) {
		//std::cout << "Idle time over" << std::endl;
		idleTimeLeft = std::numeric_limits<float>::max();
		moveTo(
			rand() % (int)((winX * RAND_POS_PADDING - winX * (1.f - RAND_POS_PADDING)) + winX * RAND_POS_PADDING),
			rand() % (int)((winY * RAND_POS_PADDING - winY * (1.f - RAND_POS_PADDING)) + winY * RAND_POS_PADDING)
		);
	}
}
