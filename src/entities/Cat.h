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
		ON_EDGE,
		ANGRY,
		DEAD,
		DRAGGED,		// if dragged by mouse
		NUM_ENTITY_STATES,
	};

	std::vector<std::string> catSpriteRefs;		// sprite and texture refs share same name

private:

	Cat();

	sf::Text* speech;
	static constexpr const char* textRef = "cat_speech";
	static constexpr float SPEECH_FADE_OUT_S = 5.f;

	bool alive = true;

	struct EntityAnimationData {
		EntityAnimationData(EntityAnimationStates s, int frames, int loops, int staticIdx)
			: state{ s }, frameCount{ frames }, loopCount{ loops }, staticFrameIdx{ staticIdx }
		{
		}

		EntityAnimationStates state{};
		int frameCount{};
		int loopCount{};
		int staticFrameIdx{};
	};

	const std::unordered_map<EntityAnimationStates, EntityAnimationData> EntityAnimationDatas{
	{EntityAnimationStates::IDLE,{EntityAnimationStates::IDLE, 6, -1, 0}},
	{EntityAnimationStates::EXCITED,{EntityAnimationStates::EXCITED, 3, -1, 0}},
	{EntityAnimationStates::LAZING,{EntityAnimationStates::LAZING, 1 , -1, 0}},
	{EntityAnimationStates::SLEEPING,{EntityAnimationStates::SLEEPING, 4, -1, 0}},
	{EntityAnimationStates::HAPPY,{EntityAnimationStates::HAPPY, 10, -1, 0}},
	{EntityAnimationStates::RUNNING,{EntityAnimationStates::RUNNING, 6, -1, 0}},
	{EntityAnimationStates::LEAPING,{EntityAnimationStates::LEAPING, 12, -1, 0}},
	{EntityAnimationStates::BOX_1,{EntityAnimationStates::BOX_1, 12, -1, 0}},
	{EntityAnimationStates::BOX_2,{EntityAnimationStates::BOX_2, 10, -1, 0}},
	{EntityAnimationStates::BOX_3,{EntityAnimationStates::BOX_3, 12, -1, 0}},
	{EntityAnimationStates::CRYING,{EntityAnimationStates::CRYING, 4, -1, 0}},
	{EntityAnimationStates::DANCING,{EntityAnimationStates::DANCING, 4, -1, 0}},
	{EntityAnimationStates::CHILLING,{EntityAnimationStates::CHILLING, 8, -1, 0}},
	{EntityAnimationStates::SURPRISED,{EntityAnimationStates::SURPRISED, 2, -1, 0}},
	{EntityAnimationStates::LAUGHING,{EntityAnimationStates::LAUGHING, 4, -1, 0}},
	{EntityAnimationStates::DIE_1,{EntityAnimationStates::DIE_1, 6, 1, 5}},
	{EntityAnimationStates::DIE_2,{EntityAnimationStates::DIE_2, 5, 1, 4}},
	{EntityAnimationStates::HURT,{EntityAnimationStates::HURT, 8, -1, 0}},
	{EntityAnimationStates::ATTACK,{EntityAnimationStates::ATTACK, 7, -1, 0}},
	};

	const std::map<int, EntityStates> entityStateThresholds{
		{std::numeric_limits<int>::max(), EntityStates::HAPPY},
		{80, EntityStates::WANDERING},
		{20, EntityStates::ON_EDGE},
		{10, EntityStates::SAD},
		{0, EntityStates::ANGRY},
		//{0, EntityStates::DEAD}
		{std::numeric_limits<int>::min(), EntityStates::SAD},
	};

	const std::unordered_map<EntityStates, std::vector<EntityAnimationStates>> STATE_ANIMATION_MAP{
		{EntityStates::HAPPY,			// movement to leaping
			{
			EntityAnimationStates::EXCITED,
			//EntityAnimationStates::DANCING,
			EntityAnimationStates::SURPRISED,
			//EntityAnimationStates::LAUGHING,
			EntityAnimationStates::CHILLING,
			EntityAnimationStates::IDLE,
			EntityAnimationStates::LAZING,
			EntityAnimationStates::SLEEPING,
			EntityAnimationStates::HAPPY,
		}
	},
	{EntityStates::WANDERING,
		{
			EntityAnimationStates::LAZING,
			EntityAnimationStates::SLEEPING,
			EntityAnimationStates::CHILLING,
			EntityAnimationStates::IDLE,
			EntityAnimationStates::HAPPY,
		}
	},
	{EntityStates::ON_EDGE,
		{
			EntityAnimationStates::IDLE,
		}
	},
	{EntityStates::SAD,
		{
			EntityAnimationStates::CRYING,
			EntityAnimationStates::CRYING,
			EntityAnimationStates::IDLE,
		}
	},
	{EntityStates::ANGRY,		// ignored, chases mouse instead
		{
			EntityAnimationStates::CRYING,
		}
	},
	{EntityStates::DEAD,
		{
			//EntityAnimationStates::DIE_1,
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
		//EntityAnimationStates::LEAPING,
		EntityAnimationStates::RUNNING,
	};

	const std::unordered_map<EntityStates, std::vector<std::string>> STATE_SPEECH_OPTIONS{
		{EntityStates::HAPPY, {
			"Press ` or ~ to see my room ;D",
			"I am nature's most ferocious hunter",
			"meow meow pussycat",
			"body scrubbers? who needs them?",
			"*yawns",
			"bow down to me puny human",
			"I once caught a mouse all by myself",
			"My secret is no cigarette",
			"mmm french toast",
			"I pooped a real big one the other day",
			"What is a wheel?",
			"I am the prettiest cat",
			"I am the prettiest dog",
			"I will eat your homework",
			"MY PARCEL IS HEREE",
			"i am microwave mmmmmmmmmmmmm",
	}},
		{
			EntityStates::WANDERING, {
			"Press ` or ~ to see my room ;D",
			"I wonder what's over here",
			"What are you doing? :)",
			"Being productive?",
			"Study hard!",
			"FOCUSSS",
			"Stay hydrated"
			"I am so smart",
			"I once caught a mouse all by myself",
			"So why did the chicken cross the road?",
			"God I am cute",
			"I rubbed my butt on your pillow",
			"Am I a boy cat or a girl cat?",
			"omnomnom",
			"why do japanese cats say nya",
			"i am so fast",
			"i kinda like that uncle roger guy"
}
},
		{
			EntityStates::ON_EDGE, {
			"I'm getting a little tired",
			"Time for a break?",
			"I could really use a good stretch right now",
			"I'm tired of sitting",
			"I want to stand!",
			"Rest your eyes a little?",
			"You worked hard, you deserve some rest",
			"Sitting too long is bad",
			"Please rest :)",
			"Take a break, come back fresher?",
			"Toilet breaaakk"
}
},
		{
			EntityStates::SAD, {
			"Please? Just a 5 minute break? For me?",
			"You're going to make me cry..",
			":(",
			"Break?",
			"..."
}
},
		{
			EntityStates::ANGRY, {
			"Break nowwwwwwwwwww",
			"I will snatch your mouse",
			"Time for a break lets gooooo",
			"BREAK BREAK BREAK",
			"IM HUNGRY LETS GO GET SOME FOOD"
}
},
		{
			EntityStates::DRAGGED, {
			"Where are we going? :D",
			"Stop touching meeee I'm ticklish",
			"hiiiiiiiiiiii",
			"lets gooo :D",
			"that tickles!",
			"be careful!"
			"LET ME GOOO",
			"UNHAND ME PEASANT",
			"AAAAAAAAAAAAAA",
			"Pretty sure I was sleeping",
			"you touch me too much",
			"PUT ME DOWN",
}
}
	};

	static constexpr float MAX_HAPPINESS = 100.f;
	float happiness{ MAX_HAPPINESS };
	float HAPPINESS_LIFESPAN = 60.f;		// how many minutes before happiness fully drains
	float happiness_drain_rate_s = MAX_HAPPINESS / (HAPPINESS_LIFESPAN * 60.f);		// how much happiness drains per second

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
	static constexpr const char* catSpriteName = "cat";
	std::string catTextureRef = "";

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
	static constexpr int MIN_IDLE_TIME = 20;
	static constexpr int MAX_IDLE_TIME = 60;

	float timeToNextIdleAnimation{};
	static constexpr int MIN_TTNIA = 5;
	static constexpr int MAX_TTNIA = 20;

	static constexpr float SPRITE_SCALE = 1.f;

	static constexpr float RAND_POS_PADDING = 0.05f;		// do not go to extreme 5% of screen
public:

	static Cat& get() {
		static Cat instance;
		return instance;
	}

	void init(bool resetPos = true);

	void update(float dt);

	int getWidth() const {
		return width;
	}

	int getHeight() const {
		return height;
	}

	EntityAnimationStates getAnimationState() const {
		return activeAnimationState;
	}

	EntityStates getEntityState() const {
		return entityState;
	}

	std::string getCatSpriteName() {
		return catSpriteName;
	}

	//void setCatSpriteName(const std::string& sn) {
	//	catSpriteName = sn;
	//}

	void moveTo(float x, float y);

	void setEntityState(EntityStates s, std::optional<std::reference_wrapper<TextureManager::JS_SPRITE>> opt_sprite);

	void setEntityAnimationState(EntityAnimationStates s, std::optional<std::reference_wrapper<TextureManager::JS_SPRITE>> opt_sprite, int animationLoopCount);

	bool isColliding(const sf::Vector2f& p) {
		if (p.x < pos.x - width / 2.f || p.x > pos.x + width / 2.f) return false;
		if (p.y < pos.y - height / 2.f || p.y > pos.y + height / 2.f) return false;
		return true;
	}
};
