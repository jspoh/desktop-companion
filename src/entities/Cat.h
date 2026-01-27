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

	const int xoffset = 5;
	const int yoffset = 50;
	const int leftOffset = 0;
	const int topOffset = 15;
	const int width = 45;
	const int height = 35;
	const float animationAdvanceTime = 0.1f;

	EntityStates activeState = IDLE;
	const std::unordered_map<EntityStates, int> STATE_FRAMES_MAP{
		{IDLE, 6}
	};

	void init();
};
