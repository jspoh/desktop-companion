#include "pch.h"
#include "TextureManager.h"
#include "entities/Cat.h"
#include "Room/Room.h"
#include "BreakData.h"
#include "GameManager.h"


void UI::init() {

}

template<typename fn>
void UI::renderImageButtonsTab(const ImVec2& pos, const std::string& title, const std::string& instruction, const std::vector<std::string>& refs, int entriesPerLine, float w, float h, fn callback, const std::vector<bool>& disabled) {
	if (!ImGui::BeginTabItem(title.c_str())) return;

	ImGui::Text(instruction.c_str());
	ImGui::Separator();

	ImGui::BeginGroup();

	int entries{};
	for (int i{}; i < (int)refs.size(); ++i) {

		const std::string& ref = refs[i];

		if (entries++ % entriesPerLine != 0) ImGui::SameLine();

		//if (disabled.size() && disabled[i]) ImGui::BeginDisabled();
		if (disabled.size() && disabled[i]) {
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.25f, 0.25f, 0.25f, 0.5f));
		}
		if (ImGui::ImageButton(ref.c_str(), texM.getSprite(ref).sprite, sf::Vector2f(w, h))) {
			callback(ref);
		}
		if (disabled.size() && disabled[i]) {
			ImGui::PopStyleColor(3);
		}
		//if (disabled.size() && disabled[i]) ImGui::EndDisabled();

	}
	ImGui::EndGroup();

	ImGui::EndTabItem();


}

template<typename fn>
void UI::renderImageButtonsWindow(const ImVec2& pos, const std::string& title, const std::string& instruction, const std::vector<std::string>& refs, int entriesPerLine, float w, float h, fn callback, const std::vector<bool>& disabled) {
	ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always); // Auto-size

	ImGui::Begin(title.c_str(), nullptr,
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoMove);

	if (ImGui::BeginTabBar(title.c_str())) {
		renderImageButtonsTab(pos, title, instruction, refs, entriesPerLine, w, h, callback, disabled);
		ImGui::EndTabBar();
	}

	lastWinSize.y += ImGui::GetWindowSize().y + PADDING.y;
	ImGui::End();
}

void UI::_furniturePurchase() {
	if (!ImGui::BeginTabItem("Furniture shop")) return;

	ImGui::Text("Click to purchase (Cost: %d)", Room::Furniture::COST);
	ImGui::Separator();

	static constexpr int NUM_COLUMNS = 6;
	static constexpr int CELL_WIDTH = 50;
	ImGui::BeginChild("FurnitureGrid", ImVec2(NUM_COLUMNS * CELL_WIDTH + 120, 200), true);
	ImGui::BeginGroup();

	int entries{};
	sf::Sprite tempSprite(texM.getTexture("all_furnitures"));
	for (auto& [type, od] : Room::Furniture::spritesheetOffsets) {
		if (entries++ % NUM_COLUMNS != 0) ImGui::SameLine();

		tempSprite.setTextureRect(sf::IntRect({ od.left, od.top }, { od.width, od.height }));

		if (ImGui::ImageButton(("furniture" + std::to_string(entries)).c_str(), tempSprite, sf::Vector2f(CELL_WIDTH, CELL_WIDTH))) {
			if (Settings::coins < Room::Furniture::COST) {
				if (Settings::catTalks) texM.setTextContent(Cat::getTextRef(), "You can't afford this :(");
				continue;
			}

			Settings::coins -= Room::Furniture::COST;

			Room::get().addFurniture(type);

			Settings::save();
		}
	}
	ImGui::EndGroup();
	ImGui::EndChild();
	ImGui::EndTabItem();
}

