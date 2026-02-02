#include "pch.h"
#include "Cat.h"


Cat::Cat() {
	const std::string path = "assets/Cats";
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (!entry.is_regular_file()) continue;
		const auto filename = entry.path().filename().string();
		texM.registerTexture(filename, path + "/" + filename);

		texM.createSprite(filename, filename, EntityAnimationDatas.at(activeAnimationState).frameCount, xoffset, leftOffset, topOffset, width, height, true, animationAdvanceTime, false);
		catSpriteRefs.push_back(filename);
	}

	//catTextureRef = catSpriteRefs.at(rand() % (int)catSpriteRefs.size());
	catTextureRef = Settings::catTexRef;
	texM.createSprite(catSpriteName, catTextureRef, EntityAnimationDatas.at(activeAnimationState).frameCount, xoffset, leftOffset, topOffset, width, height, true, animationAdvanceTime, true);
	texM.getSprite(catSpriteName).z = z;

	// text

	speech = &texM.registerText(textRef, "", 16);

	// poop

	bool success = texM.registerTexture(Poop::texRef, "assets/poop.png");
	if (!success) {
		std::cerr << "poop texture registration failed" << std::endl;
	}

	// particles

	texM.registerTexture("heart_particle", "assets/particles/heart.png");
	texM.registerTexture("cat_particle", "assets/particles/cat.png");

	activeParticleTexRef = "cat_particle";

	// --

	recalculateHappiness();

	if (Settings::gf) {
		enableGfLines();
	}
}

Cat& Cat::get() {
	static Cat instance;
	return instance;
}


void Cat::init(bool resetPos) {
	//std::cout << resetPos << std::endl;
	TextureManager::JS_SPRITE& catSprite = texM.getSprite(catSpriteName);
	if (resetPos) {
		pos = Window::get().getWindow().getSize() / 2.f;
		catSprite.sprite.setPosition(pos);
	}
	catSprite.sprite.setScale({ SPRITE_SCALE * Settings::catScale, SPRITE_SCALE * Settings::catScale });
	catSprite.sprite.setOrigin(catSprite.sprite.getLocalBounds().size / 2.f);
	catSprite.visible = true;

	idleTimeLeft = 2.f;

	// text

	if (Settings::catTalks) texM.setTextContent(textRef, Settings::gf ? "Yippee! I'm so happy to see you again bbygirl <3" : "Yippee! I'm so happy to be here!");

	// poop
	Poop::lastPoopTime = std::chrono::system_clock::now();
	Poop::nextPoopTime = Poop::lastPoopTime + std::chrono::seconds(rand() % (Poop::MAX_POOP_INTERVAL_S - Poop::MIN_POOP_INTERVAL_S) + Poop::MIN_POOP_INTERVAL_S);
}

void Cat::enableGfLines() {
	static bool gfe = false;
	if (gfe) return;

	gfe = true;

	for (const auto es : { EntityStates::HAPPY, EntityStates::WANDERING }) {
		STATE_SPEECH_OPTIONS.at(es).push_back("how's my cutie doing?");
		STATE_SPEECH_OPTIONS.at(es).push_back("I really do find you cute all the time");
		STATE_SPEECH_OPTIONS.at(es).push_back("I love youuuu");
		STATE_SPEECH_OPTIONS.at(es).push_back("you are the best!!!");
		STATE_SPEECH_OPTIONS.at(es).push_back("you're so pretty.. yes you ms loh <3");
		STATE_SPEECH_OPTIONS.at(es).push_back("how's your butthole surgery? :]");
		STATE_SPEECH_OPTIONS.at(es).push_back("i'm a pirate arrrrrrrrrrrr");
	}

	for (const auto es : { EntityStates::ON_EDGE, EntityStates::SAD, EntityStates::ANGRY }) {
		STATE_SPEECH_OPTIONS.at(es).push_back("Rest first, i send you heart sticker okay");
		STATE_SPEECH_OPTIONS.at(es).push_back("eat some fruits?");
		//STATE_SPEECH_OPTIONS.at(es).push_back("Break soon okay baby?");
		//STATE_SPEECH_OPTIONS.at(es).push_back("sleep is important okay bby");
		//STATE_SPEECH_OPTIONS.at(es).push_back("baby please take a break, i want you to be happy :)");
	}

	STATE_SPEECH_OPTIONS.at(EntityStates::DRAGGED).push_back("haha okay you can touch me ;]");
	STATE_SPEECH_OPTIONS.at(EntityStates::DRAGGED).push_back("you watch out later i tickle you");
	STATE_SPEECH_OPTIONS.at(EntityStates::DRAGGED).push_back("haha i love picking you up toooo");
	STATE_SPEECH_OPTIONS.at(EntityStates::DRAGGED).push_back("let's always be happy together okay");
	STATE_SPEECH_OPTIONS.at(EntityStates::DRAGGED).push_back("i love youuuuuuuu");
	STATE_SPEECH_OPTIONS.at(EntityStates::DRAGGED).push_back("no angy me okay? :')");
	STATE_SPEECH_OPTIONS.at(EntityStates::DRAGGED).push_back("i love my considerate sensible girl");
	STATE_SPEECH_OPTIONS.at(EntityStates::DRAGGED).push_back("how's your butthole surgery? :]");
}

