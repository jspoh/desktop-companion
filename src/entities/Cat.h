#pragma once

#include "defines.h"
#include "TextureManager.h"

// create Entity parent class if implementing more animals/types


class Cat {
public:
	enum class EntityAnimationStates {
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

	static constexpr int xoffset = 0;
	static constexpr int yoffset = 0;
	static constexpr int leftOffset = 0;
	static constexpr int topOffset = 0;
	static constexpr int width = 64;
	static constexpr int height = 64;
	static constexpr float animationAdvanceTime = 0.1f;

	EntityAnimationStates activeAnimationState = EntityAnimationStates::IDLE;
	float stateElapsedTime{};
	const std::unordered_map<EntityAnimationStates, int> STATE_FRAMES_MAP{
		{EntityAnimationStates::IDLE, 6},
		{EntityAnimationStates::EXCITED, 3},
		{EntityAnimationStates::LAZING, 1 },
		{EntityAnimationStates::SLEEPING, 4},
		{EntityAnimationStates::HAPPY, 10},
		{EntityAnimationStates::RUNNING, 6},
		{EntityAnimationStates::LEAPING, 12},
		{EntityAnimationStates::BOX_1, 12},
		{EntityAnimationStates::BOX_2, 10},
		{EntityAnimationStates::BOX_3, 12},
		{EntityAnimationStates::CRYING, 4},
		{EntityAnimationStates::DANCING, 4},
		{EntityAnimationStates::CHILLING, 8},
		{EntityAnimationStates::SURPRISED, 2},
		{EntityAnimationStates::LAUGHING, 4},
		{EntityAnimationStates::DIE_1, 6},
		{EntityAnimationStates::DIE_2, 6},
		{EntityAnimationStates::HURT, 8},
		{EntityAnimationStates::ATTACK, 7},
	};

	static constexpr const char* catSpriteName = "cat";

	static constexpr float MAX_MOVEMENT_SPEED = 500.f;
	static constexpr float ACCELERATION = 5000.f;

	// for movement
	//float movementSpeed{};
	sf::Vector2f d{ 0, 0 };
	sf::Vector2f move_vector{ 0,0 };	// actual movement vector
	static constexpr sf::Vector2f NO_TARGET{ -1.f, -1.f };
	sf::Vector2f target{ NO_TARGET };
	sf::Vector2f pos{ 100.f, 100.f };
	bool moveToComplete = true;
	static constexpr float MOVEMENT_EPSILON = 25.f;

	static constexpr sf::Vector2f RIGHT_VECTOR{ 1, 0 };


public:

	static Cat& get() {
		static Cat instance;
		return instance;
	}

	void init();

	void update(float dt);

	EntityAnimationStates getActiveState() const {
		return activeAnimationState;
	}

	std::string getCatSpriteName() {
		return catSpriteName;
	}

	void moveTo(float x, float y);

	void setEntityState(EntityAnimationStates s, std::optional<std::reference_wrapper<TextureManager::JS_SPRITE>> opt_sprite);
};
