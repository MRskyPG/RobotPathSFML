#include "point.h"
#include "objects.h"
#include "util.h"

#include <cmath>



namespace robopath {

    namespace {
        static const double EPS = 1e-8;

        bool equal(double a, double b) {
            return abs(a - b) < EPS;
        }
    }

    double Point::distanceTo(const Point& p) const {
        return std::sqrt(distanceToSquared(p));
    }

    double Point::distanceToSquared(const Point& p) const {
        return (x - p.x) * (x - p.x) + (y - p.y) * (y - p.y);
    }

    bool Point::collidesWith(const Point& p) const {
        return areEqual(x, p.x) && areEqual(y, p.y);
    }

    bool Point::collidesWith(const LineSegment& ls) const {
        const auto& A = ls.start;
        const auto& B = ls.end;

        const auto cross = Vector::cross(A, B, A, *this); // AB cross AP

        // Check cross product for zero
        if (!isZero(cross)) {
            return false;
        }

        const auto dot = Vector::dot(A, B, A, *this); // AB dot AP
        if (dot < 0.0) {
            return false;
        }

        const auto length = Vector::dot(A, B, A, B); // AB dot AB
        return (dot <= length);
    }

   

    bool Point::collidesWith(const Rectangle& r) const {
        return r.getXInterval().includes(x) &&
            r.getYInterval().includes(y);
    }

   
    bool Point::operator==(const Point& p) const {
        return equal(x, p.x) && equal(y, p.y);
    }

    bool Point::operator!=(const Point& p) const {
        return !equal(x, p.x) || !equal(y, p.y);
    }

    bool Point::operator<(const Point& p) const {
        return (x < p.x) || (equal(x, p.x) && y < p.y);
    }

    bool Point::operator<=(const Point& p) const {
        return (x < p.x) || (equal(x, p.x) && ((y < p.y) || equal(y, p.y)));
    }

    std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }

}
