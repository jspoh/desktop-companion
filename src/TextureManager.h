#pragma once

#include "defines.h"
#include "Window.h"


class TextureManager {
public:
	struct JS_SPRITE {
		JS_SPRITE(const sf::Sprite& s) : sprite(s) {}
		JS_SPRITE(const sf::Sprite& s, int numFrames, int offset, int left, int top, int width, int height, bool playingAnimation, float animationAdvanceTime = 0.1f) : numFrames{ numFrames }, offset{ offset }, sprite(s), left(left), top(top), width(width), height(height), playingAnimation{ playingAnimation }, animationAdvanceTime{ animationAdvanceTime } {
			sprite.setTextureRect(sf::IntRect(
				sf::Vector2i(left + frame * (width + offset), top),  // position
				sf::Vector2i(width, height)                // size
			));
		}

		sf::Sprite sprite;

		int frame{};
		int numFrames{};
		float animationElapsed{};
		bool playingAnimation{ false };
		float animationAdvanceTime{};
		int offset{};

		int left{};
		int top{};
		int width{};
		int height{};
	};

private:
	sf::Font font;

	TextureManager() {
		font = sf::Font("assets/arial.ttf");
	}

	~TextureManager() = default;

	std::unordered_map<std::string, sf::Texture> textures;		// loaded into gpu vram
	std::unordered_map<std::string, JS_SPRITE> sprites;
	std::vector<std::reference_wrapper<JS_SPRITE>> spritesValues;						// for fast access of sprites.values()
	std::unordered_map<std::string, sf::Text> texts;

public:
	static TextureManager& get() {
		static TextureManager instance;
		return instance;
	}

	void cleanup() {
		textures.clear();
		sprites.clear();
		spritesValues.clear();
		texts.clear();
	}

	void render(float dt) {


		for (JS_SPRITE& s : spritesValues) {
			if (s.playingAnimation) {
				const int absoluteFrame = (int)std::floor(s.animationElapsed / s.animationAdvanceTime);
				s.frame = absoluteFrame % s.numFrames;

				s.animationElapsed += dt;

				s.sprite.setTextureRect(sf::IntRect(
					{ s.left + s.frame * (s.width + s.offset), s.top },
					{ s.width, s.height }
				));
			}

			win.draw(s.sprite);
		}

		for (const auto& [ref, txt] : texts) {
			win.draw(txt);
		}
	}

	bool registerTexture(const std::string& ref, const std::string& path) {
		try {
			textures.emplace(ref, sf::Texture(path));
		}
		catch (const std::exception& e) {
			std::cerr << "Error loading texture '" << ref << "' from path '"
				<< path << "': " << e.what() << std::endl;
			return false;
		}
		catch (...) {
			return false;
		}
		return true;
	}

	// @param left, top, width, height for sf::IntRect() to set texture rect forr cutting spritesheets
	bool createSprite(const std::string& ref, const std::string& texRef, int num_frames = 0, int offset=0, int left = 0, int top = 0, int width = 0, int height = 0, bool playingAnimation=false, float animationAdvanceTime=0.1f) {
		if (textures.find(texRef) == textures.end()) {
			std::cerr << "TextureManager::createSprite > texRef " << texRef << " does not exist" << std::endl;
			return false;
		}

		JS_SPRITE s(sf::Sprite(textures.at(texRef)), num_frames, offset, left, top, width, height, playingAnimation, animationAdvanceTime);

		sprites.emplace(ref, s);
		spritesValues.push_back(sprites.at(ref));
		return true;
	}

	sf::Texture& getTexture(const std::string& ref) {
		return textures.at(ref);
	}

	const sf::Texture& getTexture(const std::string& ref) const {
		return textures.at(ref);
	}

	JS_SPRITE& getSprite(const std::string& ref) {
		return sprites.at(ref);
	}

	const JS_SPRITE& getSprite(const std::string& ref) const {
		return sprites.at(ref);
	}

	const std::vector<std::reference_wrapper<JS_SPRITE>>& getAllSprites() const {
		return spritesValues;
	}

	sf::Text& registerText(const std::string& ref, const std::string& content, int fontSize) {
		if (texts.find(ref) != texts.end()) {
			std::cerr << "TextureManager > drawText > ref " << ref << " is already in use" << std::endl;
		}

		// Create a Text object with the font first, then emplace it
		auto [it, inserted] = texts.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(ref),
			std::forward_as_tuple(font)  // Construct sf::Text with font reference
		);

		// Now set the string and character size
		it->second.setString(content);
		it->second.setCharacterSize(fontSize);

		return it->second;
	}
};
