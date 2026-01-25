#define SFML_STATIC

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#ifdef _WIN32
	#include <windows.h>
#endif

int main()
{
	// Create the main window
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Desktop Companion", sf::Style::None);

#ifdef _WIN32
	// Windows: Set window to be always on top and click-through
	HWND hwnd = window.getNativeHandle();
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	// Make window click-through (so you can interact with windows behind it)
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hwnd, RGB(255, 0, 255), 0, LWA_COLORKEY);		// any parts that are pink/magenta will be transparent
#endif

	sf::Color transparentColor(255, 0, 255, 0);

	// Load a sprite to display
	const sf::Texture texture("assets/Idle.png");
	sf::Sprite sprite(texture);

	// Create a graphical text to display
	const sf::Font font("assets/arial.ttf");
	sf::Text text(font, "Hello SFML", 50);

	// Load a music to play
	sf::Music music("assets/Cutie-Patootie.mp3");

	// Play the music
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