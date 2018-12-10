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
// Session 8: 22:41 - 23:35 - 06/12/2018
// Session 9: 17:01 - 17:57 - 10/12/2018
// Time taken to add extra stuff:  130m
// Total time overall: 353m
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Known bugs or notes:
// - No known bugs but more code could have been simplified/put into functions and some
// constants could have had a smaller scope.

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
	setupSprites(); // load texture
	setupShapes(); // Setup the shapes and lines
	SetupAudio();
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
				if (!m_coolStuffOn) // If the extra effects are being turned on, initialise the sprites' angle
				{
					setSpriteAngle(m_rocketSprite, m_missileVelocity);
					setSpriteAngle(m_meteorSprite, m_asteroidVelocity);
				}

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
	// If the game hasn't ended, do the main game updates
	if (!m_gameOver)
	{
		updateAltitudeBar();// Update the altitude/missile power bar

		if (m_currentMissileState == MissileStates::FiredMissile) // Update the missile if the missile is active
		{
			updateMissile(); 
		}
		else if (m_currentMissileState == MissileStates::Explosion) // Update the explosion is the explosion is active
		{
			updateExplosion(); 
		}
		if (!m_asteroidInPlay) // Launch the asteroid if there are none in play
		{
			launchAsteroid();
		}
		else  // Move the asteroid if the asteroid is active
		{
			updateAsteroid();
		}
	}

	// If the cool stuff have been toggled
	if (m_coolStuffOn)
	{
		updateExtraEffects(); // update the cool stuff
	}
}

/// <summary>
/// draw the frame and then switch bufers
/// </summary>
void Game::render()
{
	m_window.clear();

	if (m_coolStuffOn)
	{
		m_window.draw(m_backgroundSprite);
		m_window.draw(m_skylineSprite);
	}
	else
	{
		m_window.draw(m_ground);
		m_window.draw(m_base);
	}

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

	// Draw the extra graphics
	if (m_coolStuffOn)
	{
		m_window.draw(m_scoreText);

		if (m_asteroidInPlay)
		{
			m_window.draw(m_meteorSprite);
		}

		if (m_currentMissileState == MissileStates::FiredMissile)
		{
			m_window.draw(m_rocketSprite);
		}

		if (m_currentMissileState == MissileStates::Explosion)
		{
			m_window.draw(m_explosionSprite);
		}
	}

	if (m_gameOver)
	{
		m_window.draw(m_gameOverText); // If the game is over display the game over text
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

			// Setup the sprite for the extra effects
			if (m_coolStuffOn)
			{
				setSpriteAngle(m_rocketSprite, m_missileVelocity);
				m_launchSound.play();
			}
			
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

		if (m_coolStuffOn) // If the extra cool effects have been enabled
		{
			m_screenShakeTimer = 30; // Shake the screen for 30 seconds
			m_explosionSound.play();
		}
		m_explosionSprite.setPosition(m_missilePosition);
		m_explosionSprite.setScale(0.35f, 0.35f); // Set the explosion to a scale similar to the circle explosion
	}
}

