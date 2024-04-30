#pragma once

#include <math.h>

#include <iostream>

class GLVector {
 public:
  GLVector();

  GLVector(double x, double y, double z);

  double operator()(int i) const;

  double &operator()(int i);

  double norm2();

  double norm();

  void normalize();

 private:
  friend class GLMatrix;
  double vector[3];
};

/**
** Gibt die Einträge des Vektors auf drei Nachkommastellen gerundet aus
**/
inline std::ostream &operator<<(std::ostream &os, const GLVector &m) {
  os << "( ";
  for (size_t j = 0; j < 3; ++j) {
    os << round(m(j) * 1000.0f) / 1000.0f << " ";
  }
  os << " )";
  return os;
}
