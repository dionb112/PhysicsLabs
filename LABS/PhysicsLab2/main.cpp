/// <summary>
/// Allow a circle to move up and down	along y axis under influence of gravity (9.8) only 
/// [an initial  force is applied to start motion]
/// Extending to project a missile and hit a target, under coefficient of air resist
/// Extending to create animations for:
///		1. Jumping	2. Moving and Jumping	3. Walking 
/// 
/// Notes:
///		mass (1) * acceleration = mass (1) * gravity - air resistamnce
///		gravity here is length of velocity * velocity
/// 
/// @author Dion Buckley
/// @date October 2017 - January 2018
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
#include <iostream>
#include <ctime>
#include <cstdlib>


int main()
{
	const int MAX_VEL = 200;
	const double PI = 3.14159265359;
	const float PIXELS_TO_METERS = 20.0f;

	srand(time(NULL));

	sf::Vector2f acceleration{ 0,0 };
	float coefficientOfFriction = 0.8f;
	sf::Vector2f unitVelocity{ 0,0 };
	double initialVelocity = 100;
	bool canJump = true;

	sf::Text text;
	sf::Font font;
	if (!font.loadFromFile("arial.ttf"))
	{
		std::cout << " Error with font loading";
	}
	text.setFont(font);
	text.setPosition(sf::Vector2f{ 150,10 });
	text.setCharacterSize(20);
	text.Italic;
	text.setFillColor(sf::Color::Magenta);
	
	sf::RectangleShape plane;
	plane.setPosition(sf::Vector2f{ 0, 700 });
	plane.setSize(sf::Vector2f{ 800, 40 });

	sf::RenderWindow window(sf::VideoMode(800, 800), "GOoOoO PHYSICS");

	sf::RectangleShape shape;
	shape.setFillColor(sf::Color::Green);
	shape.setOrigin(20, 20);
	shape.setSize(sf::Vector2f{ 40, 40 });

	sf::Vector2f velocity(0, 0);
	sf::Vector2f position(420, 690 - PIXELS_TO_METERS / 2);
	sf::Vector2f gravity(0.0f, 9.8f * PIXELS_TO_METERS);

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
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				window.close();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				if (canJump)
				{
					canJump = false;

					velocity.y = -initialVelocity * 2;
				}
			}

			if (event.type == event.KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				{
					velocity.x = -initialVelocity;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				{
						velocity.x = initialVelocity;
				}

				
			}
		}
		//get the time since last update and restart the clock
		timeSinceLastUpdate += clock.restart();
		//update every 60th of a second
		if (timeSinceLastUpdate > timePerFrame)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				velocity.x = -initialVelocity;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				velocity.x = initialVelocity;
			}

			window.clear();

			// timeChange as timeSinceLastUpdate.asSecond()
			float timeChange = timeSinceLastUpdate.asSeconds();

			// update acceleration based on friction and length of velocity vector
			float length = sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));
			if (length != 0)
			{
				unitVelocity.x = velocity.x / length;
			}
			
			// no friction in the aaiirr
			if (canJump)
			{
				acceleration.x = -coefficientOfFriction * gravity.y * unitVelocity.x;
			}
			else
			{
				acceleration = gravity;
			}

			if (velocity.x < 0.5f  && velocity.x > -0.5f)
			{
				velocity.x = 0;
			}

			// update position and velocity here using equations in lab sheet
			position = position + velocity * timeChange + 0.5f * acceleration * pow(timeChange, 2);
			velocity = velocity + acceleration * timeChange;

			// update shape on screen 
			shape.setPosition(position);

			// collision with plane
 			if (position.y > 690 - PIXELS_TO_METERS / 2)
			{
				position.y = 690 - PIXELS_TO_METERS / 2;
				acceleration.y = 0;
				canJump = true;

			}

			window.draw(shape);
			window.draw(plane);

			window.display();

			timeSinceLastUpdate = sf::Time::Zero;
		}
	}
	return 0;
}