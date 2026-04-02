#ifndef GAMECONTROLLER_H_
#define GAMECONTROLLER_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <map>
#include <memory>
#include <iostream>
#include <sstream>
#include <vector>

const int INVALID_KEY = 0;

class GameWorld;

class GameController
{
  public:
	void run(int argc, char* argv[], GameWorld* gw, std::string windowTitle);

	bool getKeyIfAny(int& value)
	{
		if (m_lastKeyHit != INVALID_KEY)
		{
			value = m_lastKeyHit;
			m_lastKeyHit = INVALID_KEY;
			return true;
		}
		return false;
	}

	void putBackKey(int key)
	{
		m_lastKeyHit = key;
	}

	void playSound(int soundID);
	void stopSounds();

	void setGameStatText(std::string text)
	{
		m_gameStatText = text;
	}

	void doSomething();

	[[noreturn]] void quitGame();

	  // Meyers singleton pattern
	static GameController& getInstance()
	{
		static GameController instance;
		return instance;
	}
	
	~GameController() {
		stopSounds();
		m_soundBuffers.clear();
	}
	
	// SFML-specific methods
	sf::RenderWindow& getWindow() { return m_window; }
	sf::Texture& getTexture(int imageID);
	void loadTexture(int imageID, const std::string& filename);

private:
    enum GameControllerState : int;

	GameWorld*	m_gw;
	GameControllerState	m_gameState;
	GameControllerState	m_nextStateAfterPrompt;
	GameControllerState	m_nextStateAfterAnimate;
	int			m_lastKeyHit;
	bool		m_singleStep;
	bool		m_postInitPreCleanup;
	bool		m_shouldStopSound;
	bool		m_giveUpLevel;
	std::string m_gameStatText;
	std::string m_mainMessage;
	std::string m_secondMessage;
	int			m_curIntraFrameTick;
	unsigned long m_totalFramesDisplayed;  // long enough for 16 months at 10ms/tick
	using SoundMapType = std::map<int, std::string>;
	using TextureMapType = std::map<int, sf::Texture>;
    using ImageNameMapType = std::map<int, std::string>;
	SoundMapType m_soundMap;
	TextureMapType m_textures;
    ImageNameMapType m_imageNameMap;
	std::map<int, unsigned long> m_frameAdvanceInterval;
	std::map<int, sf::SoundBuffer> m_soundBuffers;
	bool		m_playerWon;
	std::map<int, int> m_spriteDepth;
	
	// SFML objects
	sf::RenderWindow m_window;
	sf::Font m_font;
	
	// Sprite cache to avoid recreating sprites every frame
	mutable std::map<int, std::unique_ptr<sf::Sprite>> m_spriteCache;
	
	// Active sounds to prevent immediate destruction
	std::vector<std::unique_ptr<sf::Sound>> m_activeSounds;

    void setGameState(GameControllerState s);
	void initDrawersAndSounds();
	void initSpritesAndDepth();
	void initSoundSystem();
	void initFontSystem();
    bool passesThruWhenSingleStepping(int key) const;
	void displayGamePlay();
    void reportLeakedGraphObjects() const;
	void handleEvents();
	void drawPrompt(const std::string& mainMessage, const std::string& secondMessage);
	void drawScoreAndLives(const std::string& text);
	int convertSFMLKeyToGameKey(sf::Keyboard::Key key);
	int getNumFrames(int imageID);

	static const int kDefaultMsPerTick = 10;
	static const int WINDOW_WIDTH = 768;
	static const int WINDOW_HEIGHT = 868;
	static const int STATUS_TEXT_HEIGHT = 60;
};

inline GameController& Game()
{
	return GameController::getInstance();
}

#endif // GAMECONTROLLER_H_
