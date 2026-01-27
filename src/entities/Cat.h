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

	//const std::unordered_map<EntityStates, int> STATE_TOP_MAP{
	//	{IDLE, 0},
	//	{EXCITED, 50},
	//	{LAZING, 100},
	//	{SLEEPING, 150},
	//	{HAPPY, 200},
	//	{RUNNING, 250},
	//	{LEAPING, 300},
	//	{BOX_1, 350},
	//	{BOX_2, 400},
	//	{BOX_3, 450},
	//	{CRYING, 500},
	//	{DANCING, 550},
	//	{CHILLING, 600},
	//	{SURPRISED, 650},
	//	{LAUGHING, 700},
	//	{DIE_1, 750},
	//	{DIE_2, 800},
	//	{HURT, 850},
	//	{ATTACK, 900},
	//};

	static constexpr const char* catSpriteName = "cat";

public:

	static Cat& get() {
		static Cat instance;
		return instance;
	}

	void init();

	EntityStates getActiveState() const {
		return activeState;
	}

	std::string getCatSpriteName() {
		return catSpriteName;
	}

	void setEntityState(EntityStates s, std::optional<std::reference_wrapper<TextureManager::JS_SPRITE>> opt_sprite);
};
