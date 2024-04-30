#pragma once

/**
 ** Klasse für Farbdefinitionen
 ** Der Wertebereich je Farbkanal liegt zwischen 0.0 und 1.0
 **/
class Color {
 public:
  Color();

  Color(float r, float g, float b);

  Color &operator*=(double i);

  Color &operator/=(double i);

  Color &operator+=(const Color &rhs);

  bool operator!=(const Color &rhs);

  float r, g, b;
};
