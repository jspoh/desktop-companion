#pragma once


#include "pch.h"
#include "TextureManager.h"
#include "entities/Cat.h"
#include "Room/Room.h"


class UI {
private:


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

		ImGui::End();
	}

	static void skinSelect() {
		auto onChange = [](const std::string& ref) {
			tm.getSprite(Cat::get().getCatSpriteName()).sprite.setTexture(tm.getTexture(ref));
			Cat::get().init(false);
			};

		renderImageButtons({ 5, 50 }, "Companion skin", "Select skin", Cat::get().catSpriteRefs, 999, (float)Cat::get().getWidth(), (float)Cat::get().getHeight(), onChange);
	}

	static void roomSelect() {
		auto onChange = [](const std::string& ref) {
			Room::get().sprite->sprite.setTexture(tm.getTexture(ref));
			};

		renderImageButtons({ 5, 350 }, "Room", "Select color", Room::get().getRoomRefs(), 3, 50.f, 50.f, onChange);
	}

	static void settingsView() {
		ImGui::SetNextWindowPos({ 5,200 }, ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always); // Auto-size

		ImGui::Begin("Settings", nullptr,
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoMove);

		ImGui::Checkbox("Summon cat to mouse on left click", &Settings::catFollowsMouseClick);
		ImGui::Checkbox("Cat talks", &Settings::catTalks);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::DragFloat("Cat scale", &Settings::catScale, 0.01f, 0.1f, 10.f, "%.1f");
		ImGui::DragFloat("Room scale", &Settings::roomScale, 0.01f, 1.f, 2.f, "%.1f");

		ImGui::End();
	}

	static void coinsView() {
		ImGui::SetNextWindowPos({ 5,5 }, ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always); // Auto-size

		ImGui::Begin("Coins", nullptr,
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoMove);

		std::stringstream ss;
		ss << "Coins: " << Settings::coins;
		ImGui::Text(ss.str().c_str());

		ImGui::End();
	}

	static void render() {
		coinsView();
		roomSelect();
		skinSelect();
		settingsView();
	}
};