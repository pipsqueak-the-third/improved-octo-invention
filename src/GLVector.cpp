
#include "GLVector.hpp"

#include <cmath>

GLVector::GLVector() {}

GLVector::GLVector(double x, double y, double z) {
  vector[0] = x;
  vector[1] = y;
  vector[2] = z;
}

double GLVector::operator()(int i) const { return vector[i]; }

double &GLVector::operator()(int i) { return vector[i]; }

double GLVector::norm2() {
  return vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2];
}

double GLVector::norm() { return std::sqrt(norm2()); }

void GLVector::normalize() {
  double scale = 1.0 / norm();
  vector[0] *= scale;
  vector[1] *= scale;
  vector[2] *= scale;
}