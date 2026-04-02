#include "GameController.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdio>
#include <string>
#include <map>
#include <set>
#include <utility>
#include <cstdlib>
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <memory>
using namespace std;

const int MS_PER_FRAME = 16; // ~60 FPS

struct SpriteInfo
{
    int imageID;
    int frameNum;
    std::string fileName;
    std::string imageName;
};

enum GameController::GameControllerState : int {
    welcome, contgame, init, cleanup, makemove, animate, gameover, prompt, quit, not_applicable
};

void GameController::initSpritesAndDepth()
{
    SpriteInfo drawers[] = {
        { IID_PLAYER, 0, "mario1.png", "PLAYER" },
        { IID_LEMMING_FACTORY, 0, "factory.png", "LEMMING_FACTORY" },
        { IID_LEMMING_FACTORY, 1, "factory.png", "LEMMING_FACTORY" },
        { IID_LEMMING_FACTORY, 2, "factory.png", "LEMMING_FACTORY" },
        { IID_FIREBALL, 0, "fire1.png", "FIREBALL" },
        { IID_ICE_MONSTER, 0, "yeti1.png", "ICE_MONSTER" },
        { IID_ICE_MONSTER, 1, "yeti2.png", "ICE_MONSTER" },
        { IID_ICE_MONSTER, 2, "yeti3.png", "ICE_MONSTER" },
        { IID_ICE_MONSTER, 3, "yeti4.png", "ICE_MONSTER" },
        { IID_FLOOR, 0, "wall.png", "FLOOR" },
        { IID_TRAMPOLINE, 0, "trampoline.png", "TRAMPOLINE" },
        { IID_BONFIRE, 0, "bonfire1.png", "BONFIRE" },
        { IID_BONFIRE, 1, "bonfire2.png", "BONFIRE" },
        { IID_NET, 0, "net.png", "NET" },
        { IID_LEMMING, 0, "lemming1.tga", "LEMMING" },
        { IID_LEMMING, 1, "lemming1.tga", "LEMMING" },
        { IID_LEMMING, 2, "lemming1.tga", "LEMMING" },
        { IID_LEMMING, 3, "lemming2.tga", "LEMMING" },
        { IID_LEMMING, 4, "lemming2.tga", "LEMMING" },
        { IID_ONE_WAY_DOOR, 0, "right_door.png", "ONE_WAY_DOOR" },
        { IID_EXIT, 0, "exit.png", "EXIT" },
        { IID_PHEROMONE, 0, "pheromone.png", "PHEROMONE" },
        { IID_SPRING, 0, "spring.png", "SPRING" },
        { IID_DEMO_OBJECT, 0, "valentine.png", "DEMO_OBJECT" }, // only for demo, not the game
    };

    // Configure sprite depths for rendering order (higher depth = background)
    m_spriteDepth[IID_PLAYER] = 0;
    m_spriteDepth[IID_LEMMING_FACTORY] = 0;
    m_spriteDepth[IID_FIREBALL] = 1;
    m_spriteDepth[IID_ICE_MONSTER] = 0;
    m_spriteDepth[IID_FLOOR] = 2;
    m_spriteDepth[IID_TRAMPOLINE] = 1;
    m_spriteDepth[IID_BONFIRE] = 1;
    m_spriteDepth[IID_NET] = 2;
    m_spriteDepth[IID_LEMMING] = 0;
    m_spriteDepth[IID_ONE_WAY_DOOR] = 2;
    m_spriteDepth[IID_EXIT] = 2;
    m_spriteDepth[IID_PHEROMONE] = 1;
    m_spriteDepth[IID_SPRING] = 1;
    m_spriteDepth[IID_DEMO_OBJECT] = 0;  // only for demo, not the game

    // Intended only for stationary objects with multiple frames, since moving
    // objects advance frame when they move
    m_frameAdvanceInterval[IID_BONFIRE] = 15;  // small number flickers too fast
    m_frameAdvanceInterval[IID_LEMMING_FACTORY] = 1;  // leftover from earlier?
    m_frameAdvanceInterval[IID_EXIT] = 1;             // leftover from earlier?

    // Load sprite textures
    string assetPath = m_gw->assetPath();
    if (!assetPath.empty() && assetPath.back() != '/')
        assetPath += '/';

    for (const auto& drawer : drawers)
    {
        string fullPath = assetPath + drawer.fileName;
        sf::Texture texture;
        if (texture.loadFromFile(fullPath))
        {
            // Enable texture smoothing to reduce flickering
            texture.setSmooth(true);
            
            // Create a unique key for each frame
            int key = drawer.imageID * TEXTURE_KEY_MULTIPLIER + drawer.frameNum;
            m_textures[key] = std::move(texture);
            m_imageNameMap[drawer.imageID] = drawer.imageName;
        }
        else
        {
            cout << "Failed to load texture: " << fullPath << endl;
        }
    }
}

