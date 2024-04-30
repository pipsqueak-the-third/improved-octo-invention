#include "GLPoint.hpp"

GLPoint::GLPoint() {}

GLPoint::GLPoint(double x, double y, double z) {
  point[0] = x;
  point[1] = y;
  point[2] = z;
}

double GLPoint::operator()(int i) const { return point[i]; }

double &GLPoint::operator()(int i) { return point[i]; }