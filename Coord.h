#ifndef COORD_H_
#define COORD_H_

struct Coord {
	Coord(int xx, int yy)
	 : x(xx), y(yy)
	{}
	int x;
	int y;
};

inline bool operator==(Coord lhs, Coord rhs)
{
	return lhs.x == rhs.x  &&  lhs.y == rhs.y;
}

inline bool operator!=(Coord lhs, Coord rhs)
{
	return !(lhs == rhs);
}

#endif // COORD_H_