void GameController::initSoundSystem()
{
    // Map sound IDs to their file names
    m_soundMap[SOUND_PLAYER_DIE] = "death.wav";
    m_soundMap[SOUND_ENEMY_DIE] = "enemydie.wav";
    m_soundMap[SOUND_LEMMING_SAVED] = "goodie.wav";
    m_soundMap[SOUND_FINISHED_LEVEL] = "finished.wav";
    m_soundMap[SOUND_THEME] = "theme.wav";
    m_soundMap[SOUND_BOUNCE] = "boing.wav";
    m_soundMap[SOUND_LEMMING_DIE] = "ouch.wav";
    m_soundMap[SOUND_DEMO_SOUND] = "goodie.wav";  // only for demo, not the game

    // Load sound buffers from files
    string assetPath = m_gw->assetPath();
    if (!assetPath.empty() && assetPath.back() != '/')
        assetPath += '/';

    for (const auto& sound : m_soundMap)
    {
        string fullPath = assetPath + sound.second;
        sf::SoundBuffer buffer;
        if (buffer.loadFromFile(fullPath))
        {
            m_soundBuffers[sound.first] = std::move(buffer);
            // Note: sf::Sound will be created when needed to avoid constructor issues
        }
        else
        {
            cout << "Failed to load sound: " << fullPath << endl;
        }
    }
}

void GameController::initFontSystem()
{
    bool fontLoaded = false;
    
    string fontPaths[] = {
    // Try monospace (fixed-width) fonts first for better stat text display
          // macOS monospace fonts
        "/System/Library/Fonts/Courier.ttc",
        "/System/Library/Fonts/Supplemental/Courier New.ttf",
        "/System/Library/Fonts/Monaco.ttf",
        "/System/Library/Fonts/Menlo.ttc",
          // Windows monospace fonts
        "C:/Windows/Fonts/cour.ttf",
        "C:/Windows/Fonts/consola.ttf",
        "C:/Windows/Fonts/lucon.ttf",
          // Linux monospace fonts
        "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf",
          // Fallback to any available system fonts
        "/System/Library/Fonts/Helvetica.ttc",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
    };

    for (const auto& fontPath : fontPaths)
    {
        if (filesystem::exists(fontPath)  &&  m_font.openFromFile(fontPath))
        {
            fontLoaded = true;
            break;
        }
    }
    
    if (!fontLoaded)
    {
        cout << "Warning: Could not load font. Text may not display correctly." << endl;
    }
}

void GameController::initDrawersAndSounds()
{
    initSpritesAndDepth();
    initSoundSystem();
    initFontSystem();
}

bool GameController::passesThruWhenSingleStepping(int key) const
{
    static set<int> passThruKeys = {
        'T', 'N', 'P', 'S', 't', 'n', 'p', 's', '<', '>', KEY_PRESS_LEFT,
        KEY_PRESS_RIGHT, KEY_PRESS_UP, KEY_PRESS_DOWN,
    };
    return passThruKeys.find(key) != passThruKeys.end();
}

