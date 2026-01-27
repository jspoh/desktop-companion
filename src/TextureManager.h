#pragma once

#include "defines.h"
#include "Window.h"


class TextureManager {
public:
	struct JS_SPRITE {
		JS_SPRITE(const sf::Sprite& s) : sprite(s) {}
		JS_SPRITE(const sf::Sprite& s, int numFrames, int left, int top, int width, int height) : numFrames{ numFrames }, sprite(s), left(left), top(top), width(width), height(height) {
			sprite.setTextureRect(sf::IntRect(
				sf::Vector2i(left + frame * width, top),  // position
				sf::Vector2i(width, height)                // size
			));
		}

		sf::Sprite sprite;

		int frame{};
		int numFrames{};
		float animationElapsed{};

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

	void render() {
		for (JS_SPRITE& js_sprite : spritesValues) {
			sf::Sprite& s = js_sprite.sprite;
			//s.getTextureRect(sf::IntRect(
			//	js_sprite.left + js_sprite.frame * js_sprite.width,
			//	js_sprite.top,
			//	js_sprite.width,
			//	js_sprite.height,
			//));

			win.draw(s);
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
	bool createSprite(const std::string& ref, const std::string& texRef, int num_frames=0, int left = 0, int top = 0, int width = 0, int height = 0) {
		if (textures.find(texRef) == textures.end()) {
			std::cerr << "TextureManager::createSprite > texRef " << texRef << " does not exist" << std::endl;
			return false;
		}

		JS_SPRITE s(sf::Sprite(textures.at(texRef)), num_frames, left, top, width, height);

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
