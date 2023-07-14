#pragma once
#include <ostream>

namespace robopath {

	class LineSegment;
	
	class Rectangle;
	

	class Point {
	public:
		Point() {}
		Point(double x, double y) :
			x(x), y(y) {
		}

		double distanceTo(const Point& p) const;
		double distanceToSquared(const Point& p) const;

		bool collidesWith(const Point& p) const;
		bool collidesWith(const LineSegment& ls) const;
		
		bool collidesWith(const Rectangle& r) const;
		
		bool operator==(const Point& p) const;
		bool operator!=(const Point& p) const;
		bool operator<(const Point& p) const;
		bool operator<=(const Point& p) const;

	public:
		double x = 0.0, y = 0.0;
	};

	std::ostream& operator<<(std::ostream& os, const Point& p);
}

