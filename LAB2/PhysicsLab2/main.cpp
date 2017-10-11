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
	sf::Time timeInAir = sf::Time::Zero;
	bool velocityMinus = false;
	bool isAirborne = false;
	bool reachedMax = false;
	bool setup = true;
	float maxHeight=0;

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
	text.setString("Expected max height = " + std::to_string((pow(44.271f, 2) / (2 * 9.8))) + " meters\n" +
		"Actual max height = " + std::to_string(maxHeight) + " meters from ground\n\n" +
		"Expected time taken = " + std::to_string(44.271f * 2 / 9.8) + " seconds" +
		"\nActual time taken = " + std::to_string(timeInAir.asSeconds()) + " seconds in air!");

	sf::RectangleShape plane;
	plane.setPosition(sf::Vector2f{ 0, 700 });
	plane.setSize(sf::Vector2f{ 800, 40 });

	sf::RenderWindow window(sf::VideoMode(800, 800), "GO PHYSICS");

	sf::CircleShape shape(0.5f);
	shape.setFillColor(sf::Color::Green);

	sf::Vector2f velocity(0, 0);
	sf::Vector2f position(400, 698);
	sf::Vector2f gravity(0.0f, 0.0f);

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

			if (event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				{
					if (!isAirborne)
					{
						isAirborne = true; //swaps bool for in air
						//after playing around with values for about 5 - 10 mins I found this number 
						//which gives me the perfect: hits 100 m above start point for long enough
						//to output only one message to console
						velocity += sf::Vector2f(0.0f, -44.271f);
						gravity = { 0.0f, 9.8f };
						airtimeClock.restart(); // restart the clock here since it has been counting since it was instatiated earlier
						setup = false;
					}
				}
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
			if (position.y >= plane.getPosition().y -1)
			{
				gravity.y = 0;
				isAirborne = false;
				position.y = plane.getPosition().y - 1;
				if (!setup)
				{
					velocity.y -= 10;
					velocity.y = velocity.y * -1;
				}
			}
			else
			{
				gravity.y = 9.8f;
				isAirborne = true;
			}
			if (isAirborne && !setup)
			{
				timeInAir += airtimeClock.restart(); // add time elapsed since last checked this and restart the clock.
				if (!reachedMax)
				{
					maxHeight = (position.y - 699) * -1;
				}
				//setting the string again every update with updated numbers for realtime output
				text.setString("Expected max height = " + std::to_string((pow(44.271f, 2) / (2 * 9.8))) + " meters\n" +
					"Actual max height = " + std::to_string(maxHeight) + " meters from ground\n\n" +
					"Expected time taken = " + std::to_string(44.271f * 2 / 9.8) + " seconds" +
					"\nActual time taken = " + std::to_string(timeInAir.asSeconds()) + " seconds in air!");
			}

			if (position.y <= 599) // 100 m above start point
			{
				maxHeight = (position.y - 699) * -1; // current pos - start point
				reachedMax = true;
				std::cout << "WE high boys" << std::endl;
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