void UI::_furnitureInventoryTab() {
	if (!ImGui::BeginTabItem("Furniture inventory")) return;

	ImGui::Text("Manage inventory", Room::Furniture::COST);
	ImGui::Separator();

	static constexpr int NUM_COLUMNS = 6;
	static constexpr int CELL_WIDTH = 50;
	ImGui::BeginChild("FurnitureGrid_inv", ImVec2(NUM_COLUMNS * CELL_WIDTH + 120, 200), true);
	ImGui::BeginGroup();

	int entries{};
	sf::Sprite tempSprite(texM.getTexture("all_furnitures"));
	for (Room::Furniture& f : Settings::furnitures) {
		if (!f.inInventory) continue;

		auto& od = Room::Furniture::spritesheetOffsets.at(f.type);

		if (entries++ % NUM_COLUMNS != 0) ImGui::SameLine();

		tempSprite.setTextureRect(sf::IntRect({ od.left, od.top }, { od.width, od.height }));

		if (ImGui::ImageButton(("furniture_inv_" + std::to_string(entries)).c_str(), tempSprite, sf::Vector2f(CELL_WIDTH, CELL_WIDTH))) {
			f.inInventory = false;

			Settings::save();
		}
	}
	ImGui::EndGroup();
	ImGui::EndChild();
	ImGui::EndTabItem();
}

void UI::skinSelect() {
	std::string setSkinRef{};

	auto triggerOnChange = [&setSkinRef](const std::string& ref) {
		setSkinRef = ref;
		};

	auto onChange = [](const std::string& ref) {
		if (Settings::unlockedSkins.find(ref) == Settings::unlockedSkins.end()) {

			// user doesnt own skin

			if (Settings::coins < Cat::SKIN_COST) {
				if (Settings::catTalks) texM.setTextContent(Cat::getTextRef(), "You can't afford this :(");
				return;
			}

			Settings::coins -= Cat::SKIN_COST;
			Settings::unlockedSkins.insert(ref);
		}

		texM.getSprite(Cat::get().getCatSpriteName()).sprite.setTexture(texM.getTexture(ref));
		Cat::get().init(false);

		Settings::catTexRef = ref;
		Settings::save();

		Settings::_resetLockedSkins();

		};

	std::stringstream ss;
	ss << "Click to select/purchase skin. (Cost: " << Cat::SKIN_COST << ")";

	renderImageButtonsWindow(lastWinSize, "Companion skin", ss.str(), Cat::get().catSpriteRefs, 999, (float)Cat::get().getWidth(), (float)Cat::get().getHeight(), triggerOnChange, Settings::lockedSkins);

	// do work (need to reset disabled vector, so unsafe to perform in the renderImageButtonsWindow function)
	if (setSkinRef.size()) onChange(setSkinRef);
}

void UI::_roomSelectTab() {
	auto onChange = [](const std::string& ref) {
		Room::get().sprite->sprite.setTexture(texM.getTexture(ref));
		Settings::roomTexRef = ref;
		Settings::save();
		};


	renderImageButtonsTab(lastWinSize, "Paint", "Select color", Room::get().getRoomRefs(), 6, 50.f, 50.f, onChange, {});
}

void UI::settingsView() {
	ImGui::SetNextWindowPos(lastWinSize, ImGuiCond_Always, orientation);
	ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always); // Auto-size

	ImGui::Begin("Settings", nullptr,
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoMove);

	bool changed = false;
	if (ImGui::Checkbox("Paused", &Settings::paused));
	if (ImGui::Checkbox("Summon cat to mouse on left click", &Settings::catFollowsMouseClick)) changed = true;
	if (ImGui::Checkbox("Cat talks", &Settings::catTalks)) changed = true;
	if (ImGui::Checkbox("Catpoop", &Settings::catPoops)) changed = true;

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	if (ImGui::DragFloat("Cat scale", &Settings::catScale, 0.01f, 0.1f, 10.f, "%.1f")) changed = true;
	//if (ImGui::DragFloat("Room scale", &Settings::roomScale, 0.01f, 1.f, 2.f, "%.1f")) changed = true;

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	if (ImGui::InputInt("Max working duration without breaks (minutes)", &Settings::MAX_WORK_DURATION_M)) {
		changed = true;

		if (Settings::MAX_WORK_DURATION_M < 5) {
			Settings::MAX_WORK_DURATION_M = 5;
		}

		Cat::get().recalculateHappiness();
	}
	if (ImGui::InputInt("Min break duration (minutes)", &Settings::MIN_BREAK_DURATION_M)) changed = true;

	ImGui::Spacing();

	ImGui::TextColored({ 1, 0, 0, 1 }, "Warning: You will be locked out of your computer until the mandated min break duration has elapsed.");
	if (ImGui::Checkbox("Enforce breaks", &Settings::enforceBreaks)) changed = true;


	lastWinSize.y += ImGui::GetWindowSize().y + PADDING.y;

	ImGui::End();

	if (changed) {
		if (Settings::MIN_BREAK_DURATION_M < 1) {
			Settings::MIN_BREAK_DURATION_M = 1;
		}

		Settings::save();
	}
}

