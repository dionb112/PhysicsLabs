/// <summary>
/// Allow a circle to move up and down	along y axis under influence of gravity (9.8) only 
/// [an initial  force is applied to start motion]
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
#include <iostream>

bool setAirborne(bool inAir);
int main()
{
	const float cooeficiantOfRestitution = 0.777f;
	const float pixelsToMeters = 20.0f;
	sf::Time timeInAir = sf::Time::Zero;
	bool velocityMinus = false;
	bool isAirborne = false;
	bool firstBase = false;
	float maxHeight = 0;
	float hRange = 0;
	sf::Vector2f userValues = { 90, -100 };

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
	text.setFillColor(sf::Color::Red);
	//tested what result I should get online using provided formula and got: -9.03489795918.
	//The result I get using this system is VERY close to that so I am happy. (likely just a rounding issue)
	//The same goes for expected distance (slightly off but SO close)
	text.setString("Max height = " + std::to_string(maxHeight) + " meters from ground" +
		"\nTime taken = " + std::to_string(timeInAir.asSeconds()) + " seconds in air!" +
		"\nHorizontal Range = " + std::to_string(hRange) + "m");

	sf::RectangleShape plane;
	plane.setPosition(sf::Vector2f{ 0, 700 });
	plane.setSize(sf::Vector2f{ 800, 40 });

	sf::RenderWindow window(sf::VideoMode(800, 800), "GO PHYSICS");

	sf::CircleShape shape(10.0f);
	shape.setFillColor(sf::Color::Green);
	shape.setOrigin(10, 10);

	sf::Vector2f velocity(0, 0);
	sf::Vector2f position(20, 700 - pixelsToMeters / 2);
	sf::Vector2f gravity(0.0f, 0.0f * pixelsToMeters);

	sf::Clock airtimeClock; //start the clock (by instantiating it)
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


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				if (!isAirborne)
				{
					isAirborne = true; 
					velocity += userValues;
					gravity = { 0.0f, 9.8f * pixelsToMeters };
					airtimeClock.restart(); // restart the clock here since it has been counting since it was instatiated earlier
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				velocity = sf::Vector2f(0, 0);
				position = sf::Vector2f(20, 700 - pixelsToMeters / 2);
				gravity = sf::Vector2f(0.0f, 0.0f * pixelsToMeters);
				isAirborne = false;
				maxHeight = 0;
				hRange = 0;
				timeInAir = sf::Time::Zero;
				userValues = { 90, -100 };
				firstBase = false;
				//setting the string again every update with updated numbers for realtime output
				text.setString("Max height = " + std::to_string(maxHeight) + " meters from ground" +
					"\nTime taken = " + std::to_string(timeInAir.asSeconds()) + " seconds in air!" +
					"\nHorizontal Range = " + std::to_string(hRange) + "m");

			}
			//increase angle of projection
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
			{
				userValues.x -= 5;
				userValues.y -= 5;
				std::cout << "angle of projection increased" << std::endl;
			}
			//decrease angle of proj
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::U))
			{
				userValues.x += 5;
				userValues.y += 5;
				std::cout << "angle of projection decreased" << std::endl;
			}
			//increase vel
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
			{
				userValues.x += 5;
				userValues.y -= 5;
				std::cout << "initial velocity increased" << std::endl;
			}
			//decrease vel
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
			{
				userValues.x -= 5;
				userValues.y += 5;
				std::cout << "initial velocity decreased" << std::endl;
			}
		}
		//get the time since last update and restart the clock
		timeSinceLastUpdate += clock.restart();
		//update every 60th of a second
		if (timeSinceLastUpdate > timePerFrame)
		{
			window.clear();

			//update position and velocity here using equations in lab sheet using :
			//timeChange as timeSinceLastUpdate.asSecond()
			float timeChange = timeSinceLastUpdate.asSeconds();
			position = position + velocity * timeChange + 0.5f * gravity * pow(timeChange, 2);
			velocity = velocity + gravity * timeChange;
			//update shape on screen 
			shape.setPosition(position);
			//collision
			if (position.y > plane.getPosition().y - pixelsToMeters / 2 - 1)
			{
				position.y = plane.getPosition().y - pixelsToMeters / 2;
				velocity.y = cooeficiantOfRestitution*velocity.y * -1;

				if (isAirborne && !firstBase)
				{
					hRange = position.x;
					firstBase = true;
					//setting the string again every update with updated numbers for realtime output
					text.setString("Max height = " + std::to_string(maxHeight) + " meters from ground" +
						"\nTime taken = " + std::to_string(timeInAir.asSeconds()) + " seconds in air!" +
						"\nHorizontal Range = " + std::to_string(hRange) + "m");
				}
			}
			if (!firstBase)
			{
				timeInAir += airtimeClock.restart(); // add time elapsed since last checked this and restart the clock.

				if (690 - position.y > maxHeight)
				{
					maxHeight = (690 - position.y); // current pos - start point
				}
			}

			window.draw(shape);
			window.draw(plane);
			window.draw(text);

			window.display();

			timeSinceLastUpdate = sf::Time::Zero;
		}
	}
	return 0;
}
/// <summary>
/// function to flip the bool status of in air for the circle
/// </summary>
/// <param name="inAir"></param>
/// <returns></returns>
bool setAirborne(bool inAir)
{
	if (inAir)
	{
		inAir = false;
	}
	else
	{
		inAir = true;
	}
	return inAir;
}
