// @Author Michael Rainsford Ryan
// C00239510
// @Date 01/12/2018
// Estimated time: 180m
// Session 1: 23:17 - 23:59 - 01/12/2018
// Session 2: 00:00 - 00:50 - 02/12/2018

#include "Game.h"
#include "MyVector2.h"
#include <iostream>


/// <summary>
/// default construcor
/// pass parameters fpr sfml window, setup m_exitGame
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 800, 480, 32 }, "SFML Game" },
	m_exitGame{ false } //when true game will exit
{
	setupFontAndText(); // load font 
	setupSprite(); // load texture
	setupShapes();
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

	updateMissile();
}

/// <summary>
/// draw the frame and then switch bufers
/// </summary>
void Game::render()
{
	m_window.clear();
	m_window.draw(m_ground);
	m_window.draw(m_base);
	m_window.draw(m_altitudeBar);
	m_window.draw(m_missile);
	m_window.draw(m_asteroid);
	m_window.draw(m_altitudeText);
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
	m_altitudeText.setFont(m_ArialBlackfont);
	m_altitudeText.setString("Altitude:");
	m_altitudeText.setStyle( sf::Text::Italic );
	m_altitudeText.setPosition(40.0f, 445.0f);
	m_altitudeText.setCharacterSize(25);
	m_altitudeText.setOutlineColor(sf::Color::Black);
	m_altitudeText.setFillColor(sf::Color::White);
	m_altitudeText.setOutlineThickness(1.0f);

}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprite()
{
	if (!m_logoTexture.loadFromFile("ASSETS\\IMAGES\\SFML-LOGO.png"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading logo" << std::endl;
	}
	m_logoSprite.setTexture(m_logoTexture);
	m_logoSprite.setPosition(300.0f, 180.0f);
}

void Game::setupShapes()
{
	// Setup the base
	m_base.setSize(sf::Vector2f{ 50.0f, 50.0f });
	m_base.setPosition(375.0f, 395.0f);
	m_base.setFillColor(sf::Color::Yellow);

	// Setup the ground
	m_ground.setSize(sf::Vector2f{ 800.0f, 35.0f });
	m_ground.setPosition(0.0f, 445.0f);
	m_ground.setFillColor(sf::Color::Green);

	// Setup the ground
	m_altitudeBar.setSize(sf::Vector2f{ 400.0f, 25.0f });
	m_altitudeBar.setPosition(200.0f, 450.0f);
	m_altitudeBar.setFillColor(sf::Color::White);

	/// Temporary code
	m_missile.append(sf::Vertex{ sf::Vector2f{ 400.0f, 445.0f } , sf::Color::Red });
	m_missile.append(sf::Vertex{ sf::Vector2f{ 60.0f, 80.0f } , sf::Color::Red });

	m_asteroid.append(sf::Vertex{ sf::Vector2f{ 50.0f, 0.0f } , sf::Color::White });
	m_asteroid.append(sf::Vertex{ sf::Vector2f{ 300.0f, 200.0f } , sf::Color::White });
}

// Process all input from the mouse
void Game::processMouseEvents(sf::Event t_mouseEvent)
{
	if (sf::Mouse::Left == t_mouseEvent.mouseButton.button)
	{
		m_clickPosition = sf::Vector2f{ static_cast<float>(t_mouseEvent.mouseButton.x), static_cast<float>(t_mouseEvent.mouseButton.y) };
		m_missilePosition = sf::Vector2f{ 400.0f, 445.0f };
	}
}

void Game::updateMissile()
{
	sf::Vector2f distanceVector = m_clickPosition - sf::Vector2f{ 400.0f, 445.0f }; // Find the disance vector between the click point and the base
	sf::Vector2f missileVelocity = vectorUnitVector(distanceVector) * 2.0f; // Find the velocity using the unit vector of the distance vector and a scalar value for speed

	if (m_missilePosition.y > m_clickPosition.y)
	{
		m_missilePosition += missileVelocity;
		m_missile[1] = sf::Vertex{ m_missilePosition, sf::Color::Red };
	}
}