void UI::coinsView() {
	ImGui::SetNextWindowPos(lastWinSize, ImGuiCond_Always, orientation);
	ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always); // Auto-size

	ImGui::Begin("Coins", nullptr,
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoMove);

	std::stringstream ss;
	ss << "Coins: " << Settings::coins;
	ImGui::Text(ss.str().c_str());

	//#ifdef _DEBUG
	//		if (ImGui::Button("hackerman")) {
	//			static constexpr int ADD_AMOUNT = 1000;
	//			if (Settings::coins + ADD_AMOUNT <= std::numeric_limits<int>::max())
	//				Settings::coins += ADD_AMOUNT;
	//			Settings::save();
	//		}
	//#endif
			//static bool prev1Pressed = false;
			//if (!prev1Pressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num1)) {
			//	static constexpr int ADD_AMOUNT = 1000;
			//	if (Settings::coins + ADD_AMOUNT <= std::numeric_limits<int>::max())
			//		Settings::coins += ADD_AMOUNT;
			//	Settings::save();

			//	prev1Pressed = true;
			//}
			//else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num1)) {
			//	prev1Pressed = false;
			//}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// time left before break
	const float totalSecondsLeft = Cat::get().getHappinessPercentage() * Settings::MAX_WORK_DURATION_M * 60;
	const int iTotalSecondsLeft = std::ceil(totalSecondsLeft);


	const int iHoursLeft = iTotalSecondsLeft / 3600;
	const int iMinsLeft = (iTotalSecondsLeft - (iHoursLeft * 3600)) / 60;
	const int iSecsLeft = iTotalSecondsLeft - (iHoursLeft * 3600) - (iMinsLeft * 60);

	ImGui::TextColored({ 0, 1, 0, 1 }, "Time to next break: %02d:%02d:%02d", iHoursLeft, iMinsLeft, iSecsLeft);
	ImGui::Text("Take breaks punctually to earn coins!");

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::Text("Past %d breaks (voluntary):", gm.MAX_BREAKDATA_SIZE);
	ImGui::Spacing();

	for (const BreakData& bd : gm.breaks) {

		const std::time_t st = static_cast<std::time_t>(bd.start);
		const std::time_t et = static_cast<std::time_t>(bd.end);

		std::tm tm_st, tm_et;

		if (localtime_s(&tm_st, &st) || localtime_s(&tm_et, &et)) {
			std::cerr << "ui.h > UI::coinsView localtime_s is broken" << std::endl;
			ImGui::Text("Error with localtime_s. Inform developer(js)!");
			lastWinSize.y += ImGui::GetWindowSize().y + PADDING.y;
			ImGui::End();
			return;
		}

		ImGui::Text("%02d:%02d:%02d - %02d:%02d:%02d", tm_st.tm_hour, tm_st.tm_min, tm_st.tm_sec, tm_et.tm_hour, tm_et.tm_min, tm_et.tm_sec);
	}

	lastWinSize.y += ImGui::GetWindowSize().y + PADDING.y;
	ImGui::End();
}