// Update the explosion and collision
void Game::updateExplosion()
{
	// Increase the explosion size
	m_explosionSize++; // Increase the explosions radius size
	m_explosion.setRadius(m_explosionSize); // Set the radius of the explosion to the new increased value
	m_explosion.setOrigin(m_explosionSize, m_explosionSize); // Set the origin of the circle to keep it centred


	if (m_coolStuffOn) // Animates the explosion effect if the extra effects are enabled
	{
		animateExplosion();
	}

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

		// Setup the sprite for the extra effects
		if (m_coolStuffOn)
		{
			setSpriteAngle(m_meteorSprite, m_asteroidVelocity);
		}
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
/// load the textures and setup the sprites
/// </summary>
void Game::setupSprites()
{
	if (!m_skylineTexture.loadFromFile("ASSETS\\IMAGES\\skyline.png"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading skyline" << std::endl;
	}

	if (!m_rocketTexture.loadFromFile("ASSETS\\IMAGES\\rocket.png"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading rocket" << std::endl;
	}

	if (!m_meteorTexture.loadFromFile("ASSETS\\IMAGES\\meteor.png"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading meteor" << std::endl;
	}

	if (!m_backgroundTexture.loadFromFile("ASSETS\\IMAGES\\background.png"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading background" << std::endl;
	}

	if (!m_explosionTexture.loadFromFile("ASSETS\\IMAGES\\explosion.png"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading explosion" << std::endl;
	}

	m_skylineSprite.setTexture(m_skylineTexture);
	m_skylineSprite.setPosition(0.0f, 390.0f);

	m_rocketSprite.setTexture(m_rocketTexture);
	m_rocketSprite.setOrigin(m_rocketSprite.getGlobalBounds().width - 10, m_rocketSprite.getGlobalBounds().height / 2);
	m_rocketSprite.setScale(0.5f, 0.5f);

	m_meteorSprite.setTexture(m_meteorTexture);
	m_meteorSprite.setTextureRect(sf::IntRect{ 0, 0, 119, 46 });
	m_meteorSprite.setOrigin(m_meteorSprite.getGlobalBounds().width - 10, m_meteorSprite.getGlobalBounds().height / 2);
	m_meteorSprite.setScale(0.5f, 0.5f);

	m_backgroundSprite.setTexture(m_backgroundTexture);
	m_backgroundSprite.setPosition(0.0f, 0.0f);

	m_explosionSprite.setTexture(m_explosionTexture);
	m_explosionSprite.setPosition(300, 300);
	m_explosionSprite.setScale(0.35f, 0.35f); // Set the explosion to a scale similar to the circle explosion
	m_explosionSprite.setOrigin(73, 73);
}

// Setup the load and setup the audio
void Game::SetupAudio()
{
	// Load the sound files
	if (!m_explosionBuffer.loadFromFile("ASSETS\\AUDIO\\explosion.wav"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading explosion sound" << std::endl;
	}

	// Load the sound files
	if (!m_launchBuffer.loadFromFile("ASSETS\\AUDIO\\rocket_launch.wav"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading rocket launch sound" << std::endl;
	}

	// Assign the buffers
	m_explosionSound.setBuffer(m_explosionBuffer);

	m_launchSound.setBuffer(m_launchBuffer);
}

// Updates the extra effects of the game
void Game::updateExtraEffects()
{
	m_scoreText.setString("Score: " + std::to_string(m_score)); // Update the score text

	// increment the frame timer
	if (m_frameTimer < 60)
	{
		m_frameTimer++;
	}
	else
	{
		m_frameTimer = 0;
	}

	// Update the rocket sprite
	if (m_currentMissileState == MissileStates::FiredMissile)
	{
		m_rocketSprite.setPosition(m_missilePosition);
	}

	// Update the asteroid sprite
	if (m_asteroidInPlay)
	{
		m_meteorSprite.setPosition(m_asteroidPosition);

		// Animate the asteroid
		if (m_frameTimer % 20 > 10)
		{
			m_meteorSprite.setTextureRect(sf::IntRect{ 119, 0, 119, 46 });
		}
		else
		{
			m_meteorSprite.setTextureRect(sf::IntRect{ 0, 0, 119, 46 });
		}
	}

	if (m_screenShakeTimer > 0)
	{
		viewShake();
		m_screenShakeTimer--;
	}
}

// Shakes the screen for a preset amount time
void Game::viewShake()
{
	if (m_screenShakeTimer % 3 > 1) // Only shake the screen every three frames
	{
		m_window.setView(sf::View{ sf::FloatRect(static_cast<float>(rand() % 16 - 8), static_cast<float>(rand() % 16 - 8), static_cast<float>(m_window.getSize().x), static_cast<float>(m_window.getSize().y)) });
	}
}

// Finds and sets the angle of a sprite
void Game::setSpriteAngle(sf::Sprite & t_sprite, sf::Vector2f t_velocity)
{
	float angleRad = atan2f(t_velocity.y, t_velocity.x); // Finds the angle the object is going in radians
	float angleDeg = angleRad * 180 / PI; // Converts the angle to degrees

	t_sprite.setRotation(angleDeg); // Sets the angle of the sprite
}

void Game::animateExplosion()
{
	float explosionScale = m_explosionSize / 35.0f;
	m_explosionSprite.setScale(explosionScale, explosionScale);
}