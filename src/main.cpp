#include "pch.h"
#include "GameManager.h"
#include "Window.h"


#ifdef _DEBUG
int main()
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
	Window::get().init();
	sf::RenderWindow& window = Window::get().getWindow();


	// get from Window.h > Window::init > setLayeredWindowAttributes
	GameManager::get().init(sf::Color(255, 0, 255));

	GameManager::get().update();

	GameManager::get().cleanup();

	return 0;
}