void GameController::run(int /* argc */, char* /* argv */ [], GameWorld* gw, string windowTitle)
{
    gw->setController(this);
    m_gw = gw;
    setGameState(welcome);
    m_lastKeyHit = INVALID_KEY;
    m_singleStep = false;
    m_shouldStopSound = false;
    m_curIntraFrameTick = 0;
    m_totalFramesDisplayed = 0;
    m_playerWon = false;
    m_postInitPreCleanup = false;
    m_giveUpLevel = false;

    // Create SFML window
    m_window.create(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), windowTitle);
    m_window.setFramerateLimit(60);
    m_window.setVerticalSyncEnabled(true); // Enable VSync to prevent screen tearing

    initDrawersAndSounds();

    // Game loop
    sf::Clock clock;
    sf::Time accumulator = sf::Time::Zero;
    const sf::Time timeStep = sf::milliseconds(MS_PER_FRAME);

    while (m_window.isOpen() && m_gameState != quit)
    {
        sf::Time frameTime = clock.restart();
        accumulator += frameTime;

        handleEvents(); // handle events like key presses

        // Fixed timestep game logic - run once per frame only
        if (accumulator >= timeStep)
        {
            doSomething();
            accumulator -= timeStep;
        }

        // Render only when necessary to avoid flickering
        if (m_gameState == animate || m_gameState == prompt)
        {
            m_window.clear(sf::Color::Black);
            if (m_gameState == animate)
            {
                displayGamePlay();
            }
            else if (m_gameState == prompt)
            {
                drawPrompt(m_mainMessage, m_secondMessage);
            }
            m_window.display();
        }
    }

    delete m_gw;
    reportLeakedGraphObjects();
}

void GameController::handleEvents()
{
    while (auto event = m_window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            quitGame();
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            int key = convertSFMLKeyToGameKey(keyPressed->code);
            m_lastKeyHit = key;
            
            // Handle quit key immediately in all game states
            if (key == 'q')
            {
                // Use exit() to bypass SFML cleanup issues
                delete m_gw;
                reportLeakedGraphObjects();
                std::exit(0);
            }
        }
        else if (const auto* textEntered = event->getIf<sf::Event::TextEntered>())
        {
            // Handle text input for characters like '>', '<', 'T', 'N', etc.
            // Only process printable ASCII characters (32-127), not control characters
            // This avoids interfering with special keys handled by KeyPressed
            if (textEntered->unicode >= 32 && textEntered->unicode < 128)
            {
                int key = static_cast<int>(textEntered->unicode);
                switch (key)
                {
                  case 'f': m_singleStep = true;   break;
                  case 'r': m_singleStep = false;  break;
                  case 'g': m_giveUpLevel = true;  break;
                  default:  m_lastKeyHit = key;    break;
                }
            }
        }
    }
}

int GameController::convertSFMLKeyToGameKey(sf::Keyboard::Key key)
{
    switch (key)
    {
    case sf::Keyboard::Key::Left:   return KEY_PRESS_LEFT;
    case sf::Keyboard::Key::Right:  return KEY_PRESS_RIGHT;
    case sf::Keyboard::Key::Up:     return KEY_PRESS_UP;
    case sf::Keyboard::Key::Down:   return KEY_PRESS_DOWN;
    case sf::Keyboard::Key::Tab:         return KEY_PRESS_TAB;
    case sf::Keyboard::Key::Space:       return KEY_PRESS_SPACE;
    case sf::Keyboard::Key::Escape:      return KEY_PRESS_ESCAPE;
    case sf::Keyboard::Key::Enter:       return '\r';
    case sf::Keyboard::Key::Q:           return 'q';
    default:                             return INVALID_KEY;
    }
}

