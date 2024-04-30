#pragma once

#include "Color.hpp"

struct Material {
 public:
  Material()
      : smooth(false),
        reflection(0.0),
        refraction(0.0),
        transparency(0.0),
        color(Color(0, 0, 0)) {}

  bool smooth;
  float reflection;
  float refraction;
  float transparency;
  Color color;
};
