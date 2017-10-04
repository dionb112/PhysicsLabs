/// <summary>
/// Allow a circle to move up and down	along y axis under influence of gravity (9.8) only [an initial  force is applied to start motion]
/// 
/// 
/// @author Dion Buckley
/// @date October 2017
/// </summary>

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

#include <SFML/Graphics.hpp>
#include <math.h>


/// <summary>
/// main entry point
/// </summary>
/// <returns>zero</returns>
int main()
{
	sf::RectangleShape plane;
	plane.setPosition(0, 700);
	plane.setSize(sf::Vector2f{ 800, 40 });
	sf::RenderWindow window(sf::VideoMode(800, 800), "GO PHYSICS");
	sf::CircleShape shape(0.5f);

	shape.setFillColor(sf::Color::Green);
	sf::Vector2f velocity(0, 0);
	sf::Vector2f position(400, 400);
	sf::Vector2f gravity(0.0f, 9.8f);
	sf::Clock clock;
	const float FPS = 60.0f;
	const sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	clock.restart();
	while (window.isOpen())
	{
		//read keyboard input
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				{
					velocity = sf::Vector2f(0.0f, -100.0f);
				}
			}
			//get the time since last update and restart the clock
			timeSinceLastUpdate += clock.restart();
			//update every 60th of a second
			if (timeSinceLastUpdate > timePerFrame)
			{
				//update position and velocity here using equations in lab sheet using timeChange as timeSinceLastUpdate.asSecond()
				//update shape on screen 
				float timeChange = timeSinceLastUpdate.asSeconds();
				position = position + velocity * timeChange + 0.5f * gravity * pow(timeChange, 2);
				velocity = velocity + gravity * timeChange;
				shape.setPosition(position);
				timeSinceLastUpdate = sf::Time::Zero;
			}
		}
		window.clear();
		shape.setPosition(position);
		window.draw(shape);
		window.draw(plane);
		window.display();
	}

	return 0;
}