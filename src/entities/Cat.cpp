#include "pch.h"
#include "Cat.h"


Cat::Cat() {
	const std::string path = "assets/Cats";
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (!entry.is_regular_file()) continue;
		const auto filename = entry.path().filename().string();
		tm.registerTexture(filename, path + "/" + filename);

		tm.createSprite(filename, filename, EntityAnimationDatas.at(activeAnimationState).frameCount, xoffset, leftOffset, topOffset, width, height, true, animationAdvanceTime, false);
		catSpriteRefs.push_back(filename);
	}

	catTextureRef = catSpriteRefs.at(rand() % (int)catSpriteRefs.size());
	tm.createSprite(catSpriteName, catTextureRef, EntityAnimationDatas.at(activeAnimationState).frameCount, xoffset, leftOffset, topOffset, width, height, true, animationAdvanceTime, true);


	// text

	speech = &tm.registerText(textRef, "", 16);
}


void Cat::init(bool resetPos) {
	//std::cout << resetPos << std::endl;
	TextureManager::JS_SPRITE& catSprite = tm.getSprite(catSpriteName);
	if (resetPos) {
		pos = Window::get().getWindow().getSize() / 2.f;
		catSprite.sprite.setPosition(pos);
	}
	catSprite.sprite.setScale({ SPRITE_SCALE, SPRITE_SCALE });
	catSprite.sprite.setOrigin(catSprite.sprite.getLocalBounds().size / 2.f);
	catSprite.visible = true;

	idleTimeLeft = 2.f;

	// text

	tm.setTextContent(textRef, "Yippee! I'm happy to be here!");
}


void Cat::setEntityState(EntityStates s, std::optional<std::reference_wrapper<TextureManager::JS_SPRITE>> opt_sprite) {
	if (entityState == s) return;

	entityState = s;
	entityStateElapsedS = 0.f;

	if (opt_sprite == std::nullopt) return;
}


void Cat::setEntityAnimationState(EntityAnimationStates s, std::optional<std::reference_wrapper<TextureManager::JS_SPRITE>> opt_sprite, int animationLoopCount = -1) {
	if (s == activeAnimationState) return;

	activeAnimationState = s;

	if (opt_sprite == std::nullopt) return;
	TextureManager::JS_SPRITE& sprite = opt_sprite.value();

	//sprite.top = STATE_TOP_MAP.at(s);
	sprite.top = topOffset + (int)s * (yoffset + height);
	sprite.left = leftOffset;
	sprite.numFrames = EntityAnimationDatas.at(s).frameCount;

	if (animationLoopCount == 0) {
		sprite.animationLoopCount = EntityAnimationDatas.at(s).loopCount;
		sprite.staticFrameIdx = EntityAnimationDatas.at(s).staticFrameIdx;
	}
	else {
		sprite.animationLoopCount = animationLoopCount;
	}
	sprite.animationElapsedLoops = 0;
	if (sprite.animationLoopCount != 0) sprite.playingAnimation = true;
}


void Cat::moveTo(float x, float y) {
	if (!alive) return;

	setEntityAnimationState(MOVEMENT_ANIMATION_STATES.at(rand() % (int)MOVEMENT_ANIMATION_STATES.size()), tm.getSprite(catSpriteName), -1);

	//static constexpr int w_offset = width / 2;
	//static constexpr int h_offset = (height / 2) + yoffset;

	target = { x, y };

	//if (facing == FACING_DIRECTIONS::LEFT) {
	//	target.x += w_offset;
	//}
	//else {
	//	target.x -= w_offset;
	//}
	//target.y -= h_offset;

	moveToComplete = false;
}

