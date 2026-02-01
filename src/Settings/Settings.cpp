#include "pch.h"
#include "Settings.h"
#include "entities/Cat.h";


void Settings::cleanup() {
	unlockedSkins.clear();
	catTexRef.clear();
	roomTexRef.clear();
	furnitures.clear();
	configFilePath.clear();
}


void Settings::initConfig() {
	// default values
	unlockAll = false;
	coins = 0;
	unlockedSkins = { "AllCats.png" };
	catTexRef = "AllCats.png";
	roomTexRef = "Room1.png";
	catScale = 1.f;
	roomScale = 1.f;
	catFollowsMouseClick = false;
	catTalks = true;
	furnitures = {};
	catPoops = true;
	paused = false;
	gf = false;
}


void Settings::init() {
	namespace fs = std::filesystem;
	char* homeDir = nullptr;
	size_t len = 0;
	if (_dupenv_s(&homeDir, &len, "USERPROFILE") != 0 || !homeDir) {
		std::cerr << "Failed to find home dir" << std::endl;
		throw std::runtime_error("Failed to find home dir");
	}
	const fs::path docsPath = fs::path(homeDir) / "Documents";
	free(homeDir);
	static constexpr const char* configDirName = "jspoh desktop companion";
	if (!fs::exists(docsPath / configDirName)) {
		fs::create_directories(docsPath / configDirName);
	}
	static constexpr const char* configFileName = "jdc";
	configFilePath = docsPath / configDirName / configFileName;
	bool configExists = fs::exists(configFilePath);
	if (configExists) {
		std::ifstream ifs(configFilePath);
		json j;
		ifs >> j;

		unlockAll = j.value("unlockAll", false);
		coins = j.value("coins", 0);
		unlockedSkins = j.value("unlockedSkins", std::unordered_set<std::string>{"AllCats.png"});
		catTexRef = j.value("catTexRef", "AllCats.png");
		roomTexRef = j.value("roomTexRef", "Room1.png");
		catScale = j.value("catScale", 1.f);
		roomScale = j.value("roomScale", 1.f);
		catFollowsMouseClick = j.value("catFollowsMouseClick", false);
		catTalks = j.value("catTalks", true);
		furnitures = j.value("furnitures", std::vector<Room::Furniture>{});
		catPoops = j.value("catPoops", true);
		MAX_WORK_DURATION_M = j.value("MAX_WORK_DURATION_M", 60.f);
		MIN_BREAK_DURATION_M = j.value("MIN_BREAK_DURATION_M", 5.f);
		enforceBreaks = j.value("enforceBreaks", false);
		gf = j.value("gf", false);
		paused = false;
	}
	else {
		initConfig();
		save();
	}

	// init furnitures
	for (const Room::Furniture& f : furnitures) {
		Room::get().addFurniture(f);
	}

	_resetLockedSkins();

#ifdef _DEBUG

#define DBG_ENFORCE
#ifdef DBG_ENFORCE
	MAX_WORK_DURATION_M = 1;
	MIN_BREAK_DURATION_M = 1;
	Cat::get().recalculateHappiness();
#endif

#endif
}


void Settings::_resetLockedSkins() {
	// init locked skins
	lockedSkins.clear();
	for (const auto& sr : Cat::get().catSpriteRefs) {
		if (unlockedSkins.find(sr) != unlockedSkins.end()) {
			lockedSkins.push_back(false);
		}
		else {
			lockedSkins.push_back(true);
		}
	}
}


void Settings::save() {
	json j;
	j["unlockAll"] = unlockAll;
	j["coins"] = coins;
	j["unlockedSkins"] = unlockedSkins;
	j["catTexRef"] = catTexRef;
	j["roomTexRef"] = roomTexRef;
	j["catScale"] = catScale;
	j["roomScale"] = roomScale;
	j["catFollowsMouseClick"] = catFollowsMouseClick;
	j["catTalks"] = catTalks;
	j["furnitures"] = furnitures;
	j["catPoops"] = catPoops;
	j["MAX_WORK_DURATION_M"] = MAX_WORK_DURATION_M;
	j["MIN_BREAK_DURATION_M"] = MIN_BREAK_DURATION_M;
	j["enforceBreaks"] = enforceBreaks;
	j["gf"] = gf;

	std::ofstream ofs(configFilePath);
	ofs << j.dump();
}
