#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <list>
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();

  // Destructor
  virtual ~StudentWorld();

  // Helper function
  bool isBlockingAt(Coord place) const;

  void addActor(Actor* a);

  int getClosestPheromoneX(Coord c) const;

  bool checkClimbable(Coord c) const;

  void recordLemmingDied();

  void recordLemmingsSpawn();

  void burnLemmingAt(Coord c);

  bool freezeLemmingAt(Coord c);

  void bounceLemmingAt(Coord c, int k);

  void saveLemmingAt(Coord c);

  bool checkEmpty(Coord c) const;

  bool checkTool(char tool);

  void ForceAt(Coord c, int dir);

protected:
	// More Helper
  void updateDisplayText();
  
  void recordLemmingSaved();

  void Reset();

private:
	std::list<Actor*> m_actors;
	Actor* m_cursor = nullptr;

	int m_numLemmingsToSpawn;
	int m_numLemmingsInWorld;
	int m_numLemmingsSaved;
	int m_numLemmingsDied;
	int m_LastLemmingSpawnedTick;
	int m_gameTime;
	std::string m_tool;

};

#endif // STUDENTWORLD_H_
