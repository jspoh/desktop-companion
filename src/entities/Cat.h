#pragma once

#include "defines.h"
#include "TextureManager.h"

// create Entity parent class if implementing more animals/types


class Cat {
public:
	enum EntityStates {
		IDLE = 0,
		EXCITED,
		LAZING,
		SLEEPING,
		HAPPY,
		RUNNING,
		LEAPING,
		BOX_1,
		BOX_2,
		BOX_3,
		CRYING,
		DANCING,
		CHILLING,
		SURPRISED,
		LAUGHING,
		DIE_1,
		DIE_2,
		HURT,
		ATTACK,
		NUM_ENTITY_STATES,
	};

private:

	static constexpr int xoffset = 5;
	static constexpr int yoffset = 50;
	static constexpr int leftOffset = 0;
	static constexpr int topOffset = 5;
	static constexpr int width = 45;
	static constexpr int height = 45;
	static constexpr float animationAdvanceTime = 0.1f;

	EntityStates activeState = IDLE;
	float stateElapsedTime{};
	const std::unordered_map<EntityStates, int> STATE_FRAMES_MAP{
		{IDLE, 6},
		{EXCITED, 3},
		{LAZING, 1 },
		{SLEEPING, 4},
		{HAPPY, 10},
		{RUNNING, 6},
		{LEAPING, 12},
		{BOX_1, 12},
		{BOX_2, 10},
		{BOX_3, 12},
		{CRYING, 4},
		{DANCING, 4},
		{CHILLING, 8},
		{SURPRISED, 2},
		{LAUGHING, 4},
		{DIE_1, 6},
		{DIE_2, 6},
		{HURT, 8},
		{ATTACK, 7},
	};

	static constexpr const char* catSpriteName = "cat";

	static constexpr float MAX_MOVEMENT_SPEED = 250.f;
	static constexpr float ACCELERATION = 150.f;

	// for movement
	float movementSpeed{};
	sf::Vector2f d{ 0, 0 };
	static constexpr sf::Vector2f NO_TARGET{ -1.f, -1.f };
	sf::Vector2f target{ NO_TARGET };
	sf::Vector2f pos{ 100.f, 100.f };

public:

	static Cat& get() {
		static Cat instance;
		return instance;
	}

	void init();

	void update(float dt);

	EntityStates getActiveState() const {
		return activeState;
	}

	std::string getCatSpriteName() {
		return catSpriteName;
	}

	void moveTo(float x, float y);

	void setEntityState(EntityStates s, std::optional<std::reference_wrapper<TextureManager::JS_SPRITE>> opt_sprite);
};
