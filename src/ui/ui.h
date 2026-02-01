#pragma once


#include "pch.h"
#include "TextureManager.h"
#include "entities/Cat.h"
#include "Room/Room.h"


class UI {
private:
	inline static const ImVec2 PADDING{ 5, 5 };
	inline static ImVec2 lastWinSize{ PADDING };

public:
	static void init() {

	}

	template<typename fn>
	static void renderImageButtons(const ImVec2& pos, const std::string& title, const std::string& instruction, const std::vector<std::string>& refs, int entriesPerLine, float w, float h, fn callback) {
		ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always); // Auto-size

		ImGui::Begin(title.c_str(), nullptr,
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoMove);

		ImGui::Text(instruction.c_str());
		ImGui::Separator();

		ImGui::BeginGroup();

		int entries{};
		for (const std::string& ref : refs) {
			if (entries++ % entriesPerLine != 0) ImGui::SameLine();

			if (ImGui::ImageButton(ref.c_str(), tm.getSprite(ref).sprite, sf::Vector2f(w, h))) {
				callback(ref);
			}
		}
		ImGui::EndGroup();

		lastWinSize.y += ImGui::GetWindowSize().y + PADDING.y;

		ImGui::End();
	}

	static void furniturePurchase() {
		ImGui::SetNextWindowPos(lastWinSize, ImGuiCond_Always, { 0, 0 });
		ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always); // Auto-size

		ImGui::Begin("Furniture Shop", nullptr,
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoMove);

		ImGui::Text("Click to purchase (Cost: %d)", Room::Furniture::COST);
		ImGui::Separator();

		static constexpr int NUM_COLUMNS = 6;
		static constexpr int CELL_WIDTH = 50;
		ImGui::BeginChild("FurnitureGrid", ImVec2(NUM_COLUMNS * CELL_WIDTH + 120, 200), true);
		ImGui::BeginGroup();

		int entries{};
		sf::Sprite tempSprite(tm.getTexture("all_furnitures"));
		for (auto& [type, od] : Room::Furniture::spritesheetOffsets) {
			if (entries++ % NUM_COLUMNS != 0) ImGui::SameLine();

			tempSprite.setTextureRect(sf::IntRect({ od.left, od.top }, { od.width, od.height }));

			if (ImGui::ImageButton(("furniture" + std::to_string(entries)).c_str(), tempSprite, sf::Vector2f(CELL_WIDTH, CELL_WIDTH))) {
				if (Settings::coins < Room::Furniture::COST) {
					tm.setTextContent(Cat::getTextRef(), "You can't afford this :(");
					continue;
				}

				Settings::coins -= Room::Furniture::COST;

				Room::get().addFurniture(type);

				Settings::save();
			}
		}
		ImGui::EndGroup();
		ImGui::EndChild();

		lastWinSize.y += ImGui::GetWindowSize().y + PADDING.y;

		ImGui::End();
	}

	static void furnitureInventory() {
		ImVec2 displaySize = ImGui::GetIO().DisplaySize;

		ImGui::SetNextWindowPos(lastWinSize, ImGuiCond_Always, { 1, 0 });
		ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always); // Auto-size

		ImGui::Begin("Furniture Inventory", nullptr,
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoMove
		);

		ImGui::Text("Manage inventory", Room::Furniture::COST);
		ImGui::Separator();

		static constexpr int NUM_COLUMNS = 6;
		static constexpr int CELL_WIDTH = 50;
		ImGui::BeginChild("FurnitureGrid_inv", ImVec2(NUM_COLUMNS * CELL_WIDTH + 120, 200), true);
		ImGui::BeginGroup();

		int entries{};
		sf::Sprite tempSprite(tm.getTexture("all_furnitures"));
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

		lastWinSize.y += ImGui::GetWindowSize().y + PADDING.y;

		ImGui::End();
	}

	static void skinSelect() {
		auto onChange = [](const std::string& ref) {
			tm.getSprite(Cat::get().getCatSpriteName()).sprite.setTexture(tm.getTexture(ref));
			Cat::get().init(false);
			};

		renderImageButtons(lastWinSize, "Companion skin", "Select skin", Cat::get().catSpriteRefs, 999, (float)Cat::get().getWidth(), (float)Cat::get().getHeight(), onChange);
	}

	static void roomSelect() {
		auto onChange = [](const std::string& ref) {
			Room::get().sprite->sprite.setTexture(tm.getTexture(ref));
			};

		renderImageButtons(lastWinSize, "Room", "Select color", Room::get().getRoomRefs(), 3, 50.f, 50.f, onChange);
	}

	static void settingsView() {
		ImGui::SetNextWindowPos(lastWinSize, ImGuiCond_Always, { 0, 0 });
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


		lastWinSize.y += ImGui::GetWindowSize().y + PADDING.y;

		ImGui::End();

		if (changed) Settings::save();
	}

	static void coinsView() {
		ImGui::SetNextWindowPos(lastWinSize, ImGuiCond_Always, { 0, 0 });
		ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always); // Auto-size

		ImGui::Begin("Coins", nullptr,
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoMove);

		std::stringstream ss;
		ss << "Coins: " << Settings::coins;
		ImGui::Text(ss.str().c_str());

		lastWinSize.y += ImGui::GetWindowSize().y + PADDING.y;

		ImGui::End();
	}

	static void render() {
		coinsView();
		skinSelect();
		settingsView();
		roomSelect();
		furniturePurchase();

		lastWinSize = { ImGui::GetIO().DisplaySize.x, PADDING.y };

		furnitureInventory();

		lastWinSize = PADDING;
	}
};