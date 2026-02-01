#include "pch.h"
#include "TextureManager.h"


void TextureManager::cleanup() {
	textures.clear();
	sprites.clear();
	spritesValues.clear();
	texts.clear();
}


void TextureManager::_render(float dt, JS_SPRITE& s) {
	if (!s.visible) return;

	if (s.playingAnimation) {
		const int absoluteFrame = (int)std::floor(s.animationElapsed / s.animationAdvanceTime);
		s.frame = absoluteFrame % s.numFrames;

		if (absoluteFrame != 0 && absoluteFrame % s.numFrames == 0) {
			++s.animationElapsedLoops;
			//std::cout << s.animationElapsedLoops << std::endl;
		}

		if (s.animationLoopCount >= 0 && s.animationElapsedLoops > s.animationLoopCount) {
			s.playingAnimation = false;
			s.frame = s.staticFrameIdx;
		}

		s.animationElapsed += dt;

	}
	s.sprite.setTextureRect(sf::IntRect(
		{ s.left + s.frame * (s.width + s.xoffset), s.top },
		{ s.width, s.height }
	));

	win.draw(s.sprite);
}


void TextureManager::render(float dt) {
	for (JS_SPRITE& s : spritesValues) {
		_render(dt, s);
	}

	for (const auto& [ref, txt] : texts) {
		win.draw(txt);
	}
}


bool TextureManager::registerTexture(const std::string& ref, const std::string& path) {
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
bool TextureManager::createSprite(
	const std::string& ref,
	const std::string& texRef,
	int num_frames,
	int xoffset,
	int left,
	int top,
	int width,
	int height,
	bool playingAnimation,
	float animationAdvanceTime,
	bool visible) {
	if (textures.find(texRef) == textures.end()) {
		std::cerr << "TextureManager::createSprite > texRef " << texRef << " does not exist" << std::endl;
		return false;
	}

	JS_SPRITE s(sf::Sprite(textures.at(texRef)), num_frames, xoffset, left, top, width, height, playingAnimation, animationAdvanceTime, -1, visible);
	s.ref = ref;

	sprites.emplace(ref, s);
	spritesValues.push_back(sprites.at(ref));

	sortSprites();

	return true;
}

void TextureManager::sortSprites() {
	std::sort(spritesValues.begin(), spritesValues.end(), [](const auto& lhs, const auto& rhs) {return lhs.get().z < rhs.get().z; });
}


sf::Texture& TextureManager::getTexture(const std::string& ref) {
	return textures.at(ref);
}

const sf::Texture& TextureManager::getTexture(const std::string& ref) const {
	return textures.at(ref);
}

TextureManager::JS_SPRITE& TextureManager::getSprite(const std::string& ref) {
	if (sprites.find(ref) == sprites.end()) throw std::runtime_error("Invalid sprite ref");
	return sprites.at(ref);
}

const TextureManager::JS_SPRITE& TextureManager::getSprite(const std::string& ref) const {
	return sprites.at(ref);
}

const std::vector<std::reference_wrapper<TextureManager::JS_SPRITE>>& TextureManager::getAllSprites() const {
	return spritesValues;
}

sf::Text& TextureManager::registerText(const std::string& ref, const std::string& content, int fontSize) {
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

void TextureManager::setTextContent(const std::string& ref, const std::string& content) {
	if (texts.find(ref) == texts.end()) {
		std::cerr << "TextureManager > drawText > ref " << ref << " does not exist" << std::endl;
	}

	sf::Text& text = texts.at(ref);
	text.setString(content);
	sf::FloatRect textBounds = text.getLocalBounds();
	text.setOrigin(textBounds.position + textBounds.size / 2.f);
	text.setOutlineColor({ 0,0,0, 255 });
	text.setFillColor({ 255,255,255,255 });
	text.setOutlineThickness(1.f);
}

void TextureManager::deleteSprite(const std::string& ref) {
	auto it = std::find_if(spritesValues.begin(), spritesValues.end(), [&ref](std::reference_wrapper<JS_SPRITE> s) {return s.get().ref == ref; });
	spritesValues.erase(it);
	sprites.erase(ref);
}
