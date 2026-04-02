#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp
// Do not change or remove the createStudentWorld implementation above.

StudentWorld::StudentWorld(string assetPath)
	: GameWorld(assetPath),
      m_cursor(nullptr),
      m_numLemmingsToSpawn(10),
      m_numLemmingsInWorld(0),
      m_numLemmingsSaved(0),
      m_numLemmingsDied(0),
      m_LastLemmingSpawnedTick(0),
      m_gameTime(2000)
{
}

void StudentWorld::addActor(Actor* a) {
    m_actors.push_back(a);
}

bool StudentWorld::isBlockingAt(Coord place) const {
    for (auto* actor : m_actors) {
        if (actor->getCoord() == place && actor->isBlocking()) {
            return true;
        }
    }
    return false;
}

int StudentWorld::getClosestPheromoneX(Coord c) const {
    int closestX = -1;
    int min_dist = 100;

    for (Actor* a : m_actors) {
        if (a->getCoord().y == c.y && a->isPheromone()) {
            int dist = abs(a->getCoord().x - c.x);
            if (dist >= 1 && dist <= 5) {
                if (dist < min_dist) {
                    min_dist = dist;
                    closestX = a->getCoord().x;
                }
            }
        }
    }
    return closestX;
}

bool StudentWorld::checkClimbable(Coord c) const {
    for (Actor* a : m_actors) {
        if (a->isNet() && a->getCoord()==c) {
            return true;
        }
    }
    return false;
}

void StudentWorld::recordLemmingSaved() {
	increaseScore(SCORE_SAVED_LEMMING);
    m_numLemmingsSaved++;
    m_numLemmingsInWorld--;
}

void StudentWorld::recordLemmingDied() {
    m_numLemmingsDied++;
    m_numLemmingsInWorld--;
}

void StudentWorld::recordLemmingsSpawn() {
    m_numLemmingsToSpawn--;
    m_numLemmingsInWorld++;
    if (m_numLemmingsToSpawn == 0) {
        m_LastLemmingSpawnedTick = m_gameTime;
	}     
}

void StudentWorld::burnLemmingAt(Coord c) {
    for (Actor* a : m_actors) {
		if (a->getCoord() == c && a->isLemming()) {
            a->setDead();
            playSound(SOUND_LEMMING_DIE);
			recordLemmingDied();
        }
    }
}

bool StudentWorld::freezeLemmingAt(Coord c) {
	bool hasFrozen = false;
    for (Actor* a : m_actors) {
        if (a->getCoord() == c && a->isLemming()) {
            a->setDead();
            playSound(SOUND_LEMMING_DIE);
            recordLemmingDied();
			hasFrozen = true;
        }
    }
	return hasFrozen;
}

void StudentWorld::bounceLemmingAt(Coord c,int k) {
    for (Actor* a : m_actors) {
        if (a->getCoord() == c && a->isLemming()) {
			if (a->GetState() != 2) { // if not already bounced
                a->SetBounceState(k);
                playSound(SOUND_BOUNCE);
            }
        }
    }
}

void StudentWorld::saveLemmingAt(Coord c) {
    for (Actor* a : m_actors) {
        if (a->getCoord() == c && a->isLemming()) {
            a->setDead();
			playSound(SOUND_LEMMING_SAVED);
			recordLemmingSaved();
            return;
        }
	}
}

bool StudentWorld::checkEmpty(Coord c) const {
    for (Actor* a : m_actors) {
        if (a->getCoord() == c) {
            return false;
		}
	}
    return true;
}

bool StudentWorld::checkTool(char tool) {
    if (m_tool.find(tool) != string::npos) {
		m_tool.erase(m_tool.find(tool), 1);
        return true;
    }
	return false;
}

void StudentWorld::ForceAt(Coord c, int dir) {
    for (Actor* a : m_actors) {
        if (a->getCoord() == c && 
            (a->isLemming() || a->isIceMonster())) {
            a->setDirection(dir);
            //return;
        }
    }
}

void StudentWorld::Reset() {
    m_numLemmingsToSpawn = 10;
    m_numLemmingsInWorld = 0;
    m_numLemmingsSaved = 0;
	m_LastLemmingSpawnedTick = 0;
    m_numLemmingsDied = 0;
    m_gameTime = 2000;
	m_tool = "";
}

