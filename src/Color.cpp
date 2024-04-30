

#include "Color.hpp"

Color::Color() {}

Color::Color(float r, float g, float b) : r(r), g(g), b(b) {}

Color &Color::operator*=(double i) {
  r *= i;
  g *= i;
  b *= i;
  return *this;
}

Color &Color::operator/=(double i) {
  double frac = 1.0 / i;
  r *= frac;
  g *= frac;
  b *= frac;
  return *this;
}

Color &Color::operator+=(const Color &rhs) {
  r += rhs.r;
  g += rhs.g;
  b += rhs.b;

  return *this;
}

bool Color::operator!=(const Color &rhs) {
  if( r != rhs.r or g != rhs.g or b != rhs.b )
      return true;
  else
      return false;
}