void Cat::update(float dt) {
	const auto [winX, winY] = win.getSize();
	const auto [mX, mY] = sf::Mouse::getPosition();
	static bool handledStopMoving = true;

	bool isMovingAnimationPlaying = std::find(MOVEMENT_ANIMATION_STATES.begin(), MOVEMENT_ANIMATION_STATES.end(), activeAnimationState) != MOVEMENT_ANIMATION_STATES.end();
	static bool prevIsMovingAnimationPlaying = false;

	// happiness decrement
	happiness -= dt * happiness_drain_rate_s;

	// entity state elapsed time
	entityStateElapsedS += dt;
	idleTimeLeft -= dt;
	timeToNextIdleAnimation -= dt;

	// fade out speech
	sf::Color sfc = speech->getFillColor();
	sf::Color soc = speech->getOutlineColor();
	int alpha = static_cast<int>(sfc.a);
	alpha -= dt * 255.f / SPEECH_FADE_OUT_S;
	if (alpha < 0) alpha = 0;
	sfc.a = alpha;
	speech->setFillColor(sfc);
	soc.a = alpha;
	speech->setOutlineColor(soc);

	if (happiness <= 0) {
		//alive = false;
		happiness = 100;			// !TODO: disable reset for prod
	}

	if (!alive) {
		setEntityState(EntityStates::DEAD, tm.getSprite(catSpriteName));
	}

#pragma region user_dragging
	// allow user to drag cat
	static bool isUserDragging = false;
	static bool handledUserDrag = false;
	static auto lastState = entityState;
	if (!isUserDragging && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && Cat::get().isColliding((sf::Mouse::getPosition() * 1.f))) {
		//std::cout << "dragging" << std::endl;

		if (lastState != EntityStates::DRAGGED) lastState = entityState;		// save last state to revert

		isUserDragging = true;
	}

	if (isUserDragging) {
		pos = { mX * 1.f, mY * 1.f };		// set pos to mouse pos
		// cancel movement
		target = pos;
		handledStopMoving = true;
		moveToComplete = true;
		idleTimeLeft = 0.5f;			// stay for n seconds after user drags

		if (!handledUserDrag) {
			handledUserDrag = true;
			Cat::get().setEntityState(Cat::EntityStates::DRAGGED, tm.getSprite(Cat::get().getCatSpriteName()));
			setEntityAnimationState(STATE_ANIMATION_MAP.at(entityState).at(rand() % (int)STATE_ANIMATION_MAP.at(entityState).size()), tm.getSprite(Cat::get().getCatSpriteName()));
			tm.setTextContent(textRef, STATE_SPEECH_OPTIONS.at(entityState).at(rand() % (int)STATE_SPEECH_OPTIONS.at(entityState).size()));
		}
	}

	if (isUserDragging && !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		//std::cout << "stopped dragging" << std::endl;
		isUserDragging = false;
		handledUserDrag = false;
		setEntityState(lastState, tm.getSprite(catSpriteName));

	}
#pragma endregion user_dragging

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
		setEntityAnimationState(EntityAnimationStates::IDLE, catSprite, 0);
		pos = target;
		moveToComplete = true;
		move_vector = { 0, 0 };
		d = { 0, 0 };
	}

	// set state according to happiness level
	if (!isUserDragging) {
		for (const auto [est, s] : entityStateThresholds) {
			if (happiness <= est) {
				setEntityState(s, catSprite);
				break;
			}
		}

		// just stopped moving animation, play an idle animation dependent on happiness level
		if (!isMovingAnimationPlaying && prevIsMovingAnimationPlaying) {
			const auto& availableAnimations = STATE_ANIMATION_MAP.at(entityState);
			setEntityAnimationState(availableAnimations.at(rand() % (int)availableAnimations.size()), catSprite, 0);

			// set idle time
			idleTimeLeft = (float)(rand() % (MAX_IDLE_TIME - MIN_IDLE_TIME) + MIN_IDLE_TIME);
			//std::cout << "Idling for " << idleTimeLeft << std::endl;

			// talk to user
			tm.setTextContent(textRef, STATE_SPEECH_OPTIONS.at(entityState).at(rand() % (int)STATE_SPEECH_OPTIONS.at(entityState).size()));
		}

		if (idleTimeLeft <= 0) {
			//std::cout << "Idle time over" << std::endl;
			idleTimeLeft = std::numeric_limits<float>::max();
			moveTo(
				winX* RAND_POS_PADDING + rand() % (int)(winX * (1.f - 2.f * RAND_POS_PADDING)),
				winY* RAND_POS_PADDING + rand() % (int)(winY * (1.f - 2.f * RAND_POS_PADDING))
			);
		}
		else {
			if (!isMovingAnimationPlaying && timeToNextIdleAnimation <= 0) {
				timeToNextIdleAnimation = rand() % (MAX_TTNIA - MIN_TTNIA) + MIN_TTNIA;
				const auto& availableAnimations = STATE_ANIMATION_MAP.at(entityState);
				setEntityAnimationState(availableAnimations.at(rand() % (int)availableAnimations.size()), catSprite, 0);
			}
		}
	}

	// summon cat to mouse
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num1)) {
		tm.setTextContent(textRef, "I'm cominggggggggg");
		if ((sf::Vector2f(mX * 1.f, mY * 1.f) - pos).lengthSquared() > std::pow(MOVEMENT_EPSILON, 2.f)) {
			moveTo((float)mX, (float)mY);
		}
	}

	catSprite.sprite.setPosition(pos);


	speech->setPosition({ pos.x + width / 2.f, pos.y - height / 2.f });
	//if (facing == FACING_DIRECTIONS::LEFT) {
	//	speech->setPosition({ pos.x, pos.y });
	//}
	//else {
	//}

	// update prevs static vars
	prevIsMovingAnimationPlaying = isMovingAnimationPlaying;

	// dbg
	//std::cout << catSprite.animationLoopCount << std::endl;
}
