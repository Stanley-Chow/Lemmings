#ifndef GRAPHOBJ_H_
#define GRAPHOBJ_H_

#include "GameConstants.h"
#include "Coord.h"
#include <set>
#include <cmath>

const int ANIMATION_POSITIONS_PER_TICK = 1;

class GraphObject
{
  public:

    static const int none = -1;
	static const int right = 0;
	static const int left = 180;
	static const int up = 90;
	static const int down = 270;

	GraphObject(int imageID, Coord startCoord, int dir = right, double size = DEFAULT_SPRITE_SCALE)
	 : m_imageID(imageID), m_visible(true), m_x(startCoord.x), m_y(startCoord.y),
	   m_destX(startCoord.x), m_destY(startCoord.y), m_brightness(1.0),
	   m_animationNumber(0), m_direction(dir), m_size(size)
	{
		if (m_size <= 0)
			m_size = 1;

		getGraphObjects().insert(this);
		setVisible(true);
	}

	virtual ~GraphObject()
	{
		getGraphObjects().erase(this);
	}

	void setVisible(bool shouldIDisplay)
	{
		m_visible = shouldIDisplay;
	}

	void setBrightness(double brightness)
	{
		m_brightness = brightness;
	}

	Coord getCoord() const
	{
		  // If already moved but not yet animated, use new location anyway.
		int x = static_cast<int>(m_destX);
		int y = static_cast<int>(m_destY);
		return Coord(x, y);
	}

	Coord getTargetCoord(int dir) const
	{
		return getTargetCoord(getCoord(), dir);
	}

	Coord getTargetCoord(Coord base, int dir) const
	{
		Coord target = base;
		switch (dir) {
			case right: // East
				target.x += 1;
				break;
			case left: // West
				target.x -= 1;
				break;
			case up: // North
				target.y += 1;
				break;
			case down: // South
				target.y -= 1;
				break;
			default:
				// Invalid direction, return base coord
				break;
		}
		return target;
	}

	virtual void moveTo(Coord coord)
	{
		m_destX = coord.x;
		m_destY = coord.y;
		increaseAnimationNumber();
	}

	virtual void moveTo(int direction)
	{
		Coord current = getCoord();
		switch (direction) {
			case right: // East
				current.x += 1;
				break;
			case left: // West
				current.x -= 1;
				break;
			case up: // North
				current.y += 1;
				break;
			case down: // South
				current.y -= 1;
				break;
			default:
				return; // Invalid direction
		}
		moveTo(current);
	}

	virtual void moveAngle(int angle, int units = 1)
	{
		double newX;
		double newY;
		getPositionInThisDirection(angle, units, newX, newY);
		int x = static_cast<int>(newX);
		int y = static_cast<int>(newY);
		moveTo(Coord(x, y));
	}

	virtual void getPositionInThisDirection(int angle, int units, double& dx, double& dy)
	{
		static const double PI = 4 * atan(1.0);
		Coord coord = getCoord();
		dx = (coord.x + units * cos(angle*1.0 / 360 * 2 * PI));
		dy = (coord.y + units * sin(angle*1.0 / 360 * 2 * PI));
	}

	void moveForward(int units = 1)
	{
		moveAngle(getDirection(), units);
	}


	int getDirection() const
	{
		return m_direction;
	}

	void setDirection(int d)
	{
		while (d < 0)
			d += 360;

		m_direction = d % 360;
	}

	void setSize(double size)
	{
		m_size = size;
	}

	double getSize() const
	{
		return m_size;
	}

	double getRadius() const
	{
		const int kRadiusPerUnit = 8;
		return kRadiusPerUnit * m_size;
	}

private:
	friend class GameController;
	int getID() const
	{
		return m_imageID;
	}

	static std::set<GraphObject*>& getGraphObjects()
	{
		static std::set<GraphObject*> graphObjects;
		return graphObjects;
	}

	void increaseAnimationNumber()
	{
		m_animationNumber++;
	}

	int getAnimationNumber() const
	{
		return m_animationNumber;
	}

	void getAnimationLocation(double& x, double& y) const
	{
		x = m_x;
		y = m_y;
	}

	void animate()
	{
		m_x = m_destX;
		m_y = m_destY;
	}

	bool isVisible() const
	{
		return m_visible;
	}

	double getBrightness() const
	{
		return m_brightness;
	}

  private:
	  // Prevent copying or assigning GraphObjects
	GraphObject(const GraphObject&);
	GraphObject& operator=(const GraphObject&);

	static const int NUM_DEPTHS = 4;
	int		m_imageID;
	bool	m_visible;
	double	m_x;
	double	m_y;
	double	m_destX;
	double	m_destY;
	double	m_brightness;
	int	m_animationNumber;
	int	m_direction;
	double	m_size;

};

#endif // GRAPHOBJ_H_