void StudentWorld::updateDisplayText() {
    ostringstream oss;

    oss << "Score: " << setfill('0') << setw(5) << getScore();
    oss << "  ";
    oss << "Level: " << setfill('0') << setw(2) << getLevel();
    oss << "  ";
    oss << "Lives: " << setfill('0') << setw(2) << getLives();
    oss << "  ";
    oss << "Saved: " << setfill('0') << setw(2) << m_numLemmingsSaved;
    oss << "  ";
    string toolDisplay = m_tool;
    if (toolDisplay == "") {
        toolDisplay = "None";
    }
    oss << "Tools: " << toolDisplay;
    oss << "  ";
    oss << "Time left: " << setfill('0') << setw(4) << m_gameTime;
    setGameStatText(oss.str());
}

int StudentWorld::init()
{
    int levNum = getLevel();
    string levelFile;

    if (levNum < 10) {
        levelFile = "level0" + to_string(levNum) + ".txt";
    }
    else {
        levelFile = "level" + std::to_string(levNum) + ".txt";
    }

    Level lev(assetPath());
    Level::LoadResult result = lev.loadLevel(levelFile);

    if (result == Level::load_fail_file_not_found) {
        return GWSTATUS_PLAYER_WON;
    }
    else if (result == Level::load_fail_bad_format) {
        return GWSTATUS_LEVEL_ERROR;
    }
    else {
		Reset();
        m_tool = lev.getTools();
        for (int x = 0; x < VIEW_WIDTH; x++) {
            for (int y = 0; y < VIEW_HEIGHT; y++) {
                Level::MazeEntry item = lev.getContentsOf(Coord(x, y));
                if (item == Level::floor) {
                    m_actors.push_back(new FloorBrick(Coord(x, y), this));
                }
                else if (item == Level::ice_monster) {
                    m_actors.push_back(new IceMonster(Coord(x, y), this));
                }
                else if (item == Level::lemming_factory) {
                    m_actors.push_back(new LemmingFactory(Coord(x, y), this));
                }
                else if (item == Level::bonfire) {
                    m_actors.push_back(new Bonfire(Coord(x, y), this));
                }
                else if (item == Level::trampoline) {
                    m_actors.push_back(new Trampoline(Coord(x, y), this));
                }
                else if (item == Level::spring) {
                    m_actors.push_back(new Spring(Coord(x, y), this));
                }
                else if (item == Level::net) {
                    m_actors.push_back(new Net(Coord(x, y), this));
                }
                else if (item == Level::left_one_way_door) {
                    m_actors.push_back(new OneWayDoor(Coord(x, y), this,GraphObject::left));
                }
                else if (item == Level::right_one_way_door) {
                    m_actors.push_back(new OneWayDoor(Coord(x, y), this, GraphObject::right));
                }
                else if (item == Level::pheromone) {
                    m_actors.push_back(new Pheromone(Coord(x, y), this));
				}
                else if (item == Level::lemming_exit) {
                    m_actors.push_back(new Exit(Coord(x, y), this));
				}
            }
        }
		m_cursor = new Cursor(this);
        return GWSTATUS_CONTINUE_GAME;
    }
}

int StudentWorld::move()
{
	m_gameTime--;

	m_cursor->doSomething();
    for (auto it = m_actors.begin(); it != m_actors.end(); ++it) {
        if (!(*it)->isDead()) {
            (*it)->doSomething();

            // input something here latter to check
            // if we save enough lemmings 
            // if yes, return WIN
            if (m_numLemmingsSaved >= 5 && m_numLemmingsToSpawn == 0 
                && m_numLemmingsInWorld == 0) {
				increaseScore(m_gameTime);
				playSound(SOUND_FINISHED_LEVEL);
                return GWSTATUS_FINISHED_LEVEL;
            }

        }
    }

    auto it = m_actors.begin();
    while (it != m_actors.end()) {
        if ((*it)->isDead()) {
            delete* it;
            it = m_actors.erase(it);
        }
        else {
            ++it;
        }
    }

	if (m_numLemmingsDied > 5) {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }

    if (m_gameTime == 0 && m_numLemmingsSaved < 5) {
		decLives();
        return GWSTATUS_PLAYER_DIED;
    }

    if (m_gameTime == 0 && m_numLemmingsSaved >= 5) {
		playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
	updateDisplayText();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (auto it = m_actors.begin(); it != m_actors.end(); ) {
        delete* it;
        it = m_actors.erase(it);
    }
	if (m_cursor) {
        delete m_cursor;
        m_cursor = nullptr;
	}
}

StudentWorld::~StudentWorld() {
    cleanUp();
}