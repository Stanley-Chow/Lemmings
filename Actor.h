#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Gameplay actor hierarchy and behavior contracts.

class StudentWorld;

class Actor : public GraphObject{
public:
	// Constructor
	Actor(int imageID, Coord startCoord, StudentWorld* world, int dir);

	// Public Method
	virtual void doSomething() = 0;

	StudentWorld* getWorld() const;

	// Destructor
	virtual ~Actor() {}

	// New Method for Part 2
	// isBlocking() returns judgement if it is brick
	virtual bool isBlocking() const;

	// isDead() returns judgement if character is dead
	virtual bool isDead() const;

	// setDead() just sets character dead
	void setDead();

	// isExit() just returns true if character is exit
	//virtual bool isExit() const;

	// isTrampoline() just returns true if charater is trampoline
	//virtual bool isTrampoline() const;

	// isDangerous() just returns true if character steps on a hazzard
	//virtual bool isDangerous() const;

	// isSpring() just returns true if character is spring
	//virtual bool isSpring() const;

	// isNet() just returns true if character is net
	virtual bool isNet() const;

	// isOneWay() just returns true if character is one-way door
	//virtual bool isOneWay(int dir) const;

	// isPheromone just returns true if character is Pheromone
	virtual bool isPheromone() const;

	// isLemming just returns true if character is Pheromone
	virtual bool isLemming() const;

	// Helper Function GetMyState()
	int GetMyState() const;

	// Helper Function SetMyState(int n)

	virtual bool isIceMonster() const;

	virtual int GetState() const;

	virtual void SetBounceState(int k) {}

protected:
	virtual void attemptPlacement(char toolID) {}

	// I want to have one more function:
	// reverseDirection() just reverses the direction of the character

	void reverseDirection();

	void SetMyState(int n);
private:
	StudentWorld* m_world;
	int m_state;

	// New Method for Part 2
	// m_dead stores info if the character is dead
	bool m_dead;
};



class FloorBrick : public Actor {
public:
	// Constructor
	FloorBrick(Coord startCoord, StudentWorld* world);

	// Public Method
	// FloorBrick does nothing per tick
	virtual void doSomething(){}

	// Destructor
	virtual ~FloorBrick() {}

	// New Method for Part 2
	// isBlocking(() returns true if the character is Brick
	virtual bool isBlocking() const;

private:
};



class Exit :public Actor {
public:
	// Constructor
	Exit(Coord startCoord, StudentWorld* world);

	// Public Method
	// Exit does nothing per tick
	virtual void doSomething();

	// isExit() returns true if it is exit
	//virtual bool isExit() const;

	// Destructor
	virtual ~Exit() {}
private:
};



class Trampoline :public Actor {
public:
	// Constructor
	Trampoline(Coord startCoord, StudentWorld* world);

	// Public Method
	// Trampoline stays still, which means it is reactive
	virtual void doSomething();

	// isTrampoline() returns true
	//virtual bool isTrampoline() const;

	// Destructor
	virtual ~Trampoline() {}
private:
};



class Spring : public Actor {
public:
	// Constructor
	Spring(Coord startCoord, StudentWorld* world);

	// Public Method
	// Spring stays still, which means it is reactive
	virtual void doSomething();

	// isSpring() returns true if it is spring
	//virtual bool isSpring() const;

	// Destructor
	virtual ~Spring() {}
private:
};



class Net : public Actor {
public:
	// Constructor
	Net(Coord startCoord, StudentWorld* world);

	// Public Method
	// Net stays still, which means it is reactive
	// Update: Net is included in the Lemming::doSomething() to check if it is climbable,
	// so we will not make it reactive anymore
	virtual void doSomething(){}

	// isNet() returns true if it is net
	virtual bool isNet() const;

	// Destructor
	virtual ~Net() {}
private:
};



class Pheromone : public Actor {
public:
	// Constructor
	Pheromone(Coord startCoord, StudentWorld* world);

	// Public Method
	// Pheromone decays overtime
	virtual void doSomething() {}

	// isPhermone() returns true if it is Pheromone
	virtual bool isPheromone() const;

	// Destructor
	virtual ~Pheromone() {}
private:
};



class OneWayDoor :public Actor {
public:
	// Constructor
	OneWayDoor(Coord startCoord, StudentWorld* world, int dir);

	// Public Method
	// One way door stays still
	virtual void doSomething();

	// isOneWay(int dir) returns true if
	// it is a one way door with right direction
	//virtual bool isOneWay(int dir) const;

	// Destructor
	virtual ~OneWayDoor() {}
private:
};



class Bonfire : public Actor {
public:
	// Constructor
	Bonfire(Coord startCoord, StudentWorld* world);

	// Public Method
	// Bonfire stays still
	virtual void doSomething();

	// Bonfire is dangerous
	//virtual bool isDangerous() const;

	// Destructor
	virtual ~Bonfire() {}
};



class LemmingFactory : public Actor {
public:
	LemmingFactory(Coord startCoord, StudentWorld* world);

	// Spawns lemmings every 10 ticks
	virtual void doSomething();

	// Destructor
	virtual ~LemmingFactory() {}

private:
	int m_numToSpawn = 10;
};



class Lemming : public Actor {
public:
	// Constructor
	Lemming(Coord startCoord, StudentWorld* world);

	// Public Method
	virtual void doSomething();

	// Destructor
	virtual ~Lemming() {}

	virtual bool isLemming() const;

	// Helper Function for Bouncing
	virtual int GetState() const;
	virtual void SetBounceState(int k); // k = 0 means Trampoline, k = 1 means Spring

private:
	enum State {
		WALKING,
		FALLING,
		BOUNCING,
		CLIMBING
	};
	State mm_state;
	int fall_dist;
	int m_bounce_step;
	int m_phase;
};



class IceMonster : public Actor {
public:
	// Constructor
	IceMonster(Coord startCoord, StudentWorld* world);

	// Public Method
	// Use this function for one way door
	virtual bool isIceMonster() const;

	// IceMonster does something every 10 ticks
	virtual void doSomething();

	// IceMonster is dangerous
	//virtual bool isDangerous();

	// Destructor
	virtual ~IceMonster() {}

private:
};



class Cursor : public Actor {
public:
	// Constructor
	Cursor(StudentWorld* world);

	// Public Method
	virtual void attemptPlacement(char toolID);
	// Cursor does something anyway for each tick
	virtual void doSomething();

	// Destructor
	virtual ~Cursor() {}
private:
};
/////////////////////////////////////////////////////////////////////////////////////

#endif // ACTOR_H_