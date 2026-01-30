#pragma once


#include "pch.h"
#include "TextureManager.h"
#include "entities/Cat.h"


class UI {
private:


public:
	static void init() {
		
	}

	template<typename fn>
	static void renderImageButtons(const ImVec2& pos, const std::string& title, const std::string& instruction, const std::vector<std::string>& refs, float w, float h, fn callback) {
		ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always); // Auto-size

		ImGui::Begin(title.c_str(), nullptr,
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoMove);

		ImGui::Text(instruction.c_str());
		ImGui::Separator();

		ImGui::BeginGroup();
		for (const std::string& ref : refs) {
			ImGui::SameLine();

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

		renderImageButtons({ 10, 10 }, "Companion skin", "Select skin", Cat::get().catSpriteRefs, (float)Cat::get().getWidth(), (float)Cat::get().getHeight(), onChange);
	}

	static void render() {
		skinSelect();
	}
};