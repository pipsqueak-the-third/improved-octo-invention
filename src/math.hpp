#pragma once

#include <array>
#include <vector>
#include <limits>
#include <memory>

#include "GLMatrix.hpp"
#include "GLPoint.hpp"
#include "GLVector.hpp"

inline GLVector operator*(const GLVector &lhs, double scale) {
  return GLVector(lhs(0) * scale, lhs(1) * scale, lhs(2) * scale);
}

inline GLVector operator*(double scale, const GLVector &rhs) {
  return GLVector(rhs(0) * scale, rhs(1) * scale, rhs(2) * scale);
}

inline GLPoint operator*(const GLPoint &lhs, double scale) {
  return GLPoint(lhs(0) * scale, lhs(1) * scale, lhs(2) * scale);
}

inline GLPoint operator*(double scale, const GLPoint &rhs) {
  return GLPoint(rhs(0) * scale, rhs(1) * scale, rhs(2) * scale);
}

inline GLPoint operator+(const GLPoint &p1, const GLVector &p2) {
  return GLPoint(p1(0) + p2(0), p1(1) + p2(1), p1(2) + p2(2));
}

inline GLPoint operator+(const GLPoint &p1, const GLPoint &p2) {
  return GLPoint(p1(0) + p2(0), p1(1) + p2(1), p1(2) + p2(2));
}

inline GLVector operator+(const GLVector &p1, const GLVector &p2) {
  return GLVector(p1(0) + p2(0), p1(1) + p2(1), p1(2) + p2(2));
}

inline GLVector operator-(const GLPoint &p1, const GLPoint &p2) {
  return GLVector(p1(0) - p2(0), p1(1) - p2(1), p1(2) - p2(2));
}

inline GLVector crossProduct(const GLVector &lhs, const GLVector &rhs) {
  return GLVector(lhs(1) * rhs(2) - lhs(2) * rhs(1),
                  lhs(2) * rhs(0) - lhs(0) * rhs(2),
                  lhs(0) * rhs(1) - lhs(1) * rhs(0));
}

inline double dotProduct(const GLVector &lhs, const GLVector &rhs) {
  return lhs(0) * rhs(0) + lhs(1) * rhs(1) + lhs(2) * rhs(2);
}

inline int sgn(int x) { return (x > 0) ? 1 : (x < 0) ? -1 : 0; }

/** Aufgabenblatt 2, Aufgabe 2 **/
