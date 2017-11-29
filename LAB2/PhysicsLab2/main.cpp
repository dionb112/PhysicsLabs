/// <summary>
/// Allow a circle to move up and down	along y axis under influence of gravity (9.8) only 
/// [an initial  force is applied to start motion]
/// Extending to project a missile and hit a target, under coefficient of air resist
/// @author Dion Buckley
/// @date October / November 2017
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
	const double PI = 3.14159265359;
	const float PIXELS_TO_METERS = 20.0f;

	srand(time(NULL));

	int targetAttempts = 0;
	float coefficientOfAirResist = 0.5f;
	double initialVelocity = 100;
	double angleOfProjection = -45; // the 360 degree circle seems to be on minus for this project..
	bool canFire = true;

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

	sf::RenderWindow window(sf::VideoMode(800, 800), "GO PHYSICS");

	sf::CircleShape shape(10.0f);
	shape.setFillColor(sf::Color::Green);
	shape.setOrigin(10, 10);

	sf::CircleShape target(20.0f);
	target.setFillColor(sf::Color::Red);
	target.setOrigin(20, 20);
	target.setPosition( rand() % 780 , 700 - PIXELS_TO_METERS * 2 );

	sf::Vector2f velocity(0, 0);
	sf::Vector2f position(20, 700 - PIXELS_TO_METERS / 2);
	sf::Vector2f gravity(0.0f, 0.0f * PIXELS_TO_METERS);

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
				if (canFire)
				{
					canFire = false;

					velocity.x = initialVelocity * cos(angleOfProjection * PI / 180);
					velocity.y = initialVelocity * sin(angleOfProjection * PI / 180);

					gravity = { 0.0f, 9.8f * PIXELS_TO_METERS };
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
			{
				velocity = sf::Vector2f(0, 0);
				position = sf::Vector2f(20, 700 - PIXELS_TO_METERS / 2);
				gravity = sf::Vector2f(0.0f, 0.0f * PIXELS_TO_METERS);
				canFire = true;
			}
			// not to repeat calls upon key released
			if (event.type == event.KeyPressed)
			{
				//increase angle of projection
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
				{
					angleOfProjection--;
				}
				//decrease angle of proj
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::U))
				{
					angleOfProjection++;
				}
				//increase vel
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
				{
					initialVelocity = initialVelocity + 2;
				}
				//decrease vel
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
				{
					initialVelocity = initialVelocity - 2;
				}
				//increase air resist
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
				{
					if (coefficientOfAirResist < 1)
					{
						coefficientOfAirResist += 0.1;
					}
				}
				//decrease air resist
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
				{
					if (coefficientOfAirResist >= 0.1) // > 0 allowed one extra for some reason . .
					{
						coefficientOfAirResist -= 0.1;
					}
				}
			}
		}
		//get the time since last update and restart the clock
		timeSinceLastUpdate += clock.restart();
		//update every 60th of a second
		if (timeSinceLastUpdate > timePerFrame)
		{
			text.setString(
				"Angle of projection: " + std::to_string(angleOfProjection * -1) +
				"\nInitial Speed: " + std::to_string(initialVelocity) +
				"\nCoefficient of air resist: " + std::to_string(coefficientOfAirResist) +
				"\nAttemps on target: " + std::to_string(targetAttempts));

			window.clear();

			//timeChange as timeSinceLastUpdate.asSecond()
			float timeChange = timeSinceLastUpdate.asSeconds();

			//update position and velocity here using equations in lab sheet
			sf::Vector2f accel = gravity;
			position = position + velocity * timeChange + 0.5f * gravity * pow(timeChange, 2);
			velocity = velocity + gravity * timeChange;

			//TODO: How to use coefficient of air with motion ?! 

			//update shape on screen 
			shape.setPosition(position);
			//collision
			if (position.y > plane.getPosition().y - PIXELS_TO_METERS / 2)
			{
				velocity = sf::Vector2f(0, 0);
				position.y = 700 - PIXELS_TO_METERS / 2;
				gravity = sf::Vector2f(0.0f, 0.0f * PIXELS_TO_METERS);
				targetAttempts++;
			}
			if (position.x - target.getPosition().x < 20 && position.y - target.getPosition().y < 20)
			{
				//TODO: hit target functionality
				std::cout << "hit target";
			}

			window.draw(target);
			window.draw(shape);
			window.draw(plane);
			window.draw(text);

			window.display();

			timeSinceLastUpdate = sf::Time::Zero;
		}
	}
	return 0;
}