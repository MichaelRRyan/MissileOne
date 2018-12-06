// @Author Michael Rainsford Ryan
// C00239510
// @Date 01/12/2018
// Estimated time: 180m
// Session 1: 23:17 - 23:59 - 01/12/2018
// Session 2: 00:00 - 01:48 - 02/12/2018
// Session 3: 12:10 - 13:34 - 02/12/2018
// Session 4: 13:41 - 13:51 - 02/12/2018
// Session 5: 23:51 - 00:15 - 02-03/12/2018
// Session 6: 17:45 - 18:00 - 04/12/2018
// Time to finish project: 223m

// Extra fun stuff
// Estimated time: 120m
// Session 7: 22:20 - 22:40 - 05/12/2018
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Known bugs:
// - None

#include "Game.h"
#include "MyVector2.h"
#include <iostream>


/// <summary>
/// default construcor
/// pass parameters fpr sfml window, setup m_exitGame
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 800, 480, 32 }, "SFML Game" },
	m_exitGame{ false }, //when true game will exit
	m_currentMissileState{ MissileStates::ReadyToFire }
{
	setupFontAndText(); // load font 
	setupSprite(); // load texture
	setupShapes(); // Setup the shapes and lines
}

/// <summary>
/// default destructor for game, nothing to release
/// </summary>
Game::~Game()
{
}


/// <summary>
/// game loop running at 60fps
/// </summary>
void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timePerFrame = sf::seconds(1.f / 60.f); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}

/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (sf::Event::Closed == event.type) // Window message
		{
			m_window.close();
		}
		if (sf::Event::KeyPressed == event.type) // User key press
		{
			if (sf::Keyboard::Escape == event.key.code)
			{
				m_exitGame = true;
			}
			if (sf::Keyboard::T == event.key.code) // Toggle the cool effects and graphics
			{
				m_coolStuffOn = !m_coolStuffOn;
			}
		}
		if (sf::Event::MouseButtonPressed == event.type) // User mouse press
		{
			processMouseEvents(event); // Separated into a new function to keep code tidy
		}
	}
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}

	if (!m_gameOver) // If the game hasn't ended, do the main game updates
	{
		updateAltitudeBar(); // Update the altitude/missile power bar

		if (m_currentMissileState == MissileStates::FiredMissile)
		{
			updateMissile(); // Update the missile if the missile is active
		}
		else if (m_currentMissileState == MissileStates::Explosion)
		{
			updateExplosion(); // Update the explosion is the explosion is active
		}

		if (!m_asteroidInPlay)
		{
			launchAsteroid(); // Launch the asteroid if there are none in play
		}
		else
		{
			updateAsteroid(); // Move the asteroid if the asteroid is active
		}
	}

	if (m_coolStuffOn)
	{
		m_scoreText.setString("Score: " + std::to_string(m_score)); // Update the score text
	}
}

/// <summary>
/// draw the frame and then switch bufers
/// </summary>
void Game::render()
{
	m_window.clear();
	m_window.draw(m_ground);

	// Draw the sprites
	if (m_coolStuffOn)
	{
		m_window.draw(m_skylineSprite);
	}

	m_window.draw(m_base);
	m_window.draw(m_altitudeBar);

	// Draw the asteroid and missile stuff
	if (m_asteroidInPlay)
	{
		m_window.draw(m_asteroid);
	}
	if (m_currentMissileState == MissileStates::FiredMissile)
	{
		m_window.draw(m_missile);
	}
	if (m_currentMissileState == MissileStates::Explosion)
	{
		m_window.draw(m_explosion);
	}

	// Draw the text
	m_window.draw(m_altitudeText);

	if (m_coolStuffOn)
	{
		m_window.draw(m_scoreText);
	}

	if (m_gameOver)
	{
		m_window.draw(m_gameOverText); // If the gmae is over display the game over text
	}

	m_window.display();
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupFontAndText()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	// Draw the altitude text
	m_altitudeText.setFont(m_ArialBlackfont);
	m_altitudeText.setString("ALTITUDE:");
	m_altitudeText.setPosition(10.0f, 450.0f);
	m_altitudeText.setCharacterSize(20);
	m_altitudeText.setOutlineColor(sf::Color::Black);
	m_altitudeText.setFillColor(sf::Color::White);
	m_altitudeText.setOutlineThickness(1.0f);

	// Setup the score text
	m_scoreText.setFont(m_ArialBlackfont);
	m_scoreText.setString("Score: " + std::to_string(m_score));
	m_scoreText.setPosition(10.0f, 10.0f);
	m_scoreText.setCharacterSize(20);
	m_scoreText.setFillColor(sf::Color::White);

	// Setup the game over text
	m_gameOverText.setFont(m_ArialBlackfont);
	m_gameOverText.setString("GAME OVER!");
	m_gameOverText.setPosition(200.0f, 200.0f);
	m_gameOverText.setCharacterSize(50);
	m_gameOverText.setFillColor(sf::Color::Yellow);
}

