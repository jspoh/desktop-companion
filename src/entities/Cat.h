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
	};

private:

	const int xoffset = 5;
	const int yoffset = 50;
	const int leftOffset = 0;
	const int topOffset = 15;
	const int width = 45;
	const int height = 35;
	const float animationAdvanceTime = 0.1f;

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

public:

	void init();

	void setEntityState(EntityStates s, std::optional<std::reference_wrapper<TextureManager::JS_SPRITE>> opt_sprite);
};
