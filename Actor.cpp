#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
//
/////////////////////////////////////////////////////////////////////////////////////
// Part 1 Work Area
// We need Actor (Base), FloorBrick (Derived) and IceMonster (Derived)
/////////////////////////////////////////////////////////////////////////////////////
// Part 2 Work Area
// We happen here is: we need to update a few methods and a few new class
// As a result, we will not distinguish the Part 1 and Part 2 with a clear boundary line
/////////////////////////////////////////////////////////////////////////////////////


// Constructor for each class

Actor::Actor(int imageID, Coord startCoord, StudentWorld* world, int dir) :
	GraphObject(imageID, startCoord, dir), m_world(world), m_state(0),m_dead(false){}

FloorBrick::FloorBrick(Coord startCoord, StudentWorld* world) :
	Actor(IID_FLOOR, startCoord, world, right) {}

IceMonster::IceMonster(Coord startCoord, StudentWorld* world) :
	Actor(IID_ICE_MONSTER, startCoord, world, right){}

Exit::Exit(Coord startCoord, StudentWorld* world) :
	Actor(IID_EXIT, startCoord, world, right) {
}

Trampoline::Trampoline(Coord startCoord, StudentWorld* world) :
	Actor(IID_TRAMPOLINE, startCoord, world, right) {
}


Spring::Spring(Coord startCoord, StudentWorld* world) :
	Actor(IID_SPRING, startCoord, world, right) {
}


Net::Net(Coord startCoord, StudentWorld* world) :
	Actor(IID_NET, startCoord, world, right) {
}


Pheromone::Pheromone(Coord startCoord, StudentWorld* world) :
	Actor(IID_PHEROMONE, startCoord, world, right) {
}


OneWayDoor::OneWayDoor(Coord startCoord, StudentWorld* world, int dir) :
	Actor(IID_ONE_WAY_DOOR, startCoord, world, dir) {
}


Bonfire::Bonfire(Coord startCoord, StudentWorld* world) :
	Actor(IID_BONFIRE, startCoord, world, right) {
}


LemmingFactory::LemmingFactory(Coord startCoord, StudentWorld* world) :
	Actor(IID_LEMMING_FACTORY, startCoord, world, none) {
}


Lemming ::Lemming(Coord startCoord, StudentWorld* world) :
	Actor(IID_LEMMING, startCoord, world, right) {
	mm_state = WALKING;
	fall_dist = 0;
	m_bounce_step = 0;
	m_phase = 0;
}

Cursor :: Cursor(StudentWorld* world) :
	Actor(IID_PLAYER, Coord(VIEW_WIDTH/2,VIEW_HEIGHT/2), world, right) {// specified latter the startCoord
}

/////////////////////////////////////////////////////////////////////////////////////
// Method Section
/////////////////////////////////////////////////////////////////////////////////////
// Actor Class
StudentWorld* Actor::getWorld() const {
	return m_world;
}

// isBlocking() returns judgement if it is brick
bool Actor::isBlocking() const {
	return false;
}

// isDead() returns judgement if character is dead
bool Actor::isDead() const {
	return m_dead;
}

// setDead() just sets character dead
void Actor::setDead() {
	m_dead = true;
}

// isExit() just returns true if character is exit
/*
bool Actor::isExit() const {
	return false;
}


// isTrampoline() just returns true if charater is trampoline

bool Actor::isTrampoline() const {
	return false;
}


// isDangerous() just returns true if character steps on a hazzard

bool Actor::isDangerous() const {
	return false;
}

// isSpring() just returns true if character is spring
bool Actor::isSpring() const {
	return false;
}
*/

// isNet() just returns true if character is net
bool Actor::isNet() const {
	return false;
}

/*
// isOneWay() just returns true if character is one-way door
bool Actor::isOneWay(int dir) const {
	return false;
}
*/

// isPheromone just returns true if character is Pheromone
bool Actor::isPheromone() const {
	return false;
}

bool Actor::isLemming() const {
	return false;
}

//Helper Function GetMyState()
int Actor::GetMyState() const {
	return m_state;
}