// Setup all the shapes and lines
void Game::setupShapes()
{
	// Setup the base
	m_base.setSize(sf::Vector2f{ 40.0f, 40.0f });
	m_base.setPosition(380.0f, 405.0f);
	m_base.setFillColor(sf::Color::Yellow);

	// Setup the ground
	m_ground.setSize(sf::Vector2f{ 800.0f, 35.0f });
	m_ground.setPosition(0.0f, GROUND_HEIGHT);
	m_ground.setFillColor(sf::Color::Green);

	// Setup the altitude bar
	m_altitudeBar.setSize(sf::Vector2f{ GROUND_HEIGHT, 18.0f }); // Set the altitude width to the ground height for a measure of altitude
	m_altitudeBar.setPosition(140.0f, 454.0f);
	m_altitudeBar.setFillColor(sf::Color::Yellow);
	m_altitudeBar.setOutlineColor(sf::Color::Black);
	m_altitudeBar.setOutlineThickness(2.0f);

	// Setup the explosion
	m_explosion.setRadius(1);
	m_explosion.setPosition(200.0f, 200.0f);
	m_explosion.setOrigin(m_explosion.getRadius(), m_explosion.getRadius());
	m_explosion.setFillColor(sf::Color::Red);

	// Setup the lines
	m_missile.append(sf::Vertex{ sf::Vector2f{ 400.0f, 445.0f } , sf::Color::Red });
	m_missile.append(sf::Vertex{ sf::Vector2f{ 60.0f, 80.0f } , sf::Color::Red });

	m_asteroid.append(sf::Vertex{ sf::Vector2f{ 50.0f, 0.0f } , sf::Color::White });
	m_asteroid.append(sf::Vertex{ sf::Vector2f{ 300.0f, 200.0f } , sf::Color::White });
}

// Process all input from the mouse
void Game::processMouseEvents(sf::Event t_mouseEvent)
{
	if (m_currentMissileState == MissileStates::ReadyToFire)
	{
		if (sf::Mouse::Left == t_mouseEvent.mouseButton.button)
		{
		
			m_missileDestination = sf::Vector2f{ static_cast<float>(t_mouseEvent.mouseButton.x), static_cast<float>(t_mouseEvent.mouseButton.y) }; // Get the player's click location

			// Initiate the missile to avoid flickering effect
			m_missilePosition = sf::Vector2f{ 400.0f, GROUND_HEIGHT }; // Set the missile's start position to the base position
			m_missile[1] = sf::Vertex{ m_missilePosition, sf::Color::Red }; // Set the missile's line start position

			// Find the velocity
			sf::Vector2f distanceVector = m_missileDestination - sf::Vector2f{ 400.0f, GROUND_HEIGHT }; // Find the disance vector between the click point and the base
			m_missileVelocity = vectorUnitVector(distanceVector) * MISSILE_SPEED; // Find the velocity using the unit vector of the distance vector and a scalar value for speed

			m_currentMissileState = MissileStates::FiredMissile; // Change the missile state
		}
	}
}

// Update the missile's flight
void Game::updateMissile()
{
	if (m_missilePosition.y > m_missileDestination.y && m_missilePosition.y > GROUND_HEIGHT - m_missilePower) // If the missile reaches the mouse click position or the max altitude
	{
		m_missilePosition += m_missileVelocity; // Add the velocity to the missile's position
		m_missile[1] = sf::Vertex{ m_missilePosition, sf::Color::Red }; // Update the missile line's position
	}
	else // once the missile reaches it's end position
	{
		m_explosion.setPosition(m_missilePosition); // Set the explosion's start position

		// Initiate the explosion to avoid flickering effect
		m_explosionSize = 0; // Reset the explosion size
		m_explosion.setRadius(m_explosionSize); // Set the radius of the explosion to the new increased value
		m_explosion.setOrigin(m_explosionSize, m_explosionSize); // Set the origin of the circle to keep it centred

		
		m_missilePower = 0; // Reset the missile's power

		m_currentMissileState = MissileStates::Explosion; // Switch to the explosion state
		
	}
}