void Cat::recalculateHappiness() {
	HAPPINESS_LIFESPAN = Settings::MAX_WORK_DURATION_M;
	happiness_drain_rate_s = MAX_HAPPINESS / (HAPPINESS_LIFESPAN * 60.f);
	setHappinessMax();
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

	setEntityAnimationState(MOVEMENT_ANIMATION_STATES.at(rand() % (int)MOVEMENT_ANIMATION_STATES.size()), texM.getSprite(catSpriteName), -1);

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
	if (Settings::onEnforcedBreak) return;

	const auto [winX, winY] = win.getSize();
	const auto [mX, mY] = sf::Mouse::getPosition();
	static bool handledStopMoving = true;

	bool isMovingAnimationPlaying = std::find(MOVEMENT_ANIMATION_STATES.begin(), MOVEMENT_ANIMATION_STATES.end(), activeAnimationState) != MOVEMENT_ANIMATION_STATES.end();
	static bool prevIsMovingAnimationPlaying = false;

	texM.getSprite(catSpriteName).sprite.setScale({ SPRITE_SCALE * Settings::catScale, SPRITE_SCALE * Settings::catScale });

	// happiness decrement
	happiness -= dt * happiness_drain_rate_s;

	// entity state elapsed time
	entityStateElapsedS += dt;
	idleTimeLeft -= dt;
	timeToNextIdleAnimation -= dt;

	// fade out speech
	if (Settings::catTalks) {
		sf::Color sfc = speech->getFillColor();
		sf::Color soc = speech->getOutlineColor();
		int alpha = static_cast<int>(sfc.a);
		alpha -= dt * 255.f / SPEECH_FADE_OUT_S;
		if (alpha < 0) alpha = 0;
		sfc.a = alpha;
		speech->setFillColor(sfc);
		soc.a = alpha;
		speech->setOutlineColor(soc);
	}

	if (happiness <= 0) {
		//alive = false;
		happiness = 0;
	}

	if (!alive) {
		setEntityState(EntityStates::DEAD, texM.getSprite(catSpriteName));
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
			Cat::get().setEntityState(Cat::EntityStates::DRAGGED, texM.getSprite(Cat::get().getCatSpriteName()));
			setEntityAnimationState(STATE_ANIMATION_MAP.at(entityState).at(rand() % (int)STATE_ANIMATION_MAP.at(entityState).size()), texM.getSprite(Cat::get().getCatSpriteName()));
			if (Settings::catTalks) texM.setTextContent(textRef, STATE_SPEECH_OPTIONS.at(entityState).at(rand() % (int)STATE_SPEECH_OPTIONS.at(entityState).size()));
		}
	}

	if (isUserDragging && !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		//std::cout << "stopped dragging" << std::endl;
		isUserDragging = false;
		handledUserDrag = false;
		setEntityState(lastState, texM.getSprite(catSpriteName));

	}
