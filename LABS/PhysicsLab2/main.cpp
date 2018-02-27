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
	const float PIXELS_TO_METERS = 20.0f;
	const int MAX_VEL = 200;
	const double PI = 3.14159265359;

	// Player stuff
	sf::Texture playerTexture;
	sf::Sprite playerSprite;
	float rotation = 0;
	float turnRate = 4.0f;
	sf::Vector2f position{ 333, 70 };
	sf::Vector2f velocity{ 0.0f,0.0f };
	sf::Vector2f accelerationVector;
	float acceleration = 0.42f;
	int fuel = 333;
	int lives = 3;

	// Bullet stuff
	sf::CircleShape bullet;
	sf::Vector2f bulletPosition;
	int radius = 2;
	sf::Vector2f bulletVelocity;
	double speed = 10;
	bool bulletActive = false;
	int timer = 0;
	bullet.setFillColor(sf::Color::Red);
	bullet.setRadius(2);

	sf::Vector2f gravity(0.0f, 9.8f);

	bool gameOver = false;

	srand(time(NULL));

	sf::Text text;
	sf::Font font;
	if (!font.loadFromFile("arial.ttf"))
	{
		std::cout << " Error with font loading";
	}
	if (!playerTexture.loadFromFile("ship.png"))
	{
		std::cout << "problem loading player texture file";
	}

	playerSprite.setTexture(playerTexture);
	playerSprite.setOrigin(32, 32);
	playerSprite.setPosition(position);

	sf::RectangleShape plane;
	plane.setPosition(sf::Vector2f{ 0, 700 });
	plane.setSize(sf::Vector2f{ 800, 40 });

	text.setFont(font);
	text.setPosition(sf::Vector2f{ 150,10 });
	text.setCharacterSize(20);
	text.Italic;
	text.setFillColor(sf::Color::Magenta);
	text.setString("Player Lives: " + std::to_string(lives));

	sf::RenderWindow window(sf::VideoMode(800, 800), "GOoOoO PHYSICS");

	sf::Clock clock;
	const float FPS = 60.0f;
	const sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	clock.restart();

	while (window.isOpen() )
	{
		//read keyboard input
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				window.close();
			}

			
	
		}
		//get the time since last update and restart the clock
		timeSinceLastUpdate += clock.restart();
		//update every 60th of a second
		if (timeSinceLastUpdate > timePerFrame && !gameOver)
		{

			// timeChange as timeSinceLastUpdate.asSecond()
			float timeChange = timeSinceLastUpdate.asSeconds();

			position += velocity;

			velocity.x = velocity.x * 0.99; //slow down tommy
			velocity.y = velocity.y * 0.99; //slow down tommy

			// update position and velocity here using equations in lab sheet
			position = position + velocity * timeChange + 0.5f * gravity * pow(timeChange, 2);
			velocity = velocity + gravity * timeChange;

			// bullet update
			if (bulletActive)
			{
				bulletPosition += bulletVelocity;
				
				timer--;
				if (timer <= 0)
				{
					bulletActive = false;
				}
			}
			if (position.x < 0)
			{
				position = sf::Vector2f{ 800, position.y };
			}
			if (position.x > 800)
			{
				position = sf::Vector2f{ 0, position.y };
			}
			
			// collision with plane
			if (position.y > 690 - PIXELS_TO_METERS / 2)
			{
				
				lives--;
				text.setString("Player Lives: " + std::to_string(lives));
				rotation = 0;
				turnRate = 4.0f;
				position ={ 333, 70 };
				velocity = { 0.0f,0.0f };
				acceleration = 0.42f;
				fuel = 333;

				if (lives == 0)
				{
					text.setString("Game Over !");
					gameOver = true;

				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				if (fuel > 0)
				{
					float x = std::sin(rotation * PI / 180.0);
					float y = -std::cos(rotation * PI / 180.0);

					accelerationVector = sf::Vector2f{ x, y };

					velocity.x += accelerationVector.x * acceleration;
					velocity.y += accelerationVector.y * acceleration;

					fuel--;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				rotation -= turnRate;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				rotation += turnRate;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				if (!bulletActive)
				{
					bulletPosition = position;

					float x = std::sin(rotation * PI / 180.0);
					float y = -std::cos(rotation * PI / 180.0);

					bulletVelocity = sf::Vector2f{ x, y };

					bulletVelocity.x += bulletVelocity.x * speed;
					bulletVelocity.y += bulletVelocity.y * speed;

					timer = 60;
					bulletActive = true;
				}
			}


			window.clear();

			playerSprite.setRotation(rotation);
			playerSprite.setPosition(position);
			window.draw(playerSprite);
			bullet.setPosition(bulletPosition);
			window.draw(bullet);
			window.draw(plane);
			window.draw(text);
			window.display();

			timeSinceLastUpdate = sf::Time::Zero;
		}
	}
	return 0;
}