void Actor::SetMyState(int n) {
	m_state = n;
}

void Actor::reverseDirection() {
	int dir = getDirection();
	if (dir == left) {
		setDirection(right);
	}
	else {
		setDirection(left);
	}
}

int Actor::GetState() const {
	return -1;
}

bool Actor::isIceMonster() const {
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////
// FloorBrick Class
// isBlocking(() returns true if the character is Brick
bool FloorBrick::isBlocking() const {
	return true;
}


/////////////////////////////////////////////////////////////////////////////////////
// Exit Class
// isExit() returns true if it is exit
/*
bool Exit::isExit() const {
	return true;
}
*/

void Exit::doSomething() {
	Coord current = getCoord();
	getWorld()->saveLemmingAt(current);
}

/////////////////////////////////////////////////////////////////////////////////////
// Trampoline Class
// isTrampoline() returns true
/*
bool Trampoline::isTrampoline() const {
	return true;
}
*/

void Trampoline::doSomething() {
	Coord current = getCoord();
	getWorld()->bounceLemmingAt(current,0);
}


/////////////////////////////////////////////////////////////////////////////////////
// Spring Class
// isSpring() returns true if it is spring
/*
bool Spring::isSpring() const {
	return true;
}
*/

void Spring::doSomething() {
	Coord current = getCoord();
	getWorld()->bounceLemmingAt(current,1);
}


/////////////////////////////////////////////////////////////////////////////////////
// Net Class
// isNet() returns true if it is net

bool Net::isNet() const {
	return true;
}


/////////////////////////////////////////////////////////////////////////////////////
// Phermone Class
// isPhermone() returns true if it is Pheromone

bool Pheromone::isPheromone() const {
	return true;
}


/////////////////////////////////////////////////////////////////////////////////////
// OneWayDoor Class
// isOneWay(int dir) returns true if
// it is a one way door with right direction
/*
bool OneWayDoor::isOneWay(int dir) const {
	return getDirection() == dir;
}
*/

void OneWayDoor::doSomething() {
	Coord current = getCoord();
	int dir = getDirection();
	getWorld()->ForceAt(current,dir);
}

/////////////////////////////////////////////////////////////////////////////////////
// Bonfire Class
// Bonfire is dangerous
/*
bool Bonfire::isDangerous() const {
	return true;
}
*/

void Bonfire::doSomething() {
	Coord current = getCoord();
	getWorld()->burnLemmingAt(current);
}

/////////////////////////////////////////////////////////////////////////////////////
// Lemming Factory Class
void LemmingFactory::doSomething() {
	SetMyState(GetMyState() + 1);
	if (GetMyState() == 100) {
		SetMyState(0);
		if (m_numToSpawn > 0) {
			getWorld()->addActor(new Lemming(getCoord(), getWorld()));
			getWorld()->recordLemmingsSpawn();
			m_numToSpawn--;
		}
	}

	// Small update about timing requirement
	/*
	if (GetMyState() != 0) {
		SetMyState(GetMyState() + 1);
		if (GetMyState() == 100) {
			SetMyState(0);
		}
		return;
	}// Check if 100 ticks

	if (m_numToSpawn > 0) {
		getWorld()->addActor(new Lemming(getCoord(), getWorld()));
		getWorld()->recordLemmingsSpawn();
		SetMyState(GetMyState() + 1);
		m_numToSpawn--;
	}
	*/
}

/////////////////////////////////////////////////////////////////////////////////////
// Lemming Class
int Lemming::GetState() const {
	return mm_state;
}

void Lemming::SetBounceState(int k) {
	mm_state = BOUNCING;
	if (k == 0) {
		int tmp = fall_dist - 1; // trampoline bounce
		if (tmp < 0) {
			m_bounce_step = 0;
		}
		else {
			m_bounce_step = tmp;
		}
		fall_dist = 0;
		return;
	}
	else if (k == 1) {
		m_bounce_step = 15; // spring bounce
	}
}

// isLemming just returns true if character is Lemming
bool Lemming::isLemming() const {
	return true;
}

void Lemming::doSomething() {
	// PHASE 1: Check Activity
	if (isDead()) {
		return; // if it is dead, no more action
	}

	if (mm_state == WALKING) {
		if (GetMyState() != 0) {// Frozen
			SetMyState(GetMyState() + 1);
			if (GetMyState() == 4) {
				SetMyState(0);
			}
			return;
		}//Check for Activity
	}
	else {
		if (GetMyState() != 0) {// Frozen
			SetMyState(GetMyState() + 1);
			if (GetMyState() == 3) {
				SetMyState(0);
			}
			return;
		}//Check for Activity
	}

	// PHASE 2: Check PHEROMONE
	int targetX = getWorld()->getClosestPheromoneX(getCoord());
	if (targetX != -1) {
		if (targetX > getCoord().x) {
			setDirection(right);
		}
		else {
			setDirection(left);
		}
	}

	// PHASE 3: State Decision and Action Taking

	//WALKING STATE
	if (mm_state == WALKING) {
		Coord current = getCoord();
		if (getWorld()->checkClimbable(current)) {
			mm_state = CLIMBING;
			SetMyState(1);
			return;
		}
		else {
			int direction = getDirection();
			Coord next = getTargetCoord(direction);

			// solid obstruction
			if (getWorld()->isBlockingAt(next)) {
				reverseDirection();

				/* OLD VERSION
				if (direction == left) {
					setDirection(right);
				}
				else {
					setDirection(left);
				}// if it does, reverse direction
				*/
				SetMyState(GetMyState() + 1);
				return;
			}

			Coord belownext = getTargetCoord(next, down);

			// solid floor
			if (getWorld()->isBlockingAt(belownext)) {
				moveTo(direction); // move forward
				SetMyState(GetMyState() + 1);
			}
			else {
				moveTo(direction); // move forward
				mm_state = FALLING;
				fall_dist = 0;
				SetMyState(1);
			}
			return;
		}
	}	//FALLING STATE
	else if (mm_state == FALLING) {
		Coord current = getCoord();

		if (getWorld()->checkClimbable(current)) {
			mm_state = CLIMBING;
			fall_dist = 0;
			SetMyState(1);
			return;
		}

		Coord below = getTargetCoord(down);
		if (below.y < 0 || getWorld()->isBlockingAt(below)) {
			if (fall_dist > 5) {
				setDead();
				getWorld()->playSound(SOUND_LEMMING_DIE);
				getWorld()->recordLemmingDied();//fall down too high and die
				SetMyState(GetMyState() + 1);
				return;
			}
			else {
				mm_state = WALKING;
				SetMyState(1); // survive and start to walk next time
				return;
			}
		}
		else {
			fall_dist++;
			moveTo(down);
			SetMyState(GetMyState() + 1);
			return;
		}
	} 	//CLIMBING STATE
	else if (mm_state == CLIMBING) {
		Coord current = getCoord();

		if (!getWorld()->checkClimbable(current)) {
			mm_state = WALKING;
			SetMyState(1);
			return;
		}

		Coord above = getTargetCoord(up);

		if (above.y > 19 || getWorld()->isBlockingAt(above)) {
			SetMyState(GetMyState() + 1);
			return; // get stuck
		}
		else {
			moveTo(up);
			SetMyState(GetMyState() + 1);
			return;
		}
	} // BOUNCING STATE
	else if (mm_state == BOUNCING) {
		Coord current = getCoord();

		if (getWorld()->checkClimbable(current)) {
			mm_state = CLIMBING;
			SetMyState(1);
			return;
		}

		m_phase = 1;// upward phase
		if (m_bounce_step > 0 ) {
			Coord above = getTargetCoord(up);

			if (above.y < 19 && !getWorld()->isBlockingAt(above)) {
				moveTo(up);
				m_bounce_step--;
				SetMyState(GetMyState() + 1);
				return;
			}
			else {
				m_bounce_step = 0;
			}
		}

		if (m_bounce_step == 0) {
			m_phase = 0; // no more upward phase
		}

		if (m_phase == 0) {
			int direction = getDirection();
			Coord next = getTargetCoord(current, direction);

			if (next.x < 0 || next.x > 19 || getWorld()->isBlockingAt(next)) {
				reverseDirection();

				/* OLD VERSION
				if (direction == left) {
					setDirection(right);
				}
				else {
					setDirection(left);
				}// if it does, reverse direction
				*/
			}
			else {
				moveTo(direction);
			}
			mm_state = FALLING;
			fall_dist = 0;
			SetMyState(1);
			return;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// IceMonster Class
// IceMonster is dangerous
/*
bool IceMonster::isDangerous() {
	return true;
}
*/
bool IceMonster::isIceMonster() const {
	return true;
}

void IceMonster::doSomething() {
	Coord current = getCoord();
	bool flag = getWorld()->freezeLemmingAt(current);

	if (GetMyState() != 0) {// Frozen
		SetMyState(GetMyState() + 1);
		if (GetMyState() == 10) {
			SetMyState(0);
		}
		return;
	}

	// If there is a lemming, freeze it and do nothing else
	if (flag) {
		return;
	}

	// Active
	int direction = getDirection();
	Coord next = getTargetCoord(direction);
	Coord belowNext = getTargetCoord(next, down);

	// Want Helper Function isBlocking
	// Want to make sure: 1. solid ground 2. no barrier
	if (getWorld()->isBlockingAt(next) || !getWorld()->isBlockingAt(belowNext)) {
		// Change Direction: swap left and right
		reverseDirection();

		/* OLD VERSION
		if (direction == right) {
			setDirection(left);
		}
		else {
			setDirection(right);
		}
		*/
		SetMyState(GetMyState() + 1);
		return;
	}
	else {
		// Want Ahead
		moveTo(direction);
		SetMyState(GetMyState() + 1);
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// Cursor Class
void Cursor::attemptPlacement(char toolID) {
	if (!getWorld()->checkEmpty(getCoord())) {
		return; // if there is already something, do nothing
	}
	if (!getWorld()->checkTool(toolID)) {
		return; // if the player has no more tool, do nothing
	}
	switch (toolID) {
	case 't': case 'T': // Trampoline
		getWorld()->addActor(new Trampoline(getCoord(), getWorld()));
		break;
	case 'n': case 'N': // Net
		getWorld()->addActor(new Net(getCoord(), getWorld()));
		break;
	case 'p': case 'P': // Pheromone
		getWorld()->addActor(new Pheromone(getCoord(), getWorld()));
		break;
	case 's': case 'S': // Spring
		getWorld()->addActor(new Spring(getCoord(), getWorld()));
		break;
	case '<': // One-Way Left
		getWorld()->addActor(new OneWayDoor(getCoord(), getWorld(), left));
		break;
	case '>': // One-Way Right
		getWorld()->addActor(new OneWayDoor(getCoord(), getWorld(), right));
		break;
	}
}

void Cursor::doSomething() {
	int ch;
	Coord current = getCoord();
	if (getWorld()->getKey(ch)) {
		switch (ch) {
		case KEY_PRESS_UP:
			if (current.y < VIEW_HEIGHT - 2) moveTo(Coord(current.x, current.y + 1));
			break;
		case KEY_PRESS_DOWN:
			if (current.y > 1) moveTo(Coord(current.x, current.y - 1));
			break;
		case KEY_PRESS_LEFT:
			if (current.x > 1) moveTo(Coord(current.x - 1, current.y));
			break;
		case KEY_PRESS_RIGHT:
			if (current.x < VIEW_WIDTH - 2) moveTo(Coord(current.x + 1, current.y));
			break;
		case 't': case 'T': // Trampoline
			attemptPlacement('T');
			break;
		case 'n': case 'N': // Net
			attemptPlacement('N');
			break;
		case 'p': case 'P': // Pheromone
			attemptPlacement('P');
			break;
		case 's': case 'S': // Spring
			attemptPlacement('S');
			break;
		case '<': // One-Way Left
			attemptPlacement('<');
			break;
		case '>': // One-Way Right
			attemptPlacement('>');
			break;
		}
	}
	return;
}

/////////////////////////////////////////////////////////////////////////////////////