#pragma endregion user_dragging

	TextureManager::JS_SPRITE& catSprite = texM.getSprite(catSpriteName);

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
			catSprite.sprite.setScale({ -SPRITE_SCALE * Settings::catScale, SPRITE_SCALE * Settings::catScale });
			facing = FACING_DIRECTIONS::LEFT;
		}
		else {
			catSprite.sprite.setScale({ SPRITE_SCALE * Settings::catScale, SPRITE_SCALE * Settings::catScale });
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
			if (Settings::catTalks) texM.setTextContent(textRef, STATE_SPEECH_OPTIONS.at(entityState).at(rand() % (int)STATE_SPEECH_OPTIONS.at(entityState).size()));
		}

		if (idleTimeLeft <= 0) {
			//std::cout << "Idle time over" << std::endl;
			idleTimeLeft = std::numeric_limits<float>::max();
			moveTo(
				winX * RAND_POS_PADDING + rand() % (int)(winX * (1.f - 2.f * RAND_POS_PADDING)),
				winY * RAND_POS_PADDING + rand() % (int)(winY * (1.f - 2.f * RAND_POS_PADDING))
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
	if (Settings::catFollowsMouseClick && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		if (Settings::catTalks) texM.setTextContent(textRef, "I'm cominggggggggg");
		if ((sf::Vector2f(mX * 1.f, mY * 1.f) - pos).lengthSquared() > std::pow(MOVEMENT_EPSILON, 2.f)) {
			moveTo((float)mX, (float)mY);
		}
	}

	catSprite.sprite.setPosition(pos);


	if (Settings::catTalks) speech->setPosition({ pos.x + width / 2.f, pos.y - height / 2.f });
	//if (facing == FACING_DIRECTIONS::LEFT) {
	//	speech->setPosition({ pos.x, pos.y });
	//}
	//else {
	//}

	// update prevs static vars
	prevIsMovingAnimationPlaying = isMovingAnimationPlaying;

	// dbg
	//std::cout << catSprite.animationLoopCount << std::endl;


#pragma region poop
	if (Settings::catPoops) {
		auto now = std::chrono::system_clock::now();
		if ((now - Poop::nextPoopTime).count() > 0) {
			Poop::lastPoopTime = now;
			Poop::nextPoopTime = Poop::lastPoopTime + std::chrono::seconds(rand() % (Poop::MAX_POOP_INTERVAL_S - Poop::MIN_POOP_INTERVAL_S) + Poop::MIN_POOP_INTERVAL_S);

			Poop p(pos);

			texM.createSprite(p.spriteRef, p.texRef, 0, 0, 0, 0, p.texWidth, p.texHeight, false, 0.f, true);
			texM.getSprite(p.spriteRef).sprite.setScale({ 1.f / p.texWidth * 30.f * Settings::catScale, 1.f / p.texWidth * 30.f * Settings::catScale });
			texM.getSprite(p.spriteRef).sprite.setPosition(p.pos);

			poops.emplace_back(std::move(p));

			if (Settings::catTalks) texM.setTextContent(textRef, POOP_LINES.at(rand() % (int)POOP_LINES.size()));
			idleTimeLeft = 0.f;
		}

		for (Poop& p : poops) {
			p.elapsed_s += dt;

			if (p.lifespan_s - p.elapsed_s > p.FADE_OUT_IN_LAST_N_SECONDS) {
				continue;
			}

			// fade out poop
			sf::Color color = texM.getSprite(p.spriteRef).sprite.getColor();
			color.a = static_cast<int>(255.f * (p.lifespan_s - p.elapsed_s) / p.FADE_OUT_IN_LAST_N_SECONDS);
			texM.getSprite(p.spriteRef).sprite.setColor(color);
		}

		poops.erase(std::remove_if(poops.begin(), poops.end(), [](const Poop& pp) {
			if (pp.elapsed_s >= pp.lifespan_s) {
				texM.deleteSprite(pp.spriteRef);
				return true;
			}
			return false;
			}), poops.end());
	}


#pragma endregion poop

	// particle sys

	if (Settings::particles) {
		if (Settings::gf) activeParticleTexRef = "heart_particle";
		else activeParticleTexRef = "cat_particle";

		std::deque<TextureManager::JS_PARTICLE>& particles = texM.getParticles(activeParticleTexRef);

		static auto lastSpawnParticleTime = std::chrono::system_clock::now();
		auto now = std::chrono::system_clock::now();
		if (particles.size() < TextureManager::JS_PARTICLE::MAX_PARTICLE_COUNT && std::chrono::duration_cast<std::chrono::milliseconds>(now - lastSpawnParticleTime).count() >= Settings::particleSpawnInterval_s * 1000) {
			lastSpawnParticleTime = now;

			TextureManager::JS_PARTICLE newP;
			newP.origin = pos + particleOffset;
			newP.pos = newP.origin;

			// degrees
			const int randAngleDeg = -120 + rand() % 60;
			const float rot = toRadians((float)randAngleDeg);
			
			newP.d = { cosf(rot), sinf(rot) };

			newP.rotation = toRadians((float)randAngleDeg + 90);

			newP.speed = 50.f;
			newP.accel = rand() % 20 + 20.f;

			newP.initialWidth = Cat::width / 2.f;
			newP.width = newP.initialWidth;

			newP.texRef = activeParticleTexRef;

			particles.push_back(std::move(newP));
		}
	}
}
