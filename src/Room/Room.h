#pragma once

#include "pch.h"
#include "TextureManager.h"


class Room {
public:
	static constexpr int width = 512;
	static constexpr int height = width;

	static constexpr int MAX_WIDTH = 1024;
	static constexpr int MAX_HEIGHT = MAX_WIDTH;

	int localScale;

	class Furniture {
	public:

		struct OffsetData {
			int left, right, width, height;
		};

		// L -> R, T -> B
		enum TYPE {
			WINDOW_1,
			WINDOW_2,
			WINDOW_3,
			WINDOW_4,

			STAND_1,
			STAND_2,
			STAND_3,

			BIG_BALL_1,
			SMALL_BALL_1,

			EXTENDED_STAND_1,
			EXTENDED_STAND_2,
			EXTENDED_STAND_3,

			BIG_WINDOW_1,
			BIG_WINDOW_2,
			BIG_WINDOW_3,
			BIG_WINDOW_4,

			SMALL_WINDOW_1,
			SMALL_WINDOW_2,

			PLANT_1,
			PLANT_2,
			PLANT_3,
			PLANT_4,

			BED_1,
			BED_2,
			BED_3,
			BED_4,
			BED_5,
			BED_6,

			PHOTO_1,
			PHOTO_2,
			PHOTO_3,
			PHOTO_4,
			PHOTO_5,
			PHOTO_6,

			PLAYHOUSE_1,
			PLAYHOUSE_2,

			PAINTING_PINEAPPLE,

			WIDE_WINDOW_1,
			WIDE_WINDOW_2,

			SHELF_1,
			SHELF_2,
			SHELF_3,
			SHELF_4,
			SHELF_5,

			BIG_PLANT_1,
			BIG_PLANT_2,
			BIG_PLANT_3,
			BIG_PLANT_4,

			D_1,
			D_2,

			FOOD_1,
			FOOD_2,
			FOOD_3,
			FOOD_4,
			FOOD_5,
			FOOD_6,
			FOOD_7,
			FOOD_8,
			FOOD_9,
			FOOD_10,
			FOOD_11,
			FOOD_12,

			WATER_1,
			WATER_2,
			WATER_3,
			WATER_4,
			WATER_5,
			WATER_6,

			FOOD_BAG_1,
			FOOD_BAG_2,

			DD_1,
			DD_2,
			DD_3,
			DD_4,
			DD_5,

			DICE_1,
			DICE_2,
			DICE_3,
			DICE_4,

			CHEW_1,
			CHEW_2,
			CHEW_3,
			CHEW_4,
			CHEW_5,
			CHEW_6,

			BOWTIE_1,
			BOWTIE_2,
			BOWTIE_3,
			BOWTIE_4,
			BOWTIE_5,
			BOWTIE_6,
			BOWTIE_7,
			BOWTIE_8,
			BOWTIE_9,
			BOWTIE_10,

			MOUSE,

			CHAIR_1,
			CHAIR_2,
			CHAIR_3,
			CHAIR_4,
			CHAIR_5,

			NUM_FURNITURE_TYPES,
		};

		const std::unordered_map<TYPE, OffsetData> spritesheetOffsets{
			{WINDOW_1, OffsetData{0, 0, 95, 95}},
			{WINDOW_2, OffsetData{95, 0, 95, 95}},
			{WINDOW_3, OffsetData{0, 95, 95, 95}},
			{WINDOW_4, OffsetData{95, 95, 95, 95}},

			{STAND_1, OffsetData{180, 0, 90, 130}},
			{STAND_2, OffsetData{270, 0, 90, 130}},
			{STAND_3, OffsetData{360, 0, 90, 130}},

			{BIG_BALL_1, OffsetData{450, 5, 25, 20}},
			{SMALL_BALL_1, OffsetData{485, 5, 25, 20}},

			{EXTENDED_STAND_1, OffsetData{580, 15, 85, 175}},

			{BIG_WINDOW_1, OffsetData{865, 0, 60, 125}},

			{SMALL_WINDOW_1, OffsetData{10, 225, 40, 65}},

			{PLANT_1, OffsetData{130, 190, 25, 30}},

			{BED_1, OffsetData{200, 135, 110, 85}},

			{PHOTO_1, OffsetData{450, 200, 30, 45}},

			{PAINTING_PINEAPPLE, OffsetData{715, 195, 35, 90}},

			{PLAYHOUSE_1, OffsetData{555, 220, 115, 165}},

			{WIDE_WINDOW_1, OffsetData{675, 305, 90, 140}},

			{BIG_PLANT_1, OffsetData{140, 300, 45, 105}},

			{D_1, OffsetData{200, 435, 55, 75}},

			{FOOD_1, OffsetData{265, 435, 45, 40}},

			{WATER_1, OffsetData{395, 415, 45, 40}},

			{FOOD_BAG_1, OffsetData{525, 550, 35, 50}},

			{DICE_1, OffsetData{195, 615, 20, 15}},

			{CHEW_1, OffsetData{200, 685, 45, 35}},

			{BOWTIE_1, OffsetData{385, 620, 25, 10}},

			{MOUSE, OffsetData{450, 620, 55, 35}},

			{CHAIR_1, OffsetData{675, 610, 60, 60}},


		};
	};

private:
	Room();

	std::vector<std::string> roomRefs;
	std::vector<std::string> furnitureRefs;

public:
	static Room& get() {
		static Room instance;
		return instance;
	}

	static constexpr const char* ref = "room";
	TextureManager::JS_SPRITE* sprite = nullptr;

	void init();

	void update(float dt);

	auto& getRoomRefs() const {
		return roomRefs;
	}

	auto& getFurnitureRefs() const {
		return furnitureRefs;
	}
};
