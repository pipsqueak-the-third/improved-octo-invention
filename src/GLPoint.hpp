#pragma once

#include <math.h>

#include <iostream>

class GLPoint {
 public:
  GLPoint();

  GLPoint(double x, double y, double z);

  double operator()(int i) const;

  double &operator()(int i);

 private:
  friend class GLMatrix;
  double point[3];
};

/**
** Gibt die Einträge des Punktes auf drei Nachkommastellen gerundet aus
**/
inline std::ostream &operator<<(std::ostream &os, const GLPoint &m) {
  os << "[ ";
  for (size_t j = 0; j < 3; ++j) {
    os << round(m(j) * 1000.0f) / 1000.0f << " ";
  }
  os << " ]";
  return os;
}
