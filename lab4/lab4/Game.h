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

	void updateMissile();

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	sf::Text m_altitudeText; // text used for altitude
	sf::Texture m_logoTexture; // texture used for sfml logo
	sf::Sprite m_logoSprite; // sprite used for sfml logo

	sf::RectangleShape m_ground; // Ground plain shape
	sf::RectangleShape m_base; // Base shape
	sf::RectangleShape m_altitudeBar; // Altitude bar shape
	sf::VertexArray m_missile{ sf::Lines }; // Missile array
	sf::VertexArray m_asteroid{ sf::Lines }; // Asteroid array

	sf::Vector2f m_clickPosition{ 0, 0 }; // Location of the mouse click
	sf::Vector2f m_missilePosition{ 0, 0 }; // The location of the missile

	bool m_exitGame; // control exiting game

};

#endif // !GAME

