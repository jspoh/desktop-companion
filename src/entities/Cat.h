#pragma once

#include "pch.h"
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

	enum class EntityStates {
		IDLE,
		WANDERING,
		HAPPY,		// BOX, LEAPING movement, DANCING, 
		SAD,
		HURTING,
		ANGRY,
		DEAD,
		DRAGGED,		// if dragged by mouse
		NUM_ENTITY_STATES,
	};

private:

	const std::map<int, EntityStates> entityStateThresholds{
		{std::numeric_limits<int>::max(), EntityStates::HAPPY},
		{80, EntityStates::WANDERING},
		{30, EntityStates::SAD},
		{15, EntityStates::HURTING},
		{10, EntityStates::ANGRY},
		{0, EntityStates::DEAD}
	};

	const std::unordered_map<EntityStates, std::vector<EntityAnimationStates>> STATE_ANIMATION_MAP{
		{EntityStates::HAPPY,			// movement to leaping
			{
			//EntityAnimationStates::EXCITED,
			EntityAnimationStates::DANCING,
			EntityAnimationStates::SURPRISED,
			EntityAnimationStates::LAUGHING,
			EntityAnimationStates::CHILLING,
		}
	},
	{EntityStates::WANDERING,
		{
			EntityAnimationStates::LAZING,
			EntityAnimationStates::SLEEPING,
			EntityAnimationStates::CHILLING,
			EntityAnimationStates::IDLE,
		}
	},
	{EntityStates::SAD,
		{
			EntityAnimationStates::CRYING,
			EntityAnimationStates::IDLE,
		}
	},
	{EntityStates::HURTING,		// movement type to the hurting
		{
			EntityAnimationStates::CRYING,
			EntityAnimationStates::IDLE,
		}
	},
	{EntityStates::ANGRY,
		{
			EntityAnimationStates::CRYING,
			EntityAnimationStates::IDLE,
		}
	},
	{EntityStates::DEAD,
		{
			EntityAnimationStates::DIE_1,
			EntityAnimationStates::DIE_2,
		}
	},
	{EntityStates::DRAGGED,
		{
			EntityAnimationStates::EXCITED,
		}
	},
	};

	const std::vector<EntityAnimationStates> MOVEMENT_ANIMATION_STATES{
		EntityAnimationStates::LEAPING,
		EntityAnimationStates::RUNNING,
	};

	float happiness{ 100 };
	float HAPPINESS_LIFESPAN = 10;		// how many minutes before happiness fully drains
	float happiness_drain_rate_s = happiness / (HAPPINESS_LIFESPAN * 60.f);		// how much happiness drains per second

	void setHappiness(int nh) {
		happiness = nh;
		happiness_drain_rate_s = happiness / (HAPPINESS_LIFESPAN * 60.f);
	}

	void setHappinessLifespan(int nh) {
		HAPPINESS_LIFESPAN = nh;
		happiness_drain_rate_s = happiness / (HAPPINESS_LIFESPAN * 60.f);
	}

	EntityStates entityState = EntityStates::WANDERING;
	float entityStateElapsedS{};

	static constexpr int xoffset = 0;
	static constexpr int yoffset = 0;
	static constexpr int leftOffset = 0;
	static constexpr int topOffset = 0;
	static constexpr int width = 64;
	static constexpr int height = 64;
	static constexpr float animationAdvanceTime = 0.1f;

	enum FACING_DIRECTIONS {
		LEFT,
		RIGHT
	};

	FACING_DIRECTIONS facing = FACING_DIRECTIONS::RIGHT;

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
	static constexpr float ACCELERATION = MAX_MOVEMENT_SPEED * 10.f;
	static constexpr float MAX_WANDER_SPEED = MAX_MOVEMENT_SPEED / 5.f;

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

	float idleTimeLeft{};	// how much time Cat can idle before moving to another position
	static constexpr int MIN_IDLE_TIME = 2;
	static constexpr int MAX_IDLE_TIME = 5;

	static constexpr float SPRITE_SCALE = 1.f;

	static constexpr float RAND_POS_PADDING = 0.05;		// do not go to extreme 5% of screen
public:

	static Cat& get() {
		static Cat instance;
		return instance;
	}

	void init();

	void update(float dt);

	EntityAnimationStates getAnimationState() const {
		return activeAnimationState;
	}

	EntityStates getEntityState() const {
		return entityState;
	}

	std::string getCatSpriteName() {
		return catSpriteName;
	}

	void moveTo(float x, float y);

	void setEntityState(EntityStates s, std::optional<std::reference_wrapper<TextureManager::JS_SPRITE>> opt_sprite);

	void setEntityAnimationState(EntityAnimationStates s, std::optional<std::reference_wrapper<TextureManager::JS_SPRITE>> opt_sprite, int animationLoopCount);

	bool isColliding(const sf::Vector2f& p) {
		if (p.x < pos.x || p.x > pos.x + width) return false;
		if (p.y < pos.y || p.y > pos.y + height) return false;
		return true;
	}
};
