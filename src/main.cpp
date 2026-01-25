#define SFML_STATIC

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#ifdef _WIN32
	#include <windows.h>
#endif

#include "GameManager.h"

int main()
{
	// if fullscreen, can never be a proper overlay
	auto videoMode = sf::VideoMode::getDesktopMode();
	--videoMode.size.y;

	// Create the main window
	sf::RenderWindow window(videoMode, "Desktop Companion", sf::Style::None);

#ifdef _WIN32
	window.setActive(true);
	HWND hwnd = window.getNativeHandle();

	// SWP_NOMOVE ignores x,y params. no move
	// SWP_NOSIZE ignores w,h params, no size
	// SWP_NOACTIVATE never steals focus
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	// GWL_EXSTYLE - extended window styles
	// WS_EX_TRANSPARENT - window transparent to mouse events (click through)
	// WS_EX_LAYERED - for transparency effects and setLayeredWindowAttributes
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT | WS_EX_LAYERED);	

	// makes all color with 2nd param alpha to 0
	SetLayeredWindowAttributes(hwnd, RGB(255,0,255), 0, LWA_COLORKEY);
#endif

	GameManager::get().init(sf::Color(255, 0, 255));

	// Load a sprite to display
	const sf::Texture texture("assets/Idle.png");
	sf::Sprite sprite(texture);

	// Create a graphical text to display
	const sf::Font font("assets/arial.ttf");
	//sf::Text text(font, "Hello SFML", 50);

	sf::Music music("assets/Cutie-Patootie.mp3");
	music.play();

	// Start the game loop
	while (window.isOpen())
	{
		// Process events
		while (const std::optional event = window.pollEvent())
		{
			// Close window: exit
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		// Clear screen
		window.clear(transparentColor);

		// Draw the sprite
		window.draw(sprite);

		// Draw the string
		window.draw(text);

		// Update the window
		window.display();
	}
}