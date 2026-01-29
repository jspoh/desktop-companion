#pragma once

// define in project properties
//#define SFML_STATIC

#ifdef _WIN32
	#include <windows.h>
#endif

#include <SFML/Graphics.hpp>


class Window {
private:
	sf::RenderWindow window;

	Window() {};
	~Window() {};

public:
	static Window& get() {
		static Window instance;
		return instance;
	}

	void init() {
		// if fullscreen, can never be a proper overlay
		auto videoMode = sf::VideoMode::getDesktopMode();
		--videoMode.size.y;

		// Create the main window
		window = sf::RenderWindow(videoMode, "Desktop Companion", sf::Style::None);

#ifdef _WIN32
		bool success = window.setActive(true);
		if (!success) throw std::runtime_error("Setting window to active failed in Window.h > Window::init");
		HWND hwnd = window.getNativeHandle();

		// SWP_NOMOVE ignores x,y params. no move
		// SWP_NOSIZE ignores w,h params, no size
		// SWP_NOACTIVATE never steals focus
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

		// GWL_EXSTYLE - extended window styles
		// WS_EX_TRANSPARENT - window transparent to mouse events (click through)
		// WS_EX_LAYERED - for transparency effects and setLayeredWindowAttributes
		// WS_EX_TOOLWINDOW - removes from taskbar and Alt+Tab
		SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW);

		// makes all color with 2nd param alpha to 0
		SetLayeredWindowAttributes(hwnd, RGB(255, 0, 255), 0, LWA_COLORKEY);
#endif
	}

	sf::RenderWindow& getWindow() {
		return window;
	}

	void clear(const sf::Color& col) {
		window.clear(col);
	}
};
