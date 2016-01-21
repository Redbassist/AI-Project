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
#include "Factory.h"

using namespace std;

#include <SFML/Graphics.hpp>
#include "Player.h" 
#include "Boid.h"
#include "BoidManager.h"
#include "Predator.h"
#include "Swarmer.h"
#include "AsteroidManager.h"
#include "CollisionManager.h"
#include "FactoryManager.h"
#include "MissileManager.h"
#include "PowerUpManager.h"

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
	View view = View(Vector2f(0, 0), Vector2f(1280, 720));
	view.zoom(1);
	window->setView(view);
	window->setFramerateLimit(60);
	 
	Player* player = new Player(); 

	int numFactories = 4;
	for (int i = 0; i < numFactories; i++)
	{
		FactoryManager::GetInstance()->AddFactory(new Factory(*player, i));
	}

	CollisionManager::GetInstance()->setPlayer(*player);
	sf::Texture background;

	if (!background.loadFromFile("Sprites/background.jpg"))
	{
		// error...
	}

	background.setSmooth(true);

	sf::Sprite backgroundSprite;

	backgroundSprite.setTexture(background);

	backgroundSprite.setScale(sf::Vector2f(1.5f, 1.5f));

	backgroundSprite.setPosition(sf::Vector2f(1500, 0));

	backgroundSprite.setOrigin(sf::Vector2f(1715, 1733));

	//for (int i = 0; i < 50; i++)
	//{
	//	BoidManager::GetInstance()->AddBoid(new Swarmer(200, 200, CollisionManager::GetInstance()->player));
	//}

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
		BoidManager::GetInstance()->Update();
		BulletManager::GetInstance()->Update();
		MissileManager::GetInstance()->Update();
		FactoryManager::GetInstance()->Update();
		CollisionManager::GetInstance()->CheckCollisions();
		PowerUpManager::GetInstance()->Update();

		window->clear();
		//draw stuff here
		window->draw(backgroundSprite); 
		player->Draw();
		AsteroidManager::GetInstance()->Draw();
		PowerUpManager::GetInstance()->Draw();
		BoidManager::GetInstance()->Draw();
		FactoryManager::GetInstance()->Draw();
		BulletManager::GetInstance()->Draw();
		MissileManager::GetInstance()->Draw();
		window->display();

	}

	return 0;
}