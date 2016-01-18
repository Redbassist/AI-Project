#ifdef _DEBUG
#pragma comment(lib,"sfml-graphics-d.lib")
#pragma comment(lib,"sfml-audio-d.lib")
#pragma comment(lib,"sfml-system-d.lib")
#pragma comment(lib,"sfml-window-d.lib")
#pragma comment(lib,"sfml-network-d.lib")
#else
#pragma comment(lib,"sfml-graphics.lib")
#pragma comment(lib,"sfml-audio.lib")
#pragma comment(lib,"sfml-system.lib")
#pragma comment(lib,"sfml-window.lib")
#pragma comment(lib,"sfml-network.lib")
#endif

#include "stdafx.h"
#include "Globals.h"
#include <iostream>

using namespace std;

#include <SFML/Graphics.hpp>
#include "Player.h" 
#include "AsteroidManager.h"

int main()
{
	srand(time(NULL));
	sf::Font MyFont;
	if (!MyFont.loadFromFile("arial.ttf"))
	{
		// Error...
	}

	srand(time(NULL)); //setting seed for random

	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 1000;
	settings.majorVersion = 3;
	settings.minorVersion = 0;

	globalBounds = Vector2f(3840, 2160);
	 
	window = new RenderWindow(sf::VideoMode(1280, 720), "AI Lab 1", sf::Style::Default, settings);
	window->setFramerateLimit(60);
	 
	Player* player = new Player();  

	//update loop
	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();
		}

		player->Update();
		AsteroidManager::GetInstance()->Update();

		window->clear();
		//draw stuff here 
		player->Draw();
		AsteroidManager::GetInstance()->Draw();
		window->display();

	}

	return 0;
}