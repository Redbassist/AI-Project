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
#include <iostream>

using namespace std;

#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h> 
#include "Player.h" 

int main()
{
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

	//sf::RenderWindow window(sf::VideoMode(800, 600), "AI Lab 1");

	sf::RenderWindow window(sf::VideoMode(800, 600), "AI Lab 1", sf::Style::Default, settings);
	 
	Player player = Player(); 

	//update loop
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		//draw stuff here 
		player.Draw(window); 
		window.display(); 

		player.Update(window); 
	}

	return 0;
}