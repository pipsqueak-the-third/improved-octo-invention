#pragma once

#include "Color.hpp"
#include "math.hpp"

struct Ray {
 public:
  Ray() {}

 public:
  // initialized by the main loop or any iteration
  GLPoint origin;
  GLVector direction;

  void print() const {
    std::cout << "Ray origin: " << origin << " direction: " << direction
              << std::endl;
  }
};

struct HitRecord {
 public:
  // initialized by intersection function
  Color color;       // accumulated color
  double parameter;  // distance from eye to intersection point

  GLPoint intersectionPoint;  // point where current ray intersects with the object
  GLVector rayDirection;      // current ray direction relevant for shading
  GLVector normal;  // normal at the intersection point relevant for shading

  int recursions;  // number of recursions already processed

  int triangleId;  // triangle index of the object hit by ray (default should be
                   // -1)
  int modelId;   // model index of the object hit by ray (default should be -1)
  int sphereId;  // sphere index of the object hit by ray (default should be -1)

  void print() const {
    std::cout << "Intersection at Parameter: " << parameter << " with triangle "
              << triangleId << " at point " << intersectionPoint(0) << ", "
              << intersectionPoint(1) << ", " << intersectionPoint(2)
              << " with normal " << normal << std::endl;
  }
};

struct Triangle {
  std::array<GLPoint, 3> vertex;
  GLVector normal;
};