// Update the explosion and collision
void Game::updateExplosion()
{
	// Increase the explosion size
	m_explosionSize++; // Increase the explosions radius size
	m_explosion.setRadius(m_explosionSize); // Set the radius of the explosion to the new increased value
	m_explosion.setOrigin(m_explosionSize, m_explosionSize); // Set the origin of the circle to keep it centred

	if (m_asteroidInPlay) // If the asteroid is in play, check collisions
	{
		if (checkCollisions()) // If colliding with the asteroid
		{
			int minimumTime = 120 - m_score; // Set a minimum time for the asteroid to spawn, this will also decrease the maximum time
			if (minimumTime < 0)
			{
				minimumTime = 0; // If the time is a negative value, make it zero
			}

			m_asteroidInPlay = false; // Set the asteroid to no longer in play
			m_asteroidLaunchTime = rand() % 90 + minimumTime; // Set the asteroid launch time to a random number based on the player's current score
			m_score += 5; // Add to the score when an asteroid is destroyed
		}
	}

	if (m_explosionSize >= MAX_EXPLOSION_SIZE) // If the explosion reaches its max size, remove it
	{
		m_currentMissileState = MissileStates::ReadyToFire; // Allow the player to launch another missile
	}
}

// Launch an asteroid
void Game::launchAsteroid()
{
	if (m_asteroidLaunchTime > 0)
	{
		m_asteroidLaunchTime--; // Count down the asteroid launch time
	}
	else
	{
		// Set a random start and end point for the asteroid
		sf::Vector2f asteroidStartPosition{ static_cast<float>(rand() % 700 + 50), 0.0f };
		sf::Vector2f asteroidEndPosition{ static_cast<float>(rand() % 700 + 50), GROUND_HEIGHT };
		
		// Find the asteroids velocity
		sf::Vector2f distanceVector = asteroidEndPosition - asteroidStartPosition; // Find the disance vector between the asteroid's end point and its beginning point
		m_asteroidVelocity = vectorUnitVector(distanceVector) * ASTEROID_SPEED; // Find the velocity using the unit vector of the distance vector and a scalar value for speed

		// Initiate the asteroid to the start position
		m_asteroidPosition = asteroidStartPosition;
		m_asteroid[0] = sf::Vertex{ m_asteroidPosition, sf::Color::White };
		m_asteroid[1] = sf::Vertex{ m_asteroidPosition, sf::Color::White };
		m_asteroidInPlay = true;
	}
}

// Update the asteroid's flight
void Game::updateAsteroid()
{
	if (m_asteroidPosition.y < GROUND_HEIGHT) // Move the asteroid if it hasn't hit the ground
	{
		m_asteroidPosition += m_asteroidVelocity;
		m_asteroid[1] = sf::Vertex{ m_asteroidPosition, sf::Color::White };
	}
	else // Destroy the asteroid if it hits the ground
	{
		//m_asteroidInPlay = false;
		m_gameOver = true;
	}
}

// Check for collisions between asteroid and missile
bool Game::checkCollisions()
{
	bool isColliding{ false }; // The bool that will be returned
	sf::Vector2f distanceVector = m_asteroidPosition - m_missilePosition; // Find the distance vector between the asteroid and the explosion

	if (vectorLengthSquared(distanceVector) < m_explosionSize * m_explosionSize) // Check of the distance length squared is less than the radius squared
	{
		isColliding = true;
	}
	
	return isColliding; // Return whether or not the asteroid is colliding with the explosion
}

// Update the altitude bar and power level for the missile
void Game::updateAltitudeBar()
{
	if (m_currentMissileState != MissileStates::FiredMissile) // If the missile has not been fired, increment the missile power
	{
		if (m_missilePower < GROUND_HEIGHT) // If the power reaches a max power, stop incrementing
		{
			m_missilePower++; // Increment the missile power
		}
	}

	m_altitudeBar.setSize(sf::Vector2f{ m_missilePower, m_altitudeBar.getSize().y }); // Set the altitude width to the ground height for a measure of altitude
}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprite()
{
	if (!m_skylineTexture.loadFromFile("ASSETS\\IMAGES\\skyline.png"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading logo" << std::endl;
	}
	m_skylineSprite.setTexture(m_skylineTexture);
	m_skylineSprite.setPosition(0.0f, 390.0f);
}