void GameController::playSound(int soundID)
{
    if (soundID == SOUND_NONE)
        return;

    auto bufferIt = m_soundBuffers.find(soundID);
    if (bufferIt == m_soundBuffers.end())
        return;

    // Clean up finished sounds first
    m_activeSounds.erase(
        std::remove_if(m_activeSounds.begin(), m_activeSounds.end(),
            [](const std::unique_ptr<sf::Sound>& sound) {
                return sound->getStatus() == sf::SoundSource::Status::Stopped;
            }),
        m_activeSounds.end()
    );
        
    // Create a new sound with the buffer and add to active sounds
    auto sound = std::make_unique<sf::Sound>(bufferIt->second);
    sound->play();
    m_activeSounds.push_back(std::move(sound));
}

void GameController::stopSounds()
{
    for (auto& sound : m_activeSounds) {
        if (sound) {
            sound->stop();
        }
    }
    m_activeSounds.clear();
}

void GameController::setGameState(GameControllerState s)
{
    if (m_gameState != quit)
        m_gameState = s;
}

[[noreturn]] void GameController::quitGame()
{
    // Stop and clear all active sounds to prevent mutex issues
    stopSounds();
    
    // Clear sound buffers to prevent mutex issues
    m_soundBuffers.clear();
    
    setGameState(quit);
    m_window.close();
    if (m_postInitPreCleanup)  // might be false if aborted game
    {
        m_gw->cleanUp();
        m_postInitPreCleanup = false;
    }
    // Explicitly exit to ensure program terminates properly
    delete m_gw;
    reportLeakedGraphObjects();
    std::exit(0);
}

void GameController::doSomething()
{
    switch (m_gameState)
    {
    case not_applicable:
        break;
    case welcome:
        playSound(SOUND_THEME);
        m_mainMessage = "Welcome to Lemmings!";
        m_secondMessage = "Press Enter to begin play...";
        setGameState(prompt);
        m_nextStateAfterPrompt = init;
        m_shouldStopSound = true;
        break;
    case contgame:
        m_mainMessage = "You lost a life!";
        m_secondMessage = "Press Enter to continue playing...";
        setGameState(prompt);
        m_nextStateAfterPrompt = cleanup;
        break;
    case makemove:
        m_curIntraFrameTick = ANIMATION_POSITIONS_PER_TICK;
        m_nextStateAfterAnimate = not_applicable;
        if (m_giveUpLevel) {
            m_giveUpLevel = false;
            // Player gave up - treat as death
            m_gw->decLives();
            m_nextStateAfterAnimate = (m_gw->isGameOver() ? gameover : contgame);
        }
        else
        { 
            int status = m_gw->move();
            if (status == GWSTATUS_PLAYER_DIED)
            {
                m_nextStateAfterAnimate = (m_gw->isGameOver() ? gameover : contgame);
            }
            else if (status == GWSTATUS_FINISHED_LEVEL)
            {
                m_gw->advanceToNextLevel();
                m_nextStateAfterAnimate = cleanup;
            }
            else if (status == GWSTATUS_PLAYER_WON)
            {
                m_playerWon = true;
                m_nextStateAfterAnimate = gameover;
            }
        }
        setGameState(animate);
        break;
    case animate:
        if (m_curIntraFrameTick-- <= 0)
        {
            if (m_nextStateAfterAnimate != not_applicable)
                setGameState(m_nextStateAfterAnimate);
            else if (!m_singleStep)
                setGameState(makemove);
            else
            {
                int key;
                if (getKeyIfAny(key))
                {
                    if (passesThruWhenSingleStepping(key))
                        putBackKey(key);
                    setGameState(makemove);
                }
            }
        }
        break;
    case gameover:
        {
            ostringstream oss;
            oss << (m_playerWon ? "You won the game!" : "Game Over!")
                << " Final score: " << m_gw->getScore() << "!";
            m_mainMessage = oss.str();
        }
        m_secondMessage = "Press Enter to quit...";
        setGameState(prompt);
        m_nextStateAfterPrompt = quit;
        break;
    case prompt:
        {
            int key;
            if (getKeyIfAny(key) && key == '\r')
            {
                setGameState(m_nextStateAfterPrompt);
                if (m_shouldStopSound)
                {
                    stopSounds();
                    m_shouldStopSound = false;
                }
            }
        }
        break;
    case init:
        {
            int status = m_gw->init();
            m_postInitPreCleanup = true;
            // Stop all sounds (note: individual sounds are managed differently now)
            // In SFML 3.0, we'd need a different approach to stop all sounds
            
            if (status == GWSTATUS_PLAYER_WON)
            {
                m_playerWon = true;
                setGameState(gameover);
            }
            else if (status == GWSTATUS_LEVEL_ERROR)
            {
                m_mainMessage = "Level error: " + m_gw->getErrorMessage();
                m_secondMessage = "Press Enter to quit...";
                setGameState(prompt);
                m_nextStateAfterPrompt = quit;
            }
            else
                setGameState(makemove);
        }
        break;
    case cleanup:
        if (m_postInitPreCleanup)  // should aways be true here
        {
            m_gw->cleanUp();
            m_postInitPreCleanup = false;
        }
        setGameState(init);
        break;
    case quit:
        // Stop all sounds and clear resources to prevent mutex issues
        stopSounds();
        m_soundBuffers.clear();
        m_window.close();
        // Explicitly exit to ensure program terminates properly
        delete m_gw;
        reportLeakedGraphObjects();
        std::exit(0);
        break;
    }
}

void GameController::displayGamePlay()
{
    m_totalFramesDisplayed++;
    std::set<GraphObject*>& graphObjects = GraphObject::getGraphObjects();

    // Convert game coordinates to screen coordinates
    // Reserve space for status text at the top
    const double GAME_AREA_HEIGHT = WINDOW_HEIGHT - STATUS_TEXT_HEIGHT;
    const double SCALE_X = WINDOW_WIDTH / (double)VIEW_WIDTH;
    const double SCALE_Y = GAME_AREA_HEIGHT / (double)VIEW_HEIGHT;

    // Draw objects by depth (background to foreground)
    for (int depth = MAX_SPRITE_DEPTH; depth >= 0; --depth)
    {
        for (auto obj : graphObjects)
        {
            if (m_spriteDepth[obj->getID()] != depth || ! obj->isVisible())
				continue;
            obj->animate();

            double x, y;
            obj->getAnimationLocation(x, y);

            // Convert to screen coordinates
            // Since sprite origin is at center, we need to position the center of the sprite
            // at the center of each grid cell to avoid clipping
            float screenX = static_cast<float>((x + 0.5) * SCALE_X);
            float screenY = static_cast<float>((VIEW_HEIGHT - y - 0.5) * SCALE_Y + STATUS_TEXT_HEIGHT); // Flip Y axis properly and offset for status text

            int imageID = obj->getID();
			auto it = m_frameAdvanceInterval.find(imageID);
			if (it != m_frameAdvanceInterval.end()  &&  m_totalFramesDisplayed % it->second == 0)
				obj->increaseAnimationNumber();
            int frameNum = obj->getAnimationNumber() % getNumFrames(imageID);
            int textureKey = imageID * TEXTURE_KEY_MULTIPLIER + frameNum;

            auto textureIt = m_textures.find(textureKey);
            if (textureIt == m_textures.end())
                continue;

            // Use cached sprite or create new one if not exists
            sf::Sprite* sprite;
            auto spriteIt = m_spriteCache.find(textureKey);
            if (spriteIt != m_spriteCache.end())
            {
                sprite = spriteIt->second.get();
            }
            else
            {
                // Create new sprite and cache it
                auto newSprite = std::make_unique<sf::Sprite>(textureIt->second);
                sprite = newSprite.get();
                m_spriteCache[textureKey] = std::move(newSprite);
                
                // Get texture size for scaling calculations
                sf::Vector2u textureSize = textureIt->second.getSize();
                
                // Set origin to center for proper flipping
                sprite->setOrigin(sf::Vector2f(textureSize.x / 2.0f, textureSize.y / 2.0f));
            }
            
            // Update position and scale every frame (direction can change)
            sprite->setPosition(sf::Vector2f(screenX, screenY));
            
            // Calculate scale based on object size and coordinate system
            sf::Vector2u textureSize = sprite->getTexture().getSize();
            float scaleX = float(SCALE_X * obj->getSize() * SPRITE_WIDTH_GL) / textureSize.x;
            float scaleY = float(SCALE_Y * obj->getSize() * SPRITE_HEIGHT_GL) / textureSize.y;
            
            // Flip horizontally for left-facing sprites instead of rotating
            
            if (obj->getDirection() == GraphObject::left)
                scaleX = -scaleX;
            
            
            sprite->setScale(sf::Vector2f(scaleX, scaleY));

            m_window.draw(*sprite);
        }
    }

    drawScoreAndLives(m_gameStatText);
}

void GameController::drawPrompt(const string& mainMessage, const string& secondMessage)
{
    if (m_font.getInfo().family.empty())
        return; // No font loaded

    sf::Text mainText(m_font, mainMessage, 24);
    sf::Text secondText(m_font, secondMessage, 18);

    mainText.setFillColor(sf::Color::White);
    secondText.setFillColor(sf::Color::White);

    // Center the text
    sf::FloatRect mainBounds = mainText.getLocalBounds();
    sf::FloatRect secondBounds = secondText.getLocalBounds();

    mainText.setPosition(sf::Vector2f((WINDOW_WIDTH - mainBounds.size.x) / 2, WINDOW_HEIGHT / 2 - 50));
    secondText.setPosition(sf::Vector2f((WINDOW_WIDTH - secondBounds.size.x) / 2, WINDOW_HEIGHT / 2 + 20));

    m_window.draw(mainText);
    m_window.draw(secondText);
}

void GameController::drawScoreAndLives(const string& text)
{
    if (m_font.getInfo().family.empty())
        return; // No font loaded

    // Start with default font size and dynamically adjust if text is too wide
    const int PADDING = 20; // Leave some padding on each side
    const float MAX_TEXT_WIDTH = (WINDOW_WIDTH - (2 * PADDING));  
    int fontSize = 29;
    const int MIN_FONT_SIZE = 12; // Don't go below this size
    
    sf::Text scoreText(m_font, text, fontSize);
    scoreText.setFillColor(sf::Color::White);
    
    // Reduce font size if text is too wide
    sf::FloatRect textBounds = scoreText.getLocalBounds();
    while (textBounds.size.x > MAX_TEXT_WIDTH && fontSize > MIN_FONT_SIZE)
    {
        fontSize--;
        scoreText.setCharacterSize(fontSize);
        textBounds = scoreText.getLocalBounds();
    }
    
    // Center the text horizontally
    scoreText.setPosition(sf::Vector2f((WINDOW_WIDTH - textBounds.size.x) / 2, 10));

    m_window.draw(scoreText);
}

sf::Texture& GameController::getTexture(int imageID)
{
    return m_textures[imageID * TEXTURE_KEY_MULTIPLIER]; // Default to frame 0
}

void GameController::loadTexture(int imageID, const std::string& filename)
{
    sf::Texture texture;
    if (texture.loadFromFile(filename))
    {
        m_textures[imageID * TEXTURE_KEY_MULTIPLIER] = std::move(texture);
    }
}

int GameController::getNumFrames(int imageID)
{
    // Count how many frames exist for this image ID
    int count = 0;
    for (const auto& texture : m_textures)
    {
        if (texture.first / TEXTURE_KEY_MULTIPLIER == imageID)
            count++;
    }
    return max(1, count);
}

void GameController::reportLeakedGraphObjects() const
{
    set<GraphObject*>& graphObjects = GraphObject::getGraphObjects();
    if (graphObjects.empty())
        return;
    cerr << "***** " << graphObjects.size() << " leaked objects:" << endl;
    for (auto obj : graphObjects)
    {
        Coord coord = obj->getCoord();
        cerr << m_imageNameMap.at(obj->getID()) << " at (" << coord.x << "," << coord.y << ")" << endl;
    }
}