void UI::room() {
	ImGui::SetNextWindowPos(lastWinSize, ImGuiCond_Always, orientation);
	ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always); // Auto-size

	ImGui::Begin("Room", nullptr,
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoMove);

	if (ImGui::BeginTabBar("Room options")) {
		// tabs
		_roomSelectTab();
		_furniturePurchase();
		_furnitureInventoryTab();

		ImGui::EndTabBar();
	}



	lastWinSize.y += ImGui::GetWindowSize().y + PADDING.y;
	ImGui::End();
}

void UI::cheatcode() {
	ImGui::SetNextWindowPos(lastWinSize, ImGuiCond_Always, orientation);
	ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always); // Auto-size

	ImGui::Begin("Cheat", nullptr,
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoMove);

	static constexpr int IPT_SIZE = 20;
	static char buf[IPT_SIZE] = {};

	ImGui::Text("Code:");
	ImGui::InputText("##cheatcode", buf, IPT_SIZE);
	if (ImGui::Button("hackerman")) {
		if (!strcmp(buf, "kaching")) {
			static constexpr int ADD_AMOUNT = 10000;
			if (Settings::coins + ADD_AMOUNT <= std::numeric_limits<int>::max())
				Settings::coins += ADD_AMOUNT;
			Settings::save();
		}
		else if (!strcmp(buf, "exit")) {
			Settings::onEnforcedBreak = false;
			Cat::get().setHappinessMax();
		}
		else if (!strcmp(buf, "lohyilin")) {
			Settings::gf = true;
			Settings::save();

			texM.setTextContent(Cat::getTextRef(), "Hey babygirl :)");

			Cat::get().enableGfLines();
		}

		buf[0] = '\0';
	}

	lastWinSize.y += ImGui::GetWindowSize().y + PADDING.y;

	ImGui::End();
}

void UI::manageFurniture() {
	if (Room::get().selectedFurnitureIdx == -1) return;

	ImGui::SetNextWindowPos(lastWinSize, ImGuiCond_Always, orientation);
	ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always); // Auto-size

	ImGui::Begin("Furniture Management", nullptr,
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoMove);

	Room::Furniture& f = Settings::furnitures[Room::get().selectedFurnitureIdx];


	sf::Sprite tempSprite(texM.getTexture("all_furnitures"));
	auto& od = Room::Furniture::spritesheetOffsets.at(f.type);
	tempSprite.setTextureRect(sf::IntRect({ od.left, od.top }, { od.width, od.height }));

	ImGui::Image(tempSprite);

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	bool changed = false;
	if (ImGui::Checkbox("Flip horizontally", &f.mirrored)) changed = true;
	ImGui::Text("Z value (larger values are on top)");
	if (ImGui::InputInt("##Z value (larger values are on top)", &f.z)) changed = true;
	if (ImGui::Checkbox("Keep in inventory", &f.inInventory)) changed = true;

	if (changed) {
		if (texM.getSprite(f.spriteRef).z != f.z) {
			texM.getSprite(f.spriteRef).z = f.z;
			texM.sortSprites();
		}
		Settings::save();
	}

	lastWinSize.y += ImGui::GetWindowSize().y + PADDING.y;
	ImGui::End();
}

void UI::render() {
	if (!Settings::onEnforcedBreak) {
		// left side of screen
		lastWinSize = PADDING;
		orientation = { 0, 0 };

		coinsView();
		skinSelect();
		room();

		// right side of screen
		lastWinSize = { ImGui::GetIO().DisplaySize.x - PADDING.x, PADDING.y };
		orientation = { 1, 0 };

		settingsView();
		manageFurniture();
	}

	// top center of screen
	lastWinSize = { ImGui::GetIO().DisplaySize.x / 2.f, PADDING.y };
	orientation = { 0.5f, 0 };

	cheatcode();
}