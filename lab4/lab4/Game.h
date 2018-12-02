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
	void setupSprite();
	void setupShapes();

	void launchAsteroid();
	void updateMissile();
	void updateExplosion();
	void updateAsteroid();
	bool checkCollisions();

	// Declare the enums and bools
	enum missileState{ readyToFire, firedMissile, explosion }; // Enum class for the missile state
	missileState m_currentMissileState{ readyToFire }; // A enum variable of the missile state

	bool m_asteroidInPlay{ false }; // To check if the asteroid is in play
	bool m_exitGame; // control exiting game

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	sf::Text m_altitudeText; // text used for altitude
	sf::Texture m_logoTexture; // texture used for sfml logo
	sf::Sprite m_logoSprite; // sprite used for sfml logo

	// Declare shapes and lines
	sf::RectangleShape m_ground; // Ground plain shape
	sf::RectangleShape m_base; // Base shape
	sf::RectangleShape m_altitudeBar; // Altitude bar shape
	sf::CircleShape m_explosion; // Missile explosion shape
	sf::VertexArray m_missile{ sf::Lines }; // Missile array
	sf::VertexArray m_asteroid{ sf::Lines }; // Asteroid array

	// Declare vectors
	sf::Vector2f m_clickPosition{ 0.0f, 0.0f }; // Location of the mouse click
	sf::Vector2f m_missilePosition{ 0.0f, 0.0f }; // The location of the missile

	sf::Vector2f m_asteroidPosition{ 0.0f, 0.0f }; // The location of the asteroid
	sf::Vector2f m_asteroidStartPosition{ 0.0f, 0.0f }; // The beginning point of the asteroid
	sf::Vector2f m_asteroidEndPosition{ 0.0f, 0.0f }; // The end point of the asteroid

	// Declare single variables
	float m_groundHeight{ 445.0f }; // The height of the ground

	float m_missileSpeed{ 1.0f }; // The speed multiplier of the missile
	float m_asteroidSpeed{ 0.8f }; // The speed multiplier of the asteroid

	int m_asteroidLaunchTime{ 20 }; // The launch time for the asteroid. Set to 20 frames for the first asteroid

	int m_explosionSize{ 1 }; // Current explosion size
	const int M_MAX_EXPLOSION_SIZE{ 30 }; // Maximum explosion size

};

#endif // !GAME

