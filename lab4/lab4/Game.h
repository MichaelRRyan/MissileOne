// @Author Michael Rainsford Ryan
#ifndef GAME
#define GAME

#include <SFML/Graphics.hpp>

class Game
{
public:
	Game();
	~Game();
	void run();

private:

	void processEvents();
	void processMouseEvents(sf::Event t_mouseEvent);
	void update(sf::Time t_deltaTime);
	void render();

	void setupFontAndText();
	//void setupSprite();
	void setupShapes();

	void launchAsteroid();
	void updateMissile();
	void updateExplosion();
	void updateAsteroid();
	void updateAltitudeBar();
	bool checkCollisions();

	// Declare the enums and bools
	enum missileState{ readyToFire, firedMissile, explosion }; // Enum class for the missile state
	missileState m_currentMissileState{ readyToFire }; // A enum variable of the missile state

	bool m_asteroidInPlay{ false }; // To check if the asteroid is in play
	bool m_gameOver{ false }; // To check if the game is in progress/to end the game
	bool m_exitGame; // control exiting game

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	sf::Text m_altitudeText; // text used for altitude
	sf::Text m_scoreText; // Text used for the player's score
	sf::Text m_gameOverText; // Text object used to display the game over text

	// Declare shapes and lines
	sf::RectangleShape m_ground; // Ground plain shape
	sf::RectangleShape m_base; // Base shape
	sf::RectangleShape m_altitudeBar; // Altitude bar shape
	sf::CircleShape m_explosion; // Missile explosion shape
	sf::VertexArray m_missile{ sf::Lines }; // Missile array
	sf::VertexArray m_asteroid{ sf::Lines }; // Asteroid array

	// Declare vectors
	sf::Vector2f m_missileDestination{ 0.0f, 0.0f }; // Location of the mouse click
	sf::Vector2f m_missilePosition{ 0.0f, 0.0f }; // The location of the missile
	sf::Vector2f m_missileVelocity{ 0.0f, 0.0f }; // The velocity of the missile

	sf::Vector2f m_asteroidPosition{ 0.0f, 0.0f }; // The location of the asteroid
	sf::Vector2f m_asteroidVelocity{ 0.0f, 0.0f }; // The velocity of the asteroid

	// Declare single variables
	float m_groundHeight{ 445.0f }; // The height of the ground
	float m_missileSpeed{ 2.0f }; // The speed multiplier of the missile
	float m_asteroidSpeed{ 0.8f }; // The speed multiplier of the asteroid
	float m_missilePower{ 1.0f }; // The altitude at which the missile can reach
	float m_explosionSize{ 0.0f }; // Current explosion size

	int m_score{ 0 }; // The players score
	int m_asteroidLaunchTime{ 20 }; // The launch time for the asteroid. Set to 20 frames for the first asteroid
	const int M_MAX_EXPLOSION_SIZE{ 30 }; // Maximum explosion size

};

#endif